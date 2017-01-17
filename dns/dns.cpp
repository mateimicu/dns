/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <string>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

#include <bitset>
#include <iostream>
#include "dns.h"

void print_hex(char *ch, int len, bool endl)
{
    /* Afiseaxa caracter cu caracter in hexa */
    for (int i = 0; i < len; ++i)
    {
        std::cout << std::hex << (int)ch[i] << " ";
    }

    if (endl == true)
    {
        std::cout << std::endl;
    }
}

void concat(char** data, unsigned short&len, char* part1,
            unsigned short len_1, char* part2, unsigned short len_2)
{
    /* Concataneaza 2 char* in unul nou */
    char *data_aux= new char[len_1 + len_2];
    memcpy(data_aux, part1, len_1);
    memcpy((data_aux+len_1), part2, len_2);

    *data = data_aux;
    len = len_1 + len_2;
}

void print_char(char *ch, int len, bool endl)
{
    /* Afiseaxa caracter cu caracter */
    for (int i = 0; i < len; ++i)
    {
        std::cout << ch[i] << " ";
    }

    if (endl == true)
    {
        std::cout << std::endl;
    }
}

void print_int(char *ch, int len, bool endl)
{
    /* Afiseaxa caracter cu caracter in int*/
    for (int i = 0; i < len; ++i)
    {
        std::cout << (int)ch[i] << " ";
    }

    if (endl == true)
    {
        std::cout << std::endl;
    }
}

Question::Question()
{
    /* Initializeaza o unsigned shortrebare */
    this->qname = NULL;
    
    bzero(this->qtype, 2);
    bzero(this->qclass, 2);
    
    this->qname_len = 0;
}

void Question::set_name(char *name, unsigned short length)
{
    /* Seteaza un nume pentru unsigned shortrebare
     *
     * @param[in] *name
     *  Numele intrebari
     *
     * @param[in] lenght
     *  Lungimea stringului
     * */

     this->qname_len = length;

     this->qname = new char[length];
     memcpy(this->qname, name, length);
}

void Question::set_type(char type[2])
{
    /* Seteaza tipul unei intrebari
     *
     * @param[in] type[2]
     *  Tipul intrebari.
     */
     memcpy(this->qtype, type, 2);
}

void Question::set_class(char cls[2])
{
    /* Seteaza clasa unei intrebari
     *
     * @param[in] cls[2]
     *  Clasa intrebari.
     */

     memcpy(this->qclass, cls, 2);
}

void Question::get_name(char **name, unsigned short& length)
{
    /* Returneaza numele unsigned shortrebari
     *
     * @param[out] *name
     *  Numele intrebari
     *
     * @param[out] lenght
     *  Lungimea stringului
     * */
     if (this->qname_len == 0)
     {
         *name = NULL;
         length = this->qname_len;
     }
     else
     {
        char *rname = new char[this->qname_len];
        memcpy(rname, this->qname, this->qname_len);
        *name = rname;
        length = this->qname_len;
     }
}

void Question::get_type(char type[2])
{
    /* Returneaza tipul unei unsigned shortrebari
     *
     * @param[out] type[2]
     *  Tipul unsigned shortrebari.
     */

     memcpy(type, this->qtype, 2);
}

void Question::get_class(char cls[2])
{
     /* Returneaza clasa unei unsigned shortrebari
     *
     * @param[out] cls[2]
     *  Clasa unsigned shortrebari.
     */

     memcpy(cls, this->qclass, 2);
}

void Question::print_info()
{
    /* Printeaza informatii despre o intrebare */
    std::cout <<"  Name :";
    print_hex(this->qname, this->qname_len, true);
    std::cout <<"  Name :";
    print_int(this->qname, this->qname_len, true);
    std::cout <<"  Name :";
    print_char(this->qname, this->qname_len, true);
    std::cout <<"  Lungime :" << (int)this->qname_len << std::endl;

    std::cout <<" Type: ";
    print_hex(this->qtype, 2, true);

    std::cout <<" Class: ";
    print_hex(this->qclass, 2, true);
    std::cout << std::endl;
}

