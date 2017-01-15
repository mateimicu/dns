/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#ifndef PARTSE_H
#define PARTSE_H value
#include <string>
#include <vector>


class Option
{
    /* Clasa de baza pentru optiuni */
    protected:
        char short_name;
        std::string long_name, help_message;
        bool default_set, is_set, required;

        virtual void set_value(std::string param_value);
    public:

        Option(char short_name, std::string long_name, std::string help_message, bool required);
        std::string get_help();
        std::vector<std::string> parse(std::vector<std::string> parameters);
        char get_short_name();
        std::string get_long_name();
        bool is_required();

        virtual int get_int();
        virtual std::string get_string();
        virtual bool get_bool();

        virtual std::string get_type();
        
};


class IntOption:public Option
{
    /* Clasa care retine valoarea unui int */
    private:
        int value;
        int default_value;
    protected:
        void set_value(std::string param_value);
    public:
        IntOption(char short_name, std::string long_name, std::string help_message, bool required);
        void set_default(int default_value);
        bool is_required();

        int get_int();

        std::string get_type();
};

class StrOption:public Option
{
    /* Clasa care retine valoarea unui String */
    private:
        std::string value;
        std::string default_value;
    protected:
        void set_value(std::string param_value);
    public:
        StrOption(char short_name, std::string long_name, std::string help_message, bool required);
        void set_default(std::string default_value);
        bool is_required();

        std::string get_string();

        std::string get_type();
};

class BoolOption:public Option
{
    /* Clasa care retine valoarea unui Bool*/
    private:
        bool value;
        bool default_value;
    protected:
        void set_value(std::string param_value);
    public:
        BoolOption(char short_name, std::string long_name, std::string help_message, bool required);
        void set_default(bool default_value);
        bool is_required();

        bool get_bool();

        std::string get_type();
};


class Parser
{
    private:
        /* TODO(mmicu): Use a map here */
        std::vector<Option*> options;
        std::vector<std::string> args;
        std::string exe_name;
    public:
        Parser();
        void add_option(Option* opt);
        void parse(int argc, char* argv[]);
        Option* operator [](std::string name);
        void get_help();
};
#endif /* ifndef PARTSE_H */
