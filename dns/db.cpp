/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */
#include <string.h>
#include <sqlite3.h>
#include <iostream>

#include "db.h"
#include "exceptions.h"


char TABLE_NAME[] = "my_dns",
     DOMAIN[]     = "domain",
     IP[]         = "ip";

unsigned short DB::IP_MAX_SIZE = 3 * 4 + 3; /* 4 sectiuni cu 3 char + 3 puncte */

static int callback(void* data, int colnum, char** field_data, char** field_name)
{
    /* Functia de callback pentru SQLite3
     *
     * @param[in, out] data
     *  Parametru setat de noi
     *
     * @param[in] rownum
     *  Numarul de coloane
     *
     * @param[in] field_data
     *  Array cu informatiile din coloane
     *
     * @param[in] field_name
     *  Array cu numele coloanelor
     */
    if (colnum == 1)
    {
        /* Verificam existenta */
        if (atoi(field_data[0]) == 0)
        {
            memset(data, '0', DB::IP_MAX_SIZE);
        }
        else
        {
            memset(data, '1', DB::IP_MAX_SIZE);
        }
    }
    else
    {
        /* Returnam un IP */
        if (colnum != 2 || (strcmp(field_name[0], DOMAIN) != 0) ||
            strcmp(field_name[1], IP) != 0)
        {
            /* Nu avem numarul dorit de coloane */
            return 1;
        }

        strcpy((char*)data, field_data[1]);
    }
    return 0;
}

DB::DB(char* filename)
{
    /* Initializam conexiunea
     *
     *
     * In cazul in care fisierul nu exista sau nu respecta
     * structura o vom genera pe loc si o vom popula cu 2 ip-uri
     * google.com -> 172.217.22.14
     * example.com -> 1.1.1.1
     *
     * @param[in] filename
     *  Un string cu numele fisierului
     */

    this->filename = new char[strlen(filename)];
    bzero(this->filename, strlen(filename));
    strcpy(this->filename, filename);

    this->_ip = new char[this->IP_MAX_SIZE];
    bzero(this->_ip, this->IP_MAX_SIZE);

    this->errMsg = NULL;

    int rc = sqlite3_open(this->filename, &this->db);

    if (rc != 0)
    {
        throw DBConnectionException();
    }
    if (this->_is_prepare() == false)
    {
        this->_prepare();
    }
}

bool DB::_is_prepare()
{
    /* Verificam daca avem tabela de care avem nevoie */
    this->lock.lock();
    memset(this->_ip, 0, this->IP_MAX_SIZE);
    std::string sql = "SELECT count(*) FROM sqlite_master WHERE type='table' AND"
                      " name='"+std::string(TABLE_NAME)+"';";

    int res = sqlite3_exec(this->db, sql.c_str(), callback, (void*)this->_ip, &this->errMsg);

    if (res == SQLITE_ABORT)
    {
        this->lock.unlock();
        throw DBMalformedTable();
    }

    if (res != 0)
    {
        if (this->errMsg != NULL)
        {
            /* Eroare de la sqlite */
            std::cerr << this->errMsg << std::endl;
            sqlite3_free(this->errMsg);
        }

        this->lock.unlock();
        throw DBSelectException();
    }

    this->lock.unlock();
    std::cerr << this->errMsg << std::endl;
    for (int i = 0; i< this->IP_MAX_SIZE; ++i)
    {
        if (this->_ip[i] != '1')
        {
            return false;
        }
    }
    return true;
}