void Question::serialize(char** data, unsigned short& len)
{
    /* Serializeaza obiectul curent */
    char aux[this->qname_len];
    memset(aux, 0, this->qname_len);
    memcpy(aux, this->qname, this->qname_len);
    concat(data, len, aux, (this->qname_len), NULL, 0);

    char aux_2[2];
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->qtype, 2);
    concat(data, len, *data, len, aux_2, 2);

    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->qclass, 2);
    concat(data, len, *data, len, aux_2, 2);
}

void Question::serialize_hex()
{
    /* Afiseaza la stdout hexa serializari obiectului curent */
    char* data;
    unsigned short len;
    this->serialize(&data, len);
    print_hex(data, len, true);
}

Resource::Resource()
{
    /* Initializarea unei resurse */

    bzero(this->type, 2);
    bzero(this->cls, 2);
    bzero(this->rdlength, 2);
    this->rdata = NULL;
    this->name = NULL;
    this->name_len = 0;
}

void Resource::set_name(char *name, unsigned short length)
{
    /* Seteaza un nume pentru o resursa
     *
     * @param[in] *name
     *  Numele resursei
     *
     * @param[in] lenght
     *  Lungimea stringului
     * */
     this->name_len = length;

     this->name = new char[length];
     memcpy(this->name, name, length);
}

void Resource::set_type(char type[2])
{
    /* Seteaza tipul unei resurse
     *
     * @param[in] type[2]
     *  Tipul resurse.
     */

     memcpy(this->type, type, 2);
}

void Resource::set_class(char cls[2])
{
    /* Seteaza clasa unei resurse
     *
     * @param[in] cls[2]
     *  Clasa resurseri.
     */

     memcpy(this->cls, cls, 2);

}

void Resource::set_ttl(char ttl[4])
{
    /* Seteaza time to live unei resurse
     *
     * @param[in] ttl[4]
     *  time to live pentru  resurseri.
     */

     memcpy(this->ttl, ttl, 4);
}

void Resource::set_data(char* data, unsigned short length_data)
{
    /* Setarea unor informatii despre resursa
     *
     * @param[in] data
     *  Informatia propriuzisa
     *
     * @param[in] length_data
     *  Lungimea informatiei
     */

    this->rdata = new char[length_data];
    memcpy(this->rdata, data, length_data);
    memcpy(this->rdlength, (char*)&length_data, 2);
}

void Resource::get_name(char **name, unsigned short& length)
{
    /* Returneaza numele Resursei
     *
     * @param[out] *name
     *  Numele Resursei
     *
     * @param[out] lenght
     *  Lungimea stringului
     */
    if (this->name_len == 0)
    {
        *name = NULL;
        length = this->name_len;
    }
    else
    {
        *name = new char[this->name_len];
        memcpy(*name, this->name, this->name_len);
        length = this->name_len;
    }
}

void Resource::get_type(char type[2])
{
    /* Returneaza tipul unei resurse
     *
     * @param[out] type[2]
     *  Tipul resursei.
     */

     memcpy(type, this->type, 2);
}

void Resource::get_class(char cls[2])
{
     /* Returneaza clasa unei resurse
     *
     * @param[out] cls[2]
     *  Clasa resursei.
     */

     memcpy(cls, this->cls, 2);
}

void Resource::get_ttl(char ttl[4])
{
     /* Returneaza time tp live pentru o resurse
     *
     * @param[out] ttl[4]
     *  Time to live.
     */

     memcpy(ttl, this->ttl, 4);
}

void Resource::get_data(char** data, unsigned short& length_data)
{
    /* Setarea unor informatii despre resursa
     *
     * @param[out] data
     *  Informatia propriuzisa
     *
     * @param[out] length_data
     *  Lungimea informatiei
     */
    length_data = this->rdlength[0] << 8 | this->rdlength[1];
    if (length_data == 0)
    {
        *data = NULL;
    }
    else
    {
        *data = new char[length_data];
        memcpy(*data, this->rdata, length_data);
    }
}

