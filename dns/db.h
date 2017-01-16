/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#ifndef DB_H
#define DB_H value
#include <mutex>
#include <sqlite3.h>
#include <string>

class DB
{
    /* Clasa care permite conexiunea la baza de date */
    private:
        char *filename,
              *errMsg,
              *_ip;

        sqlite3 *db;
        /* In cazul in care dorim sa partajam conexiunea */
        std::mutex lock;
        void _prepare();
        bool _is_prepare();
        void _insert(std::string domain, std::string ip);
    public:
        static unsigned short IP_MAX_SIZE;
        DB(char* filename);
        std::string get_ip(char* name, unsigned short name_len);
        ~DB();
};
#endif /* ifndef DB_H */