void DB::_prepare()
{
    /* Preagitm baza de date */
    this->lock.lock();
    memset(this->_ip, 0, this->IP_MAX_SIZE);

    /* TODO(mmicu): foloseste this->IP_MAX_SIZE in loc de 15 */
    std::string sql = "CREATE TABLE " + std::string(TABLE_NAME) + " ( " +
                      std::string(DOMAIN) + " CHAR(50)," +
                      std::string(IP)     + " CHAR(15) );";
    int res = sqlite3_exec(this->db, sql.c_str(), callback, (void*)this->_ip, &this->errMsg);

    if (res != 0)
    {
        if (this->errMsg != NULL)
        {
            /* Eroare de la sqlite */
            std::cerr << this->errMsg << std::endl;
            sqlite3_free(this->errMsg);
        }

        this->lock.unlock();
        throw DBSelectException();
    }

    this->lock.unlock();

    /* Adaugam exemplele */
    this->_insert(".google.com", "172.217.22.14");
    this->_insert(".www.google.com", "172.217.22.14");
    this->_insert(".www.google.ro", "172.217.22.14");
    this->_insert(".www.google.ro.home", "172.217.22.14");
    this->_insert(".matei.micu", "172.217.22.14");
    this->_insert(".example.com", "1.1.1.1");
}

void DB::_insert(std::string domain, std::string ip)
{
    /* Insereaza in baza de date
     *
     * @param[in] domain
     *  Domeniul pe care dorim sa il inseram
     *
     * @param[in] ip
     *  Ip-ul asociat domeniului
     */

    /* preatest domeniul pentru formatul specificat
     * <lenght><data><length><data>
     */

    this->lock.lock();
    memset(this->_ip, 0, this->IP_MAX_SIZE);

    /* NOTE(mmicu): SQL injection DROP TABLE ;) */
    std::string sql = "INSERT INTO " + std::string(TABLE_NAME) + " ( " +
                      std::string(DOMAIN) + ", " + std::string(IP) + ")  VALUES" +
                      "( '"+ domain +"', '"+ ip +"' )";

    int res = sqlite3_exec(this->db, sql.c_str(), callback, (void*)this->_ip, &this->errMsg);

    if (res != 0)
    {
        if (this->errMsg != NULL)
        {
            /* Eroare de la sqlite */
            std::cerr << this->errMsg << std::endl;
            sqlite3_free(this->errMsg);
        }

        this->lock.unlock();
        throw DBSelectException();
    }
    this->lock.unlock();
}

std::string DB::get_ip(char* name, unsigned short name_len)
{
    /* Returneaza ip-ul pentru domeniul dat ca parametru.
     * In cazul in care nu am gasit nici o inregistrare returnam
     * stringul gol.
     *
     * @param[in] name
     *  Numele domeniului
     *
     * @param[in] name_len
     *  Dimensiunea numelui
     *
     */
    this->lock.lock();

    std::string real_domail;
    unsigned char lungime = 0;
    for (unsigned char i = 0; i < name_len-1; ++i)
    {
        lungime = name[i];
        real_domail = real_domail + std::string(".");
        for (unsigned char j = i+1 ; j <= i+lungime; ++j)
        {
            real_domail = real_domail + std::string(1, name[j]);
        }
        i = i + lungime;
    }
    name = (char*)real_domail.c_str();
    std::cout << " ------  Search for " << name << std::endl;

    memset(this->_ip, 0, this->IP_MAX_SIZE);

    /* Convertim in string */
    char c_name[name_len+1];
    memset(c_name, 0, name_len+1);
    memcpy(c_name, name, name_len);
    std::string s_name(c_name), ip("");

    /* NOTE(mmicu): SQL injection DROP TABLE ;) */
    std::string sql = "SELECT " + std::string(DOMAIN)+", " +std::string(IP) +
                      " FROM " + std::string(TABLE_NAME) +
                      " WHERE " + std::string(DOMAIN) +" = '"+ s_name +"';";

    int res = sqlite3_exec(this->db, sql.c_str(), callback, (void*)this->_ip, &this->errMsg);


    if (res != 0)
    {
        if (this->errMsg != NULL)
        {
            /* Eroare de la sqlite */
            std::cerr << this->errMsg << std::endl;
            sqlite3_free(this->errMsg);
        }

    }
    else
    {
        ip = std::string(this->_ip);
    }

    this->lock.unlock();
    return ip;
}

DB::~DB()
{
    /* Dealocam memoria */
    sqlite3_close(this->db);
    delete this->filename;
    delete this->_ip;
}