void Resource::print_info()
{
    /* Printeaza informatii despre resurs */
    std::cout << "  Name :";
    print_hex(this->name, this->name_len, true);
    std::cout << "  Name :";
    print_int(this->name, this->name_len, true);
    std::cout << "  Name :";
    print_char(this->name, this->name_len, true);
    std::cout <<" Len. Name: " << this->name_len << std::endl;

    std::cout << "  Type:";
    print_hex(this->type, 2, true);

    std::cout << "  Class:";
    print_hex(this->cls, 2, true);

    std::cout << "  TTL:";
    print_hex(this->ttl, 4, true);

    unsigned short len;
    memcpy(&len, this->rdlength, 2);
    len = ntohs(len);

    std::cout << " Data:";
    print_hex(this->rdata, len, true);

    std::cout << " Data:";
    print_int(this->rdata, len, true);

    std::cout << " Data:";
    print_char(this->rdata, len, true);

    std::cout << "  Len. Data:" << len << std::endl << std::endl;
}

void Resource::serialize(char** data, unsigned short& data_len)
{
    /* Serializeaza obiectul curent */
    char aux[this->name_len];
    memset(aux, 0, this->name_len);
    memcpy(aux, this->name, this->name_len);
    concat(data, data_len, aux, this->name_len, NULL, 0);

    char aux_2[2];
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->type, 2);
    concat(data, data_len, *data, data_len, aux_2, 2);

    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->cls, 2);
    concat(data, data_len, *data, data_len, aux_2, 2);

    char aux_4[4];
    bzero(aux_4, sizeof(aux_4));
    memcpy(aux_4, this->ttl, 4);
    concat(data, data_len, *data, data_len, aux_4, 4);

    /* rdlen */
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->rdlength, 2);
    concat(data, data_len, *data, data_len, aux_2, 2);

    /* rdata */
    unsigned short len;
    memcpy(&len, this->rdlength, 2);
    len = ntohs(len);

    char aux_data[len+1];
    memset(aux_data, 0, len+1);
    memcpy(aux_data, this->rdata, len);
    concat(data, data_len, *data, data_len, aux_data, len);
}

void Resource::serialize_hex()
{
    /* Afiseaza la stdout hexa serializari obiectului curent */
    char* data;
    unsigned short len;
    this->serialize(&data, len);
    print_hex(data, len, true);
}

Tranzaction::Tranzaction()
{
    /* Initializeaza o tranzactie */
    bzero(this->id, 2);
    bzero(this->flags, 2);
    bzero(this->qcount, 2);
    bzero(this->ancount, 2);
    bzero(this->nscount, 2);
    bzero(this->arcount, 2);
    this->questions.clear();
    this->answers.clear();
    this->authority.clear();
    this->additional_sections.clear();
}

unsigned short Tranzaction::_get_short_from_char(char ch[2])
{
    /*Transforma un char[2] intr-un unsigned short
     *
     * @param[in] ch
     *  Charul care trebuie transformat
     *
     * \return{Valoare in short}
     */
    unsigned short out = 0;
    out = (short)(
           ((unsigned char)ch[0]) |
           ((unsigned char)ch[1]) << 8
          );
    return out;
}

void Tranzaction::set_id(char id[2])
{
    /* Seteaza id-ul unei tranzactii
     *
     * @param[in] id
     *  Id-ul tranzactiei
     */
    memcpy(this->id, id, 2);
}

void Tranzaction::set_flags(char flags[2])
{
    /* Seteaza flagurile unei tranzactii
     *
     * @param[in] flags
     *  Flagurile tranzactiei
     * */
    memcpy(this->flags, flags, 2);
}

void Tranzaction::add_question(Question qt)
{
    /* Adauga un query (intrebare) tranzactiei
     *
     * @param qt
     *  Intrebarea, o instanta a clasei `Question`
     */
    this->questions.push_back(qt);

    /* Updateaza numarul de intrebari */

    /* NOTE(mmicu): Acest lucru ar trebui eliminat, putem avea doar un getter
     * peste `qcount` care apeleaza `.length` pe `this->questions`
     */
    unsigned short size = this->questions.size();
    memcpy(this->qcount, (char*)&(size), 2);
}

void Tranzaction::add_answer(Resource ans)
{
    /* Adauga un raspuns
     *
     * @param aut
     *  Raspuns, o instanta a clasei `Resource`
     */

    this->answers.push_back(ans);

    /* Updateaza numarul de raspunsuri */
    unsigned short size = this->answers.size();
    memcpy(this->ancount, (char*)&(size), 2);
}

