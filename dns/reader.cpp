/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <string.h>
#include <iostream>
#include <arpa/inet.h>


#include "reader.h"
#include "server.h"
#include "dns.h"
#include "exceptions.h"


/* Dimensiunea maxima pentru un pachet UDP */
int Reader::BUFF_SIZE = 512;

Reader::Reader()
{
    /* Initializam un reader */
    this->server = NULL;
    this->buff = (char*)malloc(this->BUFF_SIZE);
}

Reader::~Reader()
{
    /* Initializam un reader */
    if (this->buff != NULL)
    {
        free(this->buff);
    }
}

void Reader::set_server(Server* s)
{
    /* Seteaza un server
     *
     * @param[in] s
     *  Un obiect de tip Server
     */

    if (s == NULL)
    {
        throw ReaderValueError();
    }
    this->server = s;
}

std::vector<Question> Reader::parser_question(
    char* buff, unsigned short buff_len, unsigned short buff_index,
    unsigned short expected, unsigned short& out_buff_index)
{
    /* Parsam din buffer toate intrebarile
     *
     * @param[in] buff
     *  Bufferul din care citim
     *
     * @param[in] buff_len
     *  Lungimea bufferului
     *
     * @param[in] buff_index
     *  Indexul de la care citim din buffer
     *
     * @param[in] expected
     *  Cate resrurse speram sa gasim
     *
     * @param[out] out_buff_index
     *  Pana unde am citit in buffer
     */
    std::vector<Question> vect;

    for (int i = 0; i<expected; ++i)
    {
        Question qe;
        int len = 0;
        for (int index = buff_index; index < buff_len; ++index)
        {
            len++;
            if (buff[index] == 0)
            {
                break;
            }
        }
        if (buff_index >= buff_len - 4)
        {
            continue;
        }

        qe.set_name(&buff[buff_index], len);
        buff_index = buff_index + len;

        qe.set_type(&buff[buff_index]);
        buff_index = buff_index + 2;

        qe.set_class(&buff[buff_index]);
        buff_index = buff_index + 2;

        vect.push_back(qe);
    }
    out_buff_index = buff_index;
    return vect;
}

std::vector<Resource> Reader::parser_resource(
    char* buff, unsigned short buff_len,
    unsigned short buff_index, unsigned short expected,
    unsigned short& out_buff_index)
{
    /* Parsam din buffer toate intrebarile
     *
     * @param[in] buff
     *  Bufferul din care citim
     *
     * @param[in] buff_len
     *  Lungimea bufferului
     *
     * @param[in] buff_index
     *  Indexul de la care citim din buffer
     *
     * @param[in] expected
     *  Cate resrurse speram sa gasim
     *
     * @param[out] out_buff_index
     *  Pana unde am citit in buffer
     */
    std::vector<Resource> vect;
    for (int i = 0; i<expected; ++i)
    {
        Resource res;
        int len = 0;
        for (int index = buff_index; index < buff_len; ++index)
        {
            len++;
            if (buff[index] == 0)
            {
                break;
            }
        }
        if (buff_index >= buff_len- 4*2)
        {
            continue;
        }

        res.set_name(&buff[buff_index], len);
        buff_index = buff_index + len;

        res.set_type(&buff[buff_index]);
        buff_index = buff_index + 2;

        res.set_class(&buff[buff_index]);
        buff_index = buff_index + 2;

        res.set_class(&buff[buff_index]);
        buff_index = buff_index + 2;

        res.set_ttl(&buff[buff_index]);
        buff_index = buff_index + 2;

        unsigned short rdlen;
        memcpy(&rdlen, &buff[buff_index], 2);
        rdlen = ntohs(rdlen);
        buff_index = buff_index + 2;

        res.set_data(&buff[buff_index], rdlen);
        buff_index = buff_index + rdlen;

        vect.push_back(res);
    }
    out_buff_index = buff_index;
    return vect;
}

