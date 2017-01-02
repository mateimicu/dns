#include <iostream>
#include "parser.h"
#include "exceptions.h"

using namespace std;

Parser prepare_parser(){
    /* Pregateste parserul cu toate valorile */
    Parser parser;
    StrOption* verbosity = new StrOption('v', "verbose", "Verbosity level(v, vv, vvv).", false);
    verbosity->set_default("v");

    BoolOption* debug = new BoolOption('d', "debug", "Debug mode(default false).", false);
    debug->set_default(false);

    IntOption* port = new IntOption('p', "port", "The port(defualt 53).", false);
    port->set_default(53);

    IntOption* min_threads = new IntOption('t', "min_threads", "The min. number of threads, or the base threads (default 2).", false);
    min_threads->set_default(2);

    IntOption* max_threads = new IntOption('m', "max_threads", "The max. number of threads (default 12).", false);
    min_threads->set_default(12);

    StrOption* config_file_path = new StrOption('c', "config", "Config file path.", false);
    config_file_path->set_default("");

    parser.add_option(verbosity);
    parser.add_option(debug);
    parser.add_option(port);
    parser.add_option(min_threads);
    parser.add_option(max_threads);
    parser.add_option(config_file_path);
    return parser;
}

int main(int argc, char *argv[])
{
    Parser p = prepare_parser();
    p.parse(argc, argv);
    return 0;
}