void Tranzaction::add_authority(Resource aut)
{
    /* Adauga o autoritate tranzactiei
     *
     * @param aut
     *  Autoritatea, o instanta a clasei `Resource`
     */

    this->authority.push_back(aut);

    /* Updateaza numarul de autoritati */
    unsigned short size = this->authority.size();
    memcpy(this->nscount, (char*)&(size), 2);
}

void Tranzaction::add_additiona_section(Resource res)
{
    /* Adauga resurse aditionale
     *
     * @param res
     *  Resursa , o instanta a clasei `Resource`
     */

    this->additional_sections.push_back(res);

    /* Updateaza numarul de autoritati */
    unsigned short size = this->additional_sections.size();
    memcpy(this->arcount, (char*)&(size), 2);
}

void Tranzaction::get_id(char id[2])
{
    /* Returneaza id-ul unei tranzactii
     *
     * @param[out] id
     *  Id-ul tranzactiei
     */
    memcpy(id, this->id, 2);
}

void Tranzaction::get_flags(char flags[2])
{
    /* Returneaza flagurile unei tranzactii
     *
     * @param[out] flags
     *  Flagurile tranzactiei
     */
    memcpy(flags, this->flags, 2);
}

void Tranzaction::get_qcount_char(char r_qcount[2])
{
    /* Returneza numarul de query(intrebari)
     * sub forma unui char[2]
     *
     * @param r_qcount
     *  Numarul de query
     */
    memcpy(r_qcount, this->qcount, 2);

}

unsigned short Tranzaction::get_qcount_short()
{
    /* Returneza numarul de query(intrebari)
     * sub forma unui unsigned short
     */
    unsigned short count = this->_get_short_from_char(this->qcount);
    return count;
}

void Tranzaction::get_ancount_char(char r_ancount[2])
{
    /* Returneza numarul de raspunsuri
     * sub forma unui char[2]
     *
     * @param r_ancount
     *  Numarul de raspunsuri
     */
    memcpy(r_ancount, this->ancount, 2);
}

unsigned short Tranzaction::get_ancount_short()
{
    /* Returneza numarul de raspunsuri
     * sub forma unui unsigned short
     */
    unsigned short count = this->_get_short_from_char(this->ancount);
    return count;
}


void Tranzaction::get_nscount_char(char r_nscount[2])
{
    /* Returneza numarul de name server authority
     * sub forma unui char[2]
     *
     * @param r_nscount
     *  Numarul de name server authority
     */
    memcpy(r_nscount, this->nscount, 2);
}

unsigned short Tranzaction::get_nscount_short()
{
    /* Returneza numarul de name server authority
     * sub forma unui unsigned short
     */
    unsigned short count = this->_get_short_from_char(this->nscount);
    return count;
}

void Tranzaction::get_arcount_char(char r_arcount[2])
{
    /* Returneza numarul de additional records
     * sub forma unui char[2]
     *
     * @param r_arcount
     *  Numarul de additional records
     */
    memcpy(r_arcount, this->nscount, 2);
}

unsigned short Tranzaction::get_arcount_short()
{
    /* Returneza numarul de name server authority
     * sub forma unui unsigned short
     */
    unsigned short count = this->_get_short_from_char(this->arcount);
    return count;
}


std::vector<Question> Tranzaction::get_questions()
{
    /* Returneaza o lista cu toate intrebarile */
    return this->questions;
}

std::vector<Resource> Tranzaction::get_answers()
{
    /* Returneaza o lista cu toate raspunsurile */
    return this->answers;
}

std::vector<Resource> Tranzaction::get_authority()
{
    /* Returneaza o lista cu toate autoritatile */
    return this->authority;
}

std::vector<Resource> Tranzaction::get_additional_sections()
{
    /* Returneaza o lista cu toate resursele aditionale */
    return this->additional_sections;
}

void Tranzaction::set_client(sockaddr client)
{
    /* Setam un client pentru tranzactia asta
     *
     * @param[in] client
     *  Clientul pe care dorim sa il setam
     */
    this->client = client;
}

sockaddr Tranzaction::get_client()
{
    /* Returnam clientul */
    return this->client;
}

