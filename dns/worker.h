/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#ifndef WORKER_H
#define WORKER_H value
#include <string>
#include <string.h>
#include <iostream>
#include <mutex>
#include <thread>

#include "server.h"
#include "db.h"
#include "dns.h"
#include "parser.h"
#include "reader.h"
#include "exceptions.h"

class Worker
{
    private:
        DB* db;
        std::mutex* lock;
        Parser* parser;
        Reader* rd;
        bool stop;
        int id;
    public:
        Worker(char* db_name, std::mutex* lock, Reader* rd, Parser* pr, int id);
        ~Worker();
        Tranzaction* _read();
        void _query(Tranzaction* tr);
        void _send(Tranzaction* tr);
        void work();

        void sign_stop();
};

class WorkerPool
{
    private:
        std::vector<Worker*> workers;
        std::vector<std::thread*> threads;
        std::mutex lock;
        Reader* rd;
        Parser* pr;
        Server* server;
        bool finish;
    public:
        WorkerPool(Parser* pr);
        void close();
        void start();
};
#endif /* ifndef WORKER_H */