Tranzaction* Reader::read()
{
    Tranzaction* tr = NULL;
    /* Citim pana cand dam de o tranzactie buna */
    bool found = false;
    while (found == false)
    {
        if (tr != NULL)
        {
            /* Distrugem tranzactia facuta anterior */
            delete tr;
        }
        tr = new Tranzaction();
        /* Citeste un pachet si returneaza o tranzactie */
        bzero(this->buff, sizeof(this->buff));
        int data_len = 0;

        /* informatii despre client */
        struct sockaddr client;
        /* NOTE(mmicu): len_client e parametru de intrare iesier */
        socklen_t len_client = sizeof(client);
        bzero(&client, sizeof(client));


        try
        {
            data_len = this->server->read(buff, 512, 0, (struct sockaddr*) &client, &len_client);
        }
        catch (ServerReadError& exp)
        {
            std::cout << "Malformed request (Eroare la citire) :"<< std::endl;
            for (int i=0; i<data_len; ++i)
            {
                std::cout << std::hex << (int)buff[i];
            }
            std::cout<< std::endl;

            continue;
        }

        if (data_len < 6 * 2)
        {
            /* Dimensiunea headerului */
            std::cout << "Malformed request (Prea scurt) :"<< std::endl;
            for (int i=0; i<data_len; ++i)
            {
                std::cout << std::hex << (int)buff[i];
            }
            std::cout<< std::endl;
        }
        /* Setam id-ul tranzactiei */
        char buff_2[2];
        memcpy(buff_2, this->buff, 2);
        tr->set_id(buff_2);

        /* Setam flags */
        memcpy(buff_2, &this->buff[2], 2);
        tr->set_flags(buff_2);

        unsigned short qcount=0,
                       ancount=0,
                       nscount=0,
                       arcount=0,
                       short_buff;

        memcpy(&short_buff, &this->buff[4], 2);
        qcount = ntohs(short_buff);

        memcpy(&short_buff, &this->buff[6], 2);
        ancount = ntohs(short_buff);

        memcpy(&short_buff, &this->buff[8], 2);
        nscount = ntohs(short_buff);

        memcpy(&short_buff, &this->buff[10], 2);
        arcount = ntohs(short_buff);

        unsigned short max_index;

        /* parsam intrebarile */
        std::vector<Question> aux = this->parser_question(
            this->buff, data_len, 12, qcount, max_index);

        for (std::vector<Question>::iterator it = aux.begin();
             it != aux.end(); ++it)
        {
            tr->add_question((*it));
        }

        /* Raspunsuri */

        std::vector<Resource> aux_res = this->parser_resource(
            this->buff, data_len, max_index, ancount, max_index);

        for (std::vector<Resource>::iterator it = aux_res.begin();
             it != aux_res.end(); ++it)
        {
            tr->add_answer((*it));
        }
        /* Authority */
        aux_res = this->parser_resource(
            this->buff, data_len, max_index, nscount, max_index);

        for (std::vector<Resource>::iterator it = aux_res.begin();
             it != aux_res.end(); ++it)
        {
            tr->add_authority((*it));
        }

        /* Additional */
        aux_res = this->parser_resource(
            this->buff, data_len, max_index, arcount, max_index);

        for (std::vector<Resource>::iterator it = aux_res.begin();
             it != aux_res.end(); ++it)
        {
            tr->add_additiona_section((*it));
        }


        /* Verificam daca am consumat tot continutul cum trebuie */
        if (qcount  != tr->get_qcount_short()   ||
            ancount != tr->get_ancount_short()  ||
            nscount != tr->get_nscount_short()  ||
            arcount != tr->get_arcount_short())
        {
            std::cout << "Malformed request (Nu am putut citi cate elemente trebuia) :"<< std::endl;
            found = false;
        }

        if (max_index == data_len)
        {
            found = true;
            tr->set_client(client);
        }
        else
        {
            std::cout << "Malformed request (Nu am consumat tot raspunsu) :"<< std::endl;
            found = false;
        }
        tr->print_info();
    } /* end while(found) */
    return tr;
}

Server* Reader::get_server()
{
    /* Returneaza serverul de dns */
    return this->server;
}