void Tranzaction::set_flag_response()
{
    /* Setam flagul pentru aceasta tranzactie sa fie un response */
    this->flags[0] |= 128;
}

void Tranzaction::set_flag_notfound()
{
    /* Nu am gasit numele */
    this->flags[1] |= 1;
    this->flags[1] |= 2;
}

void Tranzaction::print_info()
{
    /* Printeaza infomatii despre tranzactie */
    std::cout << " ------------------ " << std::endl;
    std::cout << "Tranzactie id :";
    print_hex(this->id, 2, true);

    std::cout << "Flags :";
    print_hex(this->flags, 2, true);

    std::cout << "Qcount :" << this->get_qcount_short() << std::endl;
    std::cout << "Ancount:" << this->get_ancount_short() << std::endl;
    std::cout << "Nscount:" << this->get_nscount_short() << std::endl;
    std::cout << "Arcount :" << this->get_arcount_short() << std::endl;

    std::cout << "Questions :" << std::endl;
    for (std::vector<Question>::iterator it = this->questions.begin();
         it != this->questions.end(); ++it)
    {
        (*it).print_info();
    }

    std::cout << "Answers:" << std::endl;
    for (std::vector<Resource>::iterator it = this->answers.begin();
         it != this->answers.end(); ++it)
    {
        (*it).print_info();
    }

    std::cout << "Authority:" << std::endl;
    for (std::vector<Resource>::iterator it = this->authority.begin();
         it != this->authority.end(); ++it)
    {
        (*it).print_info();
    }

    std::cout << "Additional sections:" << std::endl;
    for (std::vector<Resource>::iterator it = this->additional_sections.begin();
         it != this->additional_sections.end(); ++it)
    {
        (*it).print_info();
    }

    std::cout << " ------------------ " << std::endl << std::endl;
}

void Tranzaction::serialize(char** data, unsigned short& len)
{
    /* Serializeaza obiectul curent
     *
     * @param[out] data
     * pointer catre array
     *
     * @param[out] len
     * lungimea
     */

    /* id */
    char aux_2[3];
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->id, 2);
    concat(data, len, aux_2, 2, NULL, 0);

    /* flags */
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, this->flags, 2);
    concat(data, len, *data, len, aux_2, 2);

    unsigned short s = htons(this->get_qcount_short());
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, (char*)&s, 2);
    concat(data, len, *data, len, aux_2, 2);


    s = htons(this->get_ancount_short());
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, (char*)&s, 2);
    concat(data, len, *data, len, aux_2, 2);

    s = htons(this->get_nscount_short());
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, (char*)&s, 2);
    concat(data, len, *data, len, aux_2, 2);

    s = htons(this->get_arcount_short());
    bzero(aux_2, sizeof(aux_2));
    memcpy(aux_2, (char*)&s, 2);
    concat(data, len, *data, len, aux_2, 2);

    char *aux_data;
    unsigned short aux_data_len;

    for (std::vector<Question>::iterator it = this->questions.begin();
         it != this->questions.end(); ++it)
    {
        (*it).serialize(&aux_data, aux_data_len);
        concat(data, len, *data, len, aux_data, aux_data_len);
    }

    for (std::vector<Resource>::iterator it = this->answers.begin();
         it != this->answers.end(); ++it)
    {
        (*it).serialize(&aux_data, aux_data_len);
        concat(data, len, *data, len, aux_data, aux_data_len);
    }

    for (std::vector<Resource>::iterator it = this->authority.begin();
         it != this->authority.end(); ++it)
    {
        (*it).serialize(&aux_data, aux_data_len);
        concat(data, len, *data, len, aux_data, aux_data_len);
    }

    for (std::vector<Resource>::iterator it = this->additional_sections.begin();
         it != this->additional_sections.end(); ++it)
    {
        (*it).serialize(&aux_data, aux_data_len);
        concat(data, len, *data, len, aux_data, aux_data_len);
    }
}

void Tranzaction::serialize_hex()
{
    /* Afiseaza la stdout hexa serializari obiectului curent */
    char* data;
    unsigned short len;
    this->serialize(&data, len);
    print_hex(data, len, true);
}
