/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#ifndef DNS_H
#define DNS_H value
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Question
{
    /* Question class
     *
     * Formatul unei unsigned shortrebari:
     * - nume
     * - tip
     * - clasa
     * - lungimea numelui
     * */
    private:
        char *qname, qtype[2], qclass[2];
        unsigned short qname_len;
    public:
        Question();
        void set_name(char* name, unsigned short length);
        void set_type(char type[2]);
        void set_class(char qclass[2]);

        void get_name(char **name, unsigned short& length);
        void get_type(char* type);
        void get_class(char* qclass);

        void print_info();

        void serialize(char** data, unsigned short& len);
        void serialize_hex();
};

class Resource
{
    /* Resouce class
     *
     * Raspunsurile, autoritatile si resursele aditionale au acelasi
     * format:
     *  - nume
     *  - tip
     *  - clasa
     *  - ttl
     *  - lungimea datelor
     *  - datele
     * */
    private:
        char *name, type[2], cls[2], rdlength[2], ttl[4],
             *rdata;
        unsigned short name_len;
    public:
        Resource();
        void set_name(char* name, unsigned short length);
        void set_type(char type[2]);
        void set_class(char qclass[2]);
        void set_ttl(char ttl[4]);
        void set_data(char* data, unsigned short length);

        void get_name(char** name, unsigned short& length);
        void get_type(char* type);
        void get_class(char* qclass);
        void get_ttl(char* ttl);
        void get_data(char** data, unsigned short& length);

        void print_info();

        void serialize(char** data, unsigned short& len);

        void serialize_hex();
};

class Tranzaction
{
    /* O clasa pentru fiecare tranzactie.
     *
     * O tranzatie are:
     * - id
     * - flags
     * - question_count
     * - autoritity count
     * - numarul de resource records
     * - numarul de resurse aditionale
     *
     * NOTE(mmicu): Ai grija cand serializezi informatiile.
     * */
    private:
        char id[2], flags[2], qcount[2],
             ancount[2], nscount[2], arcount[2];
        std::vector<Question> questions;
        std::vector<Resource> answers, authority, additional_sections;
        unsigned short _get_short_from_char(char ch[2]);
        struct sockaddr client;

    public:
        Tranzaction();
        void set_id(char id[2]);
        void set_flags(char flags[2]);

        void add_question(Question qt);
        void add_answer(Resource ans);
        void add_authority(Resource aut);
        void add_additiona_section(Resource res);

        void get_id(char id[2]);
        void get_flags(char flags[2]);

        void get_qcount_char(char qcount[2]);
        unsigned short get_qcount_short();

        void get_ancount_char(char ancount[2]);
        unsigned short get_ancount_short();

        void get_nscount_char(char nscount[2]);
        unsigned short get_nscount_short();

        void get_arcount_char(char arcount[2]);
        unsigned short get_arcount_short();

        void set_client(sockaddr client);
        sockaddr get_client();

        void set_flag_response();
        void set_flag_notfound();

        void print_info();

        std::vector<Question> get_questions();
        std::vector<Resource> get_answers();
        std::vector<Resource> get_authority();
        std::vector<Resource> get_additional_sections();

        void serialize(char** data, unsigned short& len);

        void serialize_hex();
};
#endif /* ifndef DNS_H */
