/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H value
#include <exception>
#include <string>

/* Aici sunt toate exceptiile */

class BaseException : public std::exception
{
    public:
        /* Exceptie de baza pentru tot proiectul */
        const char * what () const throw ();
};

/* Parser Exceptions */

class ParserException: public BaseException
{
    public:
        /* Exceptie de baza pentru tot proiectul */
        const char * what () const throw ();
};


class OptionException: public ParserException
{
    public:
        /* Exceptie de baza pentru Parserul proiectului */
        const char * what () throw ();
        std::string primit;
        OptionException(std::string& primit);
        ~OptionException() throw ();
};


/* Int Eceptions */
class IntException: public OptionException
{
    public:
        /* Exceptie de baza pentru IntOption */
        const char * what () throw ();
        IntException(std::string& primit);
};

class IntValueError: public IntException
{
    public:
        /* Valoarea primita pentru aceasta optiune nu este corecta */
        const char * what () const throw ();
        IntValueError(std::string& primit);
};

/* Str Eceptions */
class StrException: public OptionException
{
    public:
        /* Exceptie de baza pentru StrOption */
        const char * what () throw ();
        StrException(std::string& primit);
};


class StrValueError: public StrException
{
    public:
        /* Valoarea primita pentru aceasta optiune nu este corecta */
        const char * what () throw ();
        StrValueError(std::string& primit);
};

/* Bool Eceptions */
class BoolException: public OptionException
{
    public:
        /* Exceptie de baza pentru BoolOption */
        const char * what () throw ();
        BoolException(std::string& primit);
};

class BoolValueError: public BoolException
{
    public:
        /* Valoarea primita pentru aceasta optiune nu este corecta */
        const char * what () throw ();
        BoolValueError(std::string& primit);
};

class InvalidOptionException: public ParserException
{
    public:
        const char * what () const throw ();
};

class ArgumentsLeft: public ParserException
{
    public:
        const char * what () const throw ();
};

class NotTheRightType: public ParserException
{
    public:
        const char * what () const throw ();
};

class ServerException: public BaseException
{
    public:
        const char * what () const throw ();
};

class SocketException: public ServerException
{
    public:
        const char * what () const throw ();
};

class BindException: public ServerException
{
    public:
        const char * what () const throw ();
};

class ListenException: public ServerException
{
    public:
        const char * what () const throw ();
};

class NotOpenException: public ServerException
{
    public:
        const char * what () const throw ();
};

class ServerNotOpen: public ServerException
{
    public:
        const char * what () const throw ();
};

class ServerReadError: public ServerException
{
    public:
        const char * what () const throw ();
};

class ReaderError: public BaseException
{
    public:
        const char * what () const throw ();
};

class ReaderValueError: public ReaderError
{
    public:
        const char * what () const throw ();
};

class DBException: public BaseException
{
    public:
        const char * what () const throw ();
};

class DBConnectionException: public BaseException
{
    public:
        const char * what () const throw ();
};

class DBCreateException: public BaseException
{
    public:
        const char * what () const throw ();
};

class DBSelectException: public BaseException
{
    public:
        const char * what () const throw ();
};

class DBMalformedTable: public BaseException
{
    public:
        const char * what () const throw ();
};
#endif /* ifndef EXCEPTIONS_H */
