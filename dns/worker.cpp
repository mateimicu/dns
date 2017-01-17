/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <string>
#include <string.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include "server.h"
#include "db.h"
#include "dns.h"
#include "exceptions.h"
#include "parser.h"
#include "reader.h"
#include "worker.h"

void run(Worker* w)
{
    /* Pornim workerul
     *
     * @param w
     *  Workerul pe care dorim sa il pornim
     */
    std::cout << "Apelam work !" << std::endl;
    w->work();
}

Worker::Worker(char* db_name, std::mutex *lock, Reader* rd, Parser* pr, int id):
    parser(pr)
{
    /* Pregatim un worker
     *
     * @param[in] db_name
     *  Numele bazei de date
     *
     * @param[in] *lock
     *  Lock-ul pentru pool
     *
     * @param[in] *rd
     *  Un obiect de tip reader
     *
     * @param[in] *pr
     *  Un parser sa vedem optiunile de la CLI
     *
     * @param[in] id
     *  Id-ul pentru acest worker
     */
    this->id = id;
    std::cout << "Pornim Worker - "<< this->id << std::endl;
    this->parser = pr;
    this->db = new DB(db_name);
    this->lock = lock;

    this->rd = rd;
    this->stop = false;
}

Worker::~Worker()
{
    /* Deletam tot */
    delete this->db;
}

void Worker::sign_stop()
{
    /* Trimite semnalul de oprire */
    std::cout << "Trimitem semnanul de oprire" << std::endl;
    this->stop = true;
}

void Worker::work()
{
    /* Realizam un ciclu */
    std::cout << "[" << this->id << "] Work apel !" << std::endl;
    while (this->stop == false)
    {
        std::cout << "[" << this->id << "] Lock Work !" << std::endl;
        this->lock->lock();
        /* Citim o tranzactie */
        Tranzaction* tr = this->_read();

        /* Raspundem la tranzactie */
        this->_query(tr);

        /* Trimitem informatiile serializate */
        this->_send(tr);

        /* Eliberam memoria */
        delete tr;

        std::cout << "[" << this->id << "] Unlock Work !" << std::endl;
        this->lock->unlock();
    }
}

Tranzaction* Worker::_read()
{
    /* Citim o noua tranzactie */
    Tranzaction *tr = NULL;
    try
    {
         tr = this->rd->read();
    }
    catch (BaseException* ex)
    {
        std::cout << "[" << this->id << "] Eroare la citire !" << std::endl;
    }


    return tr;
}

void Worker::_query(Tranzaction* tr)
{
    /* Pregateste raspunsurile
     *
     * @param[in, out] tr
     *  Tranzactia care are intrebarile, la iesire
     *  va fi populata cu raspunsuri
     */

    std::vector<Question> aux = tr->get_questions();
    for (std::vector<Question>::iterator it = aux.begin();
         it != aux.end(); ++it)
    {
        char *domain;
        unsigned short len;
        (*it).get_name(&domain, len);
        std::string ip = this->db->get_ip(domain, len);


        if (ip != std::string(""))
        {
            /* Daca am gasit un ip */
            Resource res;
            res.set_name(domain, len);

            char generic[2]; /* generic 00 01 */
            generic[0] = 0;
            generic[1] = 1;

            res.set_type(generic);
            res.set_class(generic);

            char generic_ttl[4]; /* generic 00 00 00 30 */
            generic_ttl[0] = 0;
            generic_ttl[1] = 0;
            generic_ttl[2] = 0;
            generic_ttl[3] = 30;

            res.set_ttl(generic_ttl);
            
            in_addr_t ip_addr = inet_addr(ip.c_str());
            /* NOTE(mmicu): 4 e hardcodat, desi asta o sa fie dimensiuena unui IPv4
             * mereu ar trebui sa folosim sizeof */

            std::cout << "ip_addr" << ip_addr << std::endl;
            unsigned short len = 4;
            len = htons(len);
            res.set_data((char*)&ip_addr, len);

            tr->add_answer(res);
        }

        delete domain;
    }

    tr->set_flag_response();
    if (tr->get_ancount_short() == 0)
    {
        /* Nu avem nici un raspuns */
        tr->set_flag_notfound();
    }
}

void Worker::_send(Tranzaction* tr)
{
    /* Serializeaza tranzactia si trimite raspunsul */
    tr->print_info();

    char* data;
    unsigned short data_len;
    tr->serialize(&data, data_len);

    std::cout << "Serializare :";
    tr->serialize_hex();

    struct sockaddr client = tr->get_client();


    try
    {
        this->rd->get_server()->send(data, data_len, 0, (struct sockaddr*)&client, sizeof(client));
    }
    catch (BaseException* ex)
    {
        std::cout << "[" << this->id << "] Trimitere - eroare" << std::endl;
    }


}

WorkerPool::WorkerPool(Parser* pr)
{
    /* Pregatim Pool-ul de workers
     *
     * @param pr
     * Un obiect de tip parser
     */
    std::cout << "Pornim WorkerPool" << std::endl;
    this->pr = pr;

    this->server = new Server((*this->pr)["port"]->get_int(),
                              (*this->pr)["backlog"]->get_int());
    this->rd = new Reader();
    this->rd->set_server(this->server);

    this->server->start();


    std::cout << "Generam workes  (" <<
                 (*this->pr)["min_threads"]->get_int() <<
                 "):" << std::endl;
    for (int i = 0; i<(*this->pr)["min_threads"]->get_int(); ++i)
    {
        /* Generam numarul de workers */
        std::cout <<" Suntem la " << i << std::endl;
        this->workers.push_back(
            new Worker((char*)(*this->pr)["db_name"]->get_string().c_str(),
                &(this->lock), this->rd, this->pr, i));
    }
}

void WorkerPool::start()
{
    /* Pornim Pool-ul */
    std::cout << "Generam threads :" << std::endl;
    /* Spawnam threadurile */
    for (std::vector<Worker*>::iterator it = this->workers.begin();
        it < this->workers.end(); ++it)
    {
        this->threads.push_back(
            new std::thread(run, (*it)));
    }

    this->finish = false;

    std::cout << "Start !" << std::endl;
    while (this->finish == false)
    {
        /* Astepata */
    }
}

void WorkerPool::close()
{
    std::cout << "Inchide " << std::endl;
    this->finish = true;
    /* Trimitem semnalul de oprire */
    for (std::vector<Worker*>::iterator it = this->workers.begin();
        it < this->workers.end(); ++it)
    {
        (*it)->sign_stop();
    }

    /* Asteptam dupa fiecare thread */
    for (std::vector<std::thread*>::iterator it = this->threads.begin();
        it < this->threads.end(); ++it)
    {
        (*it)->join();
    }


    /* Inchidem servarul */
    this->lock.lock();
    this->server->stop();
    this->lock.unlock();
}
