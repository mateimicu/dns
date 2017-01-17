/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <iostream>
#include <csignal>

#include "parser.h"
#include "exceptions.h"
#include "server.h"
#include "reader.h"
#include "db.h"
#include "worker.h"

using namespace std;

/* Vom folosi asta pentru sign handlers */
WorkerPool* MAIN = NULL;


void signalHandler( int signum )
{
    std::cout << "Interrupt signal (" << signum << ") received." << std::endl;

    if (MAIN == NULL)
    {
        std::cout << "Inca nu s-a pornit servarul " << std::endl;
    }
    else
    {
        MAIN->close();
    }
}


Parser prepare_parser()
{
    /* Pregateste parserul cu toate valorile */
    Parser parser;
    StrOption* verbosity = new StrOption('v', "verbose", "Verbosity level(v, vv, vvv).", false);
    verbosity->set_default("v");

    BoolOption* debug = new BoolOption('d', "debug", "Debug mode(default false).", false);
    debug->set_default(false);

    IntOption* port = new IntOption('p', "port", "The port.", false);

    IntOption* min_threads = new IntOption(
        't', "min_threads", "The min. number of threads, or the base threads.", true);

    IntOption* backlog = new IntOption(
        'b', "backlog", "The size of the listen queue ( defaut 10).", false);
    backlog->set_default(10);


    StrOption* db_name = new StrOption('f', "db_name", "Fisierul cu baza de date", true);
    BoolOption* soft = new BoolOption(
        's', "soft", "Daca dorim sa asteptam fiecare thread inainte de terminare.", true);

    parser.add_option(verbosity);
    parser.add_option(debug);
    parser.add_option(port);
    parser.add_option(min_threads);
    parser.add_option(db_name);
    parser.add_option(soft);
    parser.add_option(backlog);
    return parser;
}

int main(int argc, char *argv[])
{
    try
    {
        Parser p = prepare_parser();
        p.parse(argc, argv);

        if (p["soft"]->get_bool() == true)
        {
            /* Inregistreaza handler pentru CTRL+C */
            signal(SIGINT, signalHandler);

            /* Inregistreaza handler pentru pentru gracefully shutdown */
            signal(SIGTERM, signalHandler);
        }


        WorkerPool pool(&p);
        MAIN = &pool;
        pool.start();

    }
    catch (BaseException& ex)
    {
        std::cout << "O exceptie nu a fost prinsa !" << std::endl;
        ex.what();
    }

    return 0;
}
