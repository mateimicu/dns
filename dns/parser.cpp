/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#include "parser.h"
#include "exceptions.h"

#include <climits>
#include <algorithm>
#include <vector>
#include <string>
#include <errno.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>


extern int errno;

/* Utils */
std::string pad_right(std::string const& str, size_t s)
{
    /* Adauga padding la dreapta un string pana la o dimensiune anume
     *
     * @param[in] str
     *   Stringul caruia vrem sa ii adaugam padding
     *
     * @param[in] s
     *   Cat pading dorim sa ii adaugam
     */
    if (str.length() < s)
    {
        return str + std::string(s-str.length(), ' ');
    }
    else
    {
        return str;
    }
}

std::string pad_left(std::string const& str, size_t s)
{
    /* Adauga padding la stanga un string pana la o dimensiune anume
     *
     * @param[in] str
     *   Stringul caruia vrem sa ii adaugam padding
     *
     * @param[in] s
     *   Cat pading dorim sa ii adaugam
     */
    if ( str.length() < s )
    {
        return std::string(s-str.length(), ' ') + str;
    }
    else
    {
        return str;
    }
}


enum STR2INT_ERROR
{
    S2I_SUCCESS,
    S2I_OVERFLOW,
    S2I_UNDERFLOW,
    S2I_INCONVERTIBLE
};

