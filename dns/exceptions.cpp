#include "exceptions.h"
#include <string>

/* Aici sunt toate exceptiile */

/* Exceptie de baza pentru tot proiectul */
const char * BaseException::what () const throw () 
{
    return "BaseException: Base exception for DNS project";
}

/* Parser Exceptions */

/* Exceptie de baza pentru Parserul proiectului */
const char * ParserException::what () const throw () 
{
    return "ParserException: Base exception for parser";
}


/* Exceptie de baza pentru Parserul proiectului */
const char * what () throw () 
{
    return "OptionrException: Base exception for option";
}
OptionException::OptionException(std::string& primit) 
{
    /* Contructor pentru OptionException.
        *
        * @param[in] primit
        *  Un string care reprezinta parametrul primit.
        *  Implicit este stringul gol.
        */
    this->primit = primit;
}

OptionException::~OptionException() throw () {}


/* Int Eceptions */

/* Exceptie de baza pentru IntOption */
const char * IntException::what () throw () 
{
    return "IntException: Base exceptio for IntOption";
}

IntException::IntException(std::string& primit): OptionException(primit)
{
}

/* Valoarea primita pentru aceasta optiune nu este corecta */
const char * IntValueError::what () const throw () 
{
    return ("Nu putem parsa " + this->primit).c_str();
}

IntValueError::IntValueError(std::string& primit): IntException(primit)
{
}

/* Str Eceptions */

/* Exceptie de baza pentru StrOption */
const char * StrException::what () throw () 
{
    return "StrException: Base exceptio for StrOption";
}

StrException::StrException(std::string& primit): OptionException(primit)
{
}

/* Valoarea primita pentru aceasta optiune nu este corecta */
const char * StrValueError::what () throw () 
{
    return ("Nu putem parsa " + this->primit).c_str();
}

StrValueError::StrValueError(std::string& primit): StrException(primit)
{
}

/* Bool Eceptions */

/* Exceptie de baza pentru BoolOption */
const char * BoolException::what () throw () 
{
    return "BoolException: Base exceptio for BoolOption";
}


BoolException::BoolException(std::string& primit): OptionException(primit)
{
}

/* Valoarea primita pentru aceasta optiune nu este corecta */
const char * BoolValueError::what () throw () 
{
    return ("Nu putem parsa " + this->primit).c_str();
}

BoolValueError::BoolValueError(std::string& primit): BoolException(primit)
{
}

const char * InvalidOptionException::what() const throw()
{
    return "Nu putem adauga optiunea asta";
}

const char * ArgumentsLeft::what() const throw()
{
    return "Au ramas argumente neparsate";
}

const char * NotTheRightType::what() const throw()
{
    return "Argumentul nu are acest tip.";
}
