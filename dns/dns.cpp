/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

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
     strncpy(this->qname, name, length);
}

void Question::set_type(char type[2])
{
    /* Seteaza tipul unei intrebari
     *
     * @param[in] type[2]
     *  Tipul intrebari.
     */

     strncpy(this->qtype, type, 2);
}

void Question::set_class(char cls[2])
{
    /* Seteaza clasa unei intrebari
     *
     * @param[in] cls[2]
     *  Clasa intrebari.
     */

     strncpy(this->qclass, cls, 2);
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
        strncpy(rname, this->qname, this->qname_len);
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

     strncpy(type, this->qtype, 2);
}

void Question::get_class(char cls[2])
{
     /* Returneaza clasa unei unsigned shortrebari
     *
     * @param[out] cls[2]
     *  Clasa unsigned shortrebari.
     */

     strncpy(cls, this->qclass, 2);
}

void Question::print_info()
{
    /* Printeaza informatii despre o intrebare */
    std::cout <<"  Name :";
    print_hex(this->qname, this->qname_len, true);
    std::cout <<"  Lungime :" << this->qname_len << std::endl;

    std::cout <<" Type: ";
    print_hex(this->qtype, 2, true);

    std::cout <<" Class: ";
    print_hex(this->qclass, 2, true);
    std::cout << std::endl;
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
     strncpy(this->name, name, length);
}

void Resource::set_type(char type[2])
{
    /* Seteaza tipul unei resurse
     *
     * @param[in] type[2]
     *  Tipul resurse.
     */

     strncpy(this->type, type, 2);
}

void Resource::set_class(char cls[2])
{
    /* Seteaza clasa unei resurse
     *
     * @param[in] cls[2]
     *  Clasa resurseri.
     */

     strncpy(this->cls, cls, 2);
}

void Resource::set_ttl(char ttl[2])
{
    /* Seteaza time to live unei resurse
     *
     * @param[in] ttl[2]
     *  time to live pentru  resurseri.
     */

     strncpy(this->ttl, ttl, 2);
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
    strncpy(this->rdata, data, length_data);
    strncpy(this->rdlength, (char*)&length_data, 2);
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
        strncpy(*name, this->name, this->name_len);
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

     strncpy(type, this->type, 2);
}

void Resource::get_class(char cls[2])
{
     /* Returneaza clasa unei resurse
     *
     * @param[out] cls[2]
     *  Clasa resursei.
     */

     strncpy(cls, this->cls, 2);
}

void Resource::get_ttl(char ttl[2])
{
     /* Returneaza time tp live pentru o resurse
     *
     * @param[out] ttl[2]
     *  Time to live.
     */

     strncpy(ttl, this->ttl, 2);
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
        strncpy(*data, this->rdata, length_data);
    }
}

void Resource::print_info()
{
    /* Printeaza informatii despre resurs */
    std::cout << "  Name :";
    print_hex(this->name, this->name_len, true);
    std::cout <<" Len. Name: " << this->name_len << std::endl;

    std::cout << "  Type:";
    print_hex(this->type, 2, true);

    std::cout << "  Class:";
    print_hex(this->cls, 2, true);

    std::cout << "  TTL:";
    print_hex(this->ttl, 2, true);

    std::cout << " Data:";
    unsigned short len;
    memcpy(&len, this->rdlength, 2);
    len = ntohs(len);
    print_hex(this->rdata, len, true);

    std::cout << "  Len. Data:" << len << std::endl << std::endl;
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
    strncpy(this->id, id, 2);
}

void Tranzaction::set_flags(char flags[2])
{
    /* Seteaza flagurile unei tranzactii
     *
     * @param[in] flags
     *  Flagurile tranzactiei
     * */
    strncpy(this->flags, flags, 2);
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
    strncpy(this->qcount, (char*)&(size), 2);
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
    strncpy(this->ancount, (char*)&(size), 2);
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
    strncpy(this->nscount, (char*)&(size), 2);
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
    strncpy(this->arcount, (char*)&(size), 2);
}

void Tranzaction::get_id(char id[2])
{
    /* Returneaza id-ul unei tranzactii
     *
     * @param[out] id
     *  Id-ul tranzactiei
     */
    strncpy(id, this->id, 2);
}

void Tranzaction::get_flags(char flags[2])
{
    /* Returneaza flagurile unei tranzactii
     *
     * @param[out] flags
     *  Flagurile tranzactiei
     */
    strncpy(flags, this->flags, 2);
}

void Tranzaction::get_qcount_char(char r_qcount[2])
{
    /* Returneza numarul de query(intrebari)
     * sub forma unui char[2]
     *
     * @param r_qcount
     *  Numarul de query
     */
    strncpy(r_qcount, this->qcount, 2);

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
    strncpy(r_ancount, this->ancount, 2);
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
    strncpy(r_nscount, this->nscount, 2);
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
    strncpy(r_arcount, this->nscount, 2);
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