STR2INT_ERROR str2int (int &i, char const *s, int base = 0)
{
    char *end;
    long  val;
    errno = 0;
    val = strtol(s, &end, base);
    if ((errno == ERANGE && val == LONG_MAX) || val > INT_MAX)
    {
        return S2I_OVERFLOW;
    }
    if ((errno == ERANGE && val == LONG_MIN) || val < INT_MIN)
    {
        return S2I_UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0')
    {
        return S2I_INCONVERTIBLE;
    }
    i = val;
    return S2I_SUCCESS;
}


Option::Option(char short_name, std::string long_name, std::string help_message, bool required)
{
    /* Constructor pentru Clasa de baza
     *
     * @param[in] short_name
     *  Un char care reprezinta varianta prescurtata a parametrului
     *  Ex: -d
     *
     * @param[in] long_name
     *  Un string care reprezinta varianta lunga aparametrului
     *  Ex: --delimitator
     *
     * @param[in] help_string
     *  Un string care reprezinta descrierea optiuni
     *
     * @param[in] required
     *  Daca un parametru este necesar sau nu
     */
    this->short_name = short_name;
    this->long_name = long_name;
    this->help_message = help_message;
    this->default_set = false; /* o valoare default este setata dupa crearea obiectului */
    this->required=required;
}

std::string Option::get_help()
{
    /* Returnam mesajul de ajutor complet */

    // TODO(mmicu):
    // Adauga un format de genu:
    // (2-4 spaces)short, long(4 - 8 spaces)help_message
    // poate si un word wrap de ~ 80 caractere

    char ch[2];
    ch[0] = this->short_name;
    ch[1] = 0;
    std::string to_ret = "  -" + std::string(ch) + ", --"+pad_right(this->long_name, 12) +
                         "  "+pad_right("("+this->get_type() +")", 6) + " :" +
                         this->help_message + "\n";
    return to_ret;
}

void Option::set_value(std::string param_value)
{
    /* Seteaza valoarea parametrului
     *
     * @param param_value
     *  Valoarea parametrului
     */
    throw ParserException();
}

char Option::get_short_name()
{
    /* Returneaza numele scurt a optinui */
    return this->short_name;
}

std::string Option::get_long_name()
{
    /* Returneaza numele lung a optinui */
    return this->long_name;
}

bool Option::is_required()
{
    /* Returneaza true daca optiunea este obligatori, fals
     * in caz contrar
     */
    return this->required;
}

std::string Option::get_type()
{
    /* Returneaza un string care reprezinta tipul optinui */
    return std::string("none");
}

std::vector<std::string> Option::parse(std::vector<std::string> parameters)
{
    /* Parseaza lista cu parametri si verifica daca parametrul cautat
     * se afla in lista.
     *
     * @param[in] parameters
     *  Un vector cu stringuri, fiecare string este un parametru de la CLI.
     */
    // for (std::vector<std::string> it = parameters.begin(); it != parameters.end();)
    for (auto it = parameters.begin(); it != parameters.end();)
    {
        char sh[3];
        sh[0] = '-';
        sh[1] = this->short_name;
        sh[2] = 0;
        if ((std::string)(*it) == std::string(sh) ||
           (std::string)(*it) == "--"+this->long_name)
        {
            parameters.erase(it);
            this->set_value((std::string)(*it));
            parameters.erase(it);
        }
        else
        {
            /* Avanseaza doar daca nu ai eliminat ceva */
            it++;
        }

    }

    return parameters;
}

int Option::get_int()
{
    throw NotTheRightType();
}

std::string Option::get_string()
{
    throw NotTheRightType();
}

bool Option::get_bool()
{
    throw NotTheRightType();
}

/* IntMethods */
IntOption::IntOption(char short_name, std::string long_name,
                     std::string help_message, bool required):
    Option(short_name, long_name, help_message, required)
{
    /* Constructor pentru IntOption
     *
     * @param[in] short_name
     *  Un char care reprezinta varianta prescurtata a parametrului
     *  Ex: -d
     *
     * @param[in] long_name
     *  Un string care reprezinta varianta lunga aparametrului
     *  Ex: --delimitator
     *
     * @param[in] help_string
     *  Un string care reprezinta descrierea optiuni
     *
     * @param[in] required
     *  Daca un parametru este necesar sau nu, implicit este setat pe false
     */
}

int IntOption::get_int()
{
    /* \return{Returneaza valoarea parametrului} */
    return this->value;
}

void IntOption::set_default(int default_value)
{
    /* Seteaza valoarea default a acestei otiuni
     *
     * @param[in] defualt_value
     *  Valoarea default.
     */
    this->default_value = default_value;
    this->is_set = true; /* Putem sa o consideram setat */
    this->required = false; /* nu trebuie sa fie parsata de la CLI */
}

void IntOption::set_value(std::string parameter)
{
    /* Seteaza valoarea unui parametru convorm unui string
     *
     * @param parameter
     *  Stringul care contine valoarea parametrului
     */
    STR2INT_ERROR out = str2int(this->value, parameter.c_str(), 10);

    if (out != S2I_SUCCESS)
    {
        throw IntValueError(parameter);
    }
    this->is_set = true;
}

std::string IntOption::get_type()
{
    /* Returneaza un string care reprezinta tipul optinui */
    return std::string("int");
}

/* String Methods */

StrOption::StrOption(char short_name, std::string long_name,
                     std::string help_message, bool required):
    Option(short_name, long_name, help_message, required)
{
    /* Constructor pentru StrOption
     *
     * @param[in] short_name
     *  Un char care reprezinta varianta prescurtata a parametrului
     *  Ex: -d
     *
     * @param[in] long_name
     *  Un string care reprezinta varianta lunga aparametrului
     *  Ex: --delimitator
     *
     * @param[in] help_string
     *  Un string care reprezinta descrierea optiuni
     *
     * @param[in] required
     *  Daca un parametru este necesar sau nu, implicit este setat pe false
     */
}

std::string StrOption::get_string()
{
    /* \return{Returneaza valoarea parametrului} */
    return this->value;
}

void StrOption::set_default(std::string default_value)
{
    /* Seteaza valoarea default a acestei otiuni
     *
     * @param[in] defualt_value
     *  Valoarea default.
     * */
    this->default_value = default_value;
    this->is_set = true; /* Putem sa o consideram setat */
    this->required = false; /* nu trebuie sa fie parsata de la CLI */
}

void StrOption::set_value(std::string parameter)
{
    /* Seteaza valoarea unui parametru convorm unui string
     *
     * @param parameter
     *  Stringul care contine valoarea parametrului
     */
    this->value = parameter;
    this->is_set = true;
}

std::string StrOption::get_type()
{
    /* Returneaza un string care reprezinta tipul optinui */
    return std::string("str");
}

/* Bool Methods */
BoolOption::BoolOption(char short_name, std::string long_name,
                     std::string help_message, bool required):
    Option(short_name, long_name, help_message, required)
{
    /* Constructor pentru BoolOption
     *
     * @param[in] short_name
     *  Un char care reprezinta varianta prescurtata a parametrului
     *  Ex: -d
     *
     * @param[in] long_name
     *  Un string care reprezinta varianta lunga aparametrului
     *  Ex: --delimitator
     *
     * @param[in] help_string
     *  Un string care reprezinta descrierea optiuni
     *
     * @param[in] required
     *  Daca un parametru este necesar sau nu, implicit este setat pe false
     */
}

bool BoolOption::get_bool()
{
    /* \return{Returneaza valoarea parametrului} */
    return this->value;
}

void BoolOption::set_default(bool default_value)
{
    /* Seteaza valoarea default a acestei otiuni
     *
     * @param[in] defualt_value
     *  Valoarea default.
     * */
    this->default_value = default_value;
    this->is_set = true; /* Putem sa o consideram setat */
    this->required = false; /* nu trebuie sa fie parsata de la CLI */
}

void BoolOption::set_value(std::string parameter)
{
    /* Seteaza valoarea unui parametru convorm unui string
     *
     * @param parameter
     *  Stringul care contine valoarea parametrului
     */
    std::string lower_param = parameter;
    /* Convert to lower case */
    std::transform(lower_param.begin(), lower_param.end(),
                   lower_param.begin(), ::tolower);

    if (lower_param == "true" || lower_param == "t")
    {
        this->value = true;
        this->is_set = true;
        return;
    }

    if (lower_param == "false" || lower_param == "f")
    {
        this->value = false;
        this->is_set = true;
        return;
    }

    throw BoolValueError(parameter);
}

std::string BoolOption::get_type()
{
    /* Returneaza un string care reprezinta tipul optinui */
    return std::string("bool");
}

/* Parser */

Parser::Parser()
{
    /* Initializeaza un parser */
    this->options.clear();
    this->args.clear();
}
void Parser::add_option(Option* opt)
{
    /* Adauga o noua optiune parserului
     *
     * @param opt
     *  O instanta a unei optiuni
     */
    for (std::vector<Option*>::iterator it = this->options.begin();
        it != this->options.end(); it++)
    {
        if ((*it)->get_short_name() == opt->get_short_name() ||
            (*it)->get_long_name() == opt->get_long_name())
        {
            throw InvalidOptionException();
        }
    }
    this->options.push_back(opt);
}

void Parser::parse(int argc, char* argv[])
{
    /* Parse the arguments
     *
     * @param argc
     *  Numarul de argumente primite
     *
     * @param argv
     *  Un vector cu char* care reprezinta argumentele
     */

    /* Pastram numele executabilului */
    this->exe_name = std::string(argv[0]);

    /* Transformam char* in vector de std::string */

    /* NOTE(mmicu): Pornim de la 1 ca sa excludem
     * numele binarului
     */
    for (int i = 1; i < argc; i++)
    {
        this->args.push_back(std::string(argv[i]));
    }

    /* Verific daca s-a cecur help-ul */
    for (auto it = this->args.begin(); it != this->args.end(); it++)
    {
        if ((*it) == "-h" || (*it) == "--help" )
        {
            this->get_help();
            exit(0);
        }
    }

    /* Daca nu avem nici un argument, afisam help-ul */
    if (this->args.size() == 0)
    {
        /* Daca afisam mesajul de ajutor, nu mai parsam restul argumentelor.
         * Terminam totuși executia programului. */
        this->get_help();
        exit(0);
    }

    for (std::vector<Option*>::iterator it = this->options.begin();
        it != this->options.end(); it++)
    {
        this->args = (*it)->parse(this->args);
    }

    if (this->args.size() > 0 )
    {
        throw ArgumentsLeft();
    }
}

Option* Parser::operator [](std::string name)
{
    /* Returneaza optiunea cu acel nume
     *
     * @param name
     *  Numele optiuni, scurt sau lung
     */

    for (std::vector<Option*>::iterator it = this->options.begin();
        it != this->options.end(); it++)
    {
        char sh[2];
        sh[0] = (*it)->get_short_name();
        sh[0] = 0;
        if ((*it)->get_long_name() == name ||
            std::string(sh) == name)
        {
            return *it;
        }
    }

    throw InvalidOptionException();
}

void Parser::get_help()
{
    /* Print the help for each option */
    std::cout << "Usage :" << std::endl << "  " << this->exe_name << std::endl;


    /* Argumente obligatorii */
    for (std::vector<Option*>::iterator it = this->options.begin();
        it != this->options.end(); it++)
    {
        if ((*it)->is_required())
        {
            std::cout << (*it)->get_help() << std::endl;
        }
    }
    
    std::cout<< "Options :" << std::endl;
    for (std::vector<Option*>::iterator it = this->options.begin();
        it != this->options.end(); it++)
    {
        if ((*it)->is_required() == false)
        {
            std::cout << (*it)->get_help() << std::endl;
        }
    }
}
