/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#include <iostream>
#include "parser.h"
#include "exceptions.h"
#include "server.h"

using namespace std;

/* Vom folosi asta pentru sign handlers */
Server* MAIN_SERVER = NULL;

Parser prepare_parser()
{
    /* Pregateste parserul cu toate valorile */
    Parser parser;
    StrOption* verbosity = new StrOption('v', "verbose", "Verbosity level(v, vv, vvv).", false);
    verbosity->set_default("v");

    BoolOption* debug = new BoolOption('d', "debug", "Debug mode(default false).", false);
    debug->set_default(false);

    IntOption* port = new IntOption('p', "port", "The port(defualt 53).", false);
    port->set_default(53);

    IntOption* min_threads = new IntOption(
        't', "min_threads", "The min. number of threads, or the base threads (default 2).", false);
    min_threads->set_default(2);

    IntOption* backlog = new IntOption(
        'b', "backlog", "The size of the listen queue ( defaut 10).", false);
    backlog->set_default(10);

    IntOption* max_threads = new IntOption(
        'm', "max_threads", "The max. number of threads (default 12).", false);
    max_threads->set_default(12);

    max_threads->set_default(12);

    StrOption* config_file_path = new StrOption('c', "config", "Config file path.", false);
    config_file_path->set_default("");

    parser.add_option(verbosity);
    parser.add_option(debug);
    parser.add_option(port);
    parser.add_option(min_threads);
    parser.add_option(max_threads);
    parser.add_option(config_file_path);
    parser.add_option(backlog);
    return parser;
}

int main(int argc, char *argv[])
{
    Parser p = prepare_parser();
    std::cout << "Start0 " << std::endl;
    p.parse(argc, argv);
    std::cout << "Start1 " << std::endl;

    Server s(p["port"]->get_int(), p["backlog"]->get_int());
    std::cout << "Start2 " << std::endl;
    MAIN_SERVER = &s;
    std::cout << "Start3 " << std::endl;

    std::cout << "Start " << std::endl;
    s.start();

    char ch[100], sd[2];
    sd[0] = 'a';
    sd[1] = 'a';
    socklen_t len_client;

    struct sockaddr client;

    s.read(ch, 100, 0, (struct sockaddr*) &client, &len_client);
    cout << "AM citit :" <<  ch << endl;

    s.send(sd, 2, 0, (struct sockaddr*) &client, len_client);
    cout << "AM scris 12:" <<  12 << endl;

    s.stop();
    return 0;
}
