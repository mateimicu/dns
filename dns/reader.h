/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <vector>

#include "server.h"
#include "dns.h"

#ifndef READER_H_
#define READER_H_ value
class Reader
{
    /* Reader class
     *
     * Citeste dintr-un server informatia si returneaza
     * un obiect de tip tranzactie
     */
    private:
        Server* server;
        char* buff;
        static int BUFF_SIZE;

        std::vector<Resource> parser_resource(
            char* buff, unsigned short buff_len,
            unsigned short buff_index, unsigned short expected,
            unsigned short& out_buff_index);

        std::vector<Question> parser_question(
            char* buff, unsigned short buff_len,
            unsigned short buff_index, unsigned short expected,
            unsigned short& out_buff_index);
    public:
        Reader();
        void set_server(Server* s);
        Tranzaction* read();
        ~Reader();
};
#endif /* ifndef READER_H_ */
