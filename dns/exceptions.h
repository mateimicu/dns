#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H value
#include <exception>
#include <string>

/* Aici sunt toate exceptiile */

class BaseException : public std::exception
{
    /* Exceptie de baza pentru tot proiectul */
   const char * what () const throw ();
};

/* Parser Exceptions */

class ParserException: public BaseException
{
    /* Exceptie de baza pentru Parserul proiectului */
   const char * what () const throw ();
};


class OptionException: public ParserException 
{
    /* Exceptie de baza pentru Parserul proiectului */
    const char * what () throw ();
    public:
        std::string primit;
        OptionException(std::string& primit);
        ~OptionException() throw ();
};


/* Int Eceptions */
class IntException: public OptionException 
{
    /* Exceptie de baza pentru IntOption */
   const char * what () throw ();
    public:
        IntException(std::string& primit);
};

class IntValueError: public IntException
{
    /* Valoarea primita pentru aceasta optiune nu este corecta */
    const char * what () const throw ();
    public:
        IntValueError(std::string& primit);
};

/* Str Eceptions */
class StrException: public OptionException 
{
    /* Exceptie de baza pentru StrOption */
   const char * what () throw ();
    public:
        StrException(std::string& primit);
};


class StrValueError: public StrException 
{
    /* Valoarea primita pentru aceasta optiune nu este corecta */
   const char * what () throw ();
    public:
        StrValueError(std::string& primit);
};

/* Bool Eceptions */
class BoolException: public OptionException 
{
    /* Exceptie de baza pentru BoolOption */
    const char * what () throw ();
    public:
        BoolException(std::string& primit);
};

class BoolValueError: public BoolException
{
    /* Valoarea primita pentru aceasta optiune nu este corecta */
   const char * what () throw ();
    public:
        BoolValueError(std::string& primit);
};

class InvalidOptionException: public ParserException 
{
   const char * what () const throw ();
};

class ArgumentsLeft: public ParserException 
{
   const char * what () const throw ();
};

class NotTheRightType: public ParserException
{
   const char * what () const throw ();
};

class ServerException: public BaseException 
{
   const char * what () const throw ();
};

class SocketException: public ServerException 
{
   const char * what () const throw ();
};

class BindException: public ServerException 
{
   const char * what () const throw ();
};

class ListenException: public ServerException 
{
   const char * what () const throw ();
};

class NotOpenException: public ServerException 
{
   const char * what () const throw ();
};

class ServerNotOpen: public ServerException 
{
   const char * what () const throw ();
};

class ServerReadError: public ServerException 
{
   const char * what () const throw ();
};
#endif /* ifndef EXCEPTIONS_H */
