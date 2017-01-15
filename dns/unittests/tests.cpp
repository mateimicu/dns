/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <string.h>
#include <iostream>

#include "catch.hpp"
#include "../parser.h"
#include "../exceptions.h"
#include "../dns.h"


TEST_CASE("DNS-Question", "[DNS-Question]")
{
    Question q;
    unsigned short len = 2;
    int cmp = -100;

    SECTION("Test default constructor - name")
    {
        char* name;
        name = (char*)1; /* Trebuie sa fie diferit de NULL */
        unsigned short length;

        q.get_name(&name, length);
        REQUIRE(length == 0);
        REQUIRE(name == NULL);
    }

    SECTION("Test default constructor - type")
    {
        char type[len];
        memset(type, 'f',  sizeof(type));
        q.get_type(type);
        cmp = strncmp(type, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test default constructor - class")
    {
        char cls[len];
        memset(cls, 'f',  sizeof(cls));
        q.get_class(cls);
        cmp = strncmp(cls, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test setter - name")
    {

        for (unsigned short i = 1; i <= 10; i++)
        {
            unsigned short aux_len = 0;
            char name_set[i];
            char* name_get = NULL;
            memset(name_set, 'f',  i);

            q.set_name(name_set, i);
            q.get_name(&name_get, aux_len);

            cmp = strncmp(name_set, name_get, aux_len);

            REQUIRE(i == aux_len);
            REQUIRE(cmp == 0);
        }
    }

    SECTION("Test setter - type")
    {
        char type_set[len];
        char type_get[len];
        memset(type_set, 'f',  sizeof(type_set));
        memset(type_set, 'x',  sizeof(type_get));
        q.set_type(type_set);
        q.get_type(type_get);

        cmp = strncmp(type_set, type_get, len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test setter - class")
    {
        char cls_set[len];
        char cls_get[len];
        memset(cls_set, 'f',  sizeof(cls_set));
        memset(cls_set, 'x',  sizeof(cls_get));
        q.set_class(cls_set);
        q.get_class(cls_get);

        cmp = strncmp(cls_set, cls_get, len);

        REQUIRE(cmp == 0);
    }

}

TEST_CASE("DNS-Resource", "[DNS-Resource]")
{
    Resource res;
    unsigned short len = 2;
    int cmp = -100;

    SECTION("Test default constructor - name")
    {
        char* name;
        name = (char*)1; /* Trebuie sa fie diferit de NULL */
        unsigned short length;

        res.get_name(&name, length);
        REQUIRE(length == 0);
        REQUIRE(name == NULL);
    }

    SECTION("Test default constructor - type")
    {
        char type[len];
        memset(type, 'f',  sizeof(type));
        res.get_type(type);
        cmp = strncmp(type, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test default constructor - class")
    {
        char cls[len];
        memset(cls, 'f',  sizeof(cls));
        res.get_class(cls);
        cmp = strncmp(cls, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test default constructor - data")
    {
        char *data = (char*)1;
        unsigned short len = 1;

        res.get_data(&data, len);
        REQUIRE(len == 0);
        REQUIRE(data == NULL);
    }

    SECTION("Test setter - name")
    {

        for (unsigned short i = 1; i <= 10; i++)
        {
            unsigned short aux_len = 0;
            char name_set[i];
            char* name_get = NULL;
            memset(name_set, 'f',  i);

            res.set_name(name_set, i);
            res.get_name(&name_get, aux_len);

            cmp = strncmp(name_set, name_get, aux_len);

            REQUIRE(i == aux_len);
            REQUIRE(cmp == 0);
        }
    }

    SECTION("Test setter - type")
    {
        char type_set[len];
        char type_get[len];
        memset(type_set, 'f',  sizeof(type_set));
        memset(type_set, 'x',  sizeof(type_get));
        res.set_type(type_set);
        res.get_type(type_get);

        cmp = strncmp(type_set, type_get, len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test setter - class")
    {
        char cls_set[len];
        char cls_get[len];
        memset(cls_set, 'f',  sizeof(cls_set));
        memset(cls_set, 'x',  sizeof(cls_get));
        res.set_class(cls_set);
        res.get_class(cls_get);

        cmp = strncmp(cls_set, cls_get, len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test setter - data")

    {

        for (unsigned short i = 1; i <= 10; i++)
        {
            unsigned short aux_len = 0;
            char data_set[i];
            char* data_get = NULL;
            memset(data_set, 'f',  i);

            res.set_name(data_set, i);
            res.get_name(&data_get, aux_len);

            cmp = strncmp(data_set, data_get, aux_len);

            REQUIRE(i == aux_len);
            REQUIRE(cmp == 0);
        }
    }

}

TEST_CASE("DNS-Tranzaction", "[DNS-Tranzaction]")
{
    Tranzaction tr;
    unsigned short len = 2;
    int cmp = -100;

    SECTION("Test default constructor - id")
    {
        char id[len];
        memset(id, 'f',  sizeof(id));
        tr.get_id(id);
        cmp = strncmp(id, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test default constructor - flags")
    {
        char flags[len];
        memset(flags, 'f',  sizeof(flags));
        tr.get_flags(flags);
        cmp = strncmp(flags, "\0\0", len);

        REQUIRE(cmp == 0);
    }

    SECTION("Test default constructor - qcount")
    {
        char qcount[len];
        memset(qcount, 'f', len);

        unsigned short rez;
        tr.get_qcount_char(qcount);
        rez = tr.get_qcount_short();

        cmp = strncmp(qcount, "\0\0", len);
        REQUIRE(cmp == 0);
        REQUIRE(rez == 0);
    }

    SECTION("Test default constructor - ancount")
    {
        char ancount[len];
        memset(ancount, 'f', len);

        unsigned short rez;
        tr.get_ancount_char(ancount);
        rez = tr.get_ancount_short();

        cmp = strncmp(ancount, "\0\0", len);
        REQUIRE(cmp == 0);
        REQUIRE(rez == 0);
    }

    SECTION("Test default constructor - nscount")
    {
        char nscount[len];
        memset(nscount, 'f', len);

        unsigned short rez;
        tr.get_nscount_char(nscount);
        rez = tr.get_nscount_short();

        cmp = strncmp(nscount, "\0\0", len);
        REQUIRE(cmp == 0);
        REQUIRE(rez == 0);
    }

    SECTION("Test default constructor - arcount")
    {
        char arcount[len];
        memset(arcount, 'f', len);

        unsigned short rez;
        tr.get_arcount_char(arcount);
        rez = tr.get_arcount_short();

        cmp = strncmp(arcount, "\0\0", len);
        REQUIRE(cmp == 0);
        REQUIRE(rez == 0);
    }

    SECTION("Test setter - id")
    {
        char id_set[2],
             id_get[2];

        for (char i ='a'; i <= 'c'; i++)
        {
            id_set[0] = i;
            id_set[1] = i+1;

            tr.set_id(id_set);
            tr.get_id(id_get);

            cmp = strncmp(id_set, id_get, 2);
            REQUIRE(cmp == 0);
        }
    }

    SECTION("Test setter - flags")
    {
        char flags_set[2],
             flags_get[2];

        for (char i ='a'; i <= 'c'; i++)
        {
            flags_set[0] = i;
            flags_set[1] = i+1;

            tr.set_flags(flags_set);
            tr.get_flags(flags_get);
            cmp = strncmp(flags_set, flags_get, 2);
            REQUIRE(cmp == 0);
        }
    }


    SECTION("Test setter - questions")
    {
        Question q1, q2;

        REQUIRE(tr.get_qcount_short() == 0);
        
        /* One question */
        tr.add_question(q1);
        std::vector<Question> questions;
        questions = tr.get_questions();

        REQUIRE(tr.get_qcount_short() == 1);
        REQUIRE(questions.size() == 1);
        
        /* Two question */
        tr.add_question(q2);
        questions = tr.get_questions();

        REQUIRE(tr.get_qcount_short() == 2);
        REQUIRE(questions.size() == 2);
        
    }

    SECTION("Test setter - answers")
    {
        Resource r1, r2;

        REQUIRE(tr.get_ancount_short() == 0);
        
        /* One resources */
        tr.add_answer(r1);
        std::vector<Resource> resource;
        resource = tr.get_answers();

        REQUIRE(tr.get_ancount_short() == 1);
        REQUIRE(resource.size() == 1);
        
        /* Two resouces */
        tr.add_answer(r2);
        resource = tr.get_answers();

        REQUIRE(tr.get_ancount_short() == 2);
        REQUIRE(resource.size() == 2);
    }

    SECTION("Test setter - authority")
    {
        Resource r1, r2;

        REQUIRE(tr.get_nscount_short() == 0);
        
        /* One resources */
        tr.add_authority(r1);
        std::vector<Resource> resource;
        resource = tr.get_authority();

        REQUIRE(tr.get_nscount_short() == 1);
        REQUIRE(resource.size() == 1);
        
        /* Two resouces */
        tr.add_authority(r2);
        resource = tr.get_authority();

        REQUIRE(tr.get_nscount_short() == 2);
        REQUIRE(resource.size() == 2);
    }

    SECTION("Test setter - additional_sections")
    {
        Resource r1, r2;

        REQUIRE(tr.get_arcount_short() == 0);
        
        /* One resources */
        tr.add_additiona_section(r1);
        std::vector<Resource> resource;
        resource = tr.get_additional_sections();

        REQUIRE(tr.get_arcount_short() == 1);
        REQUIRE(resource.size() == 1);
        
        /* Two resouces */
        tr.add_additiona_section(r2);
        resource = tr.get_additional_sections();

        REQUIRE(tr.get_arcount_short() == 2);
        REQUIRE(resource.size() == 2);
    }

}
