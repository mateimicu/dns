/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#include "server.h"
#include "exceptions.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>

extern int errno;

Server::Server(unsigned short port, unsigned int backlog)
{
    /* Initializeaza un server
     *
     * @param[in] port
     *  Portul pe care o sa deschidem servarul
     *
     * @param[in] backlog
     *  Dimensiunea cozi pentru listen (Folosit doar pentru TCP).
     * */
    this->port = port;
    this->backlog = backlog;
    this->sock = 0;

    /* umplem structura folosita de server */
    bzero(&this->server, sizeof(this->server));
    this->server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (this->port);

}

void Server::start()
{
    /* Porneste un server UDP pe local host */
    if ((this->sock = socket(this->server.sin_family, SOCK_DGRAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      throw SocketException();
    }

    /* Accepta reutilizarea portului */
    int on=1;
    setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* facem bind la socket cu (adresa, port) */
    if (bind (this->sock, (struct sockaddr *) &this->server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      throw BindException();
    }
}

void Server::stop()
{
    /* Opreste servarul */
    if (this->sock == 0)
    {
        /* Nu s-a pornis servarul nici o data */
        throw NotOpenException();

    }
    close(this->sock);
    this->sock = 0;
}

ssize_t Server::read(void *buf, size_t len, int flags,
             struct sockaddr *src_addr, socklen_t *addrlen)
{
    /* Citeste informatii de pe retea.
     *
     * @param[out] buf
     *  Unde o sa salvam datele primite
     *
     * @param[in] len
     *  Lungimea buffarului unde salvam datele
     *
     * @param[in] flags
     *  Flagurile pentru operatia de citire
     *
     * @param[out] src_addr
     *  sockaddr structura umpluta cu informatiile clientului
     *  de la care am citit date
     *
     * @param[out] addrlen
     *  Lungimea structuri sockaddr
     */
    if (this->sock == 0)
    {
        /* Daca nu am pornit servarul */
        throw ServerNotOpen();
    }

    bzero(src_addr, sizeof(src_addr));
    bzero(buf, len);
    ssize_t msglen;
    msglen = recvfrom(this->sock, buf, len, 0, src_addr, addrlen);

    if (msglen < 0)
    {
        /* Avem o eroare */
        perror("Reading from server !\n");
        throw ServerReadError();
    }

    return msglen;
}

ssize_t Server::send(const void *buf, size_t len, int flags,
                     const struct sockaddr *dest_addr, socklen_t addrlen)
{
    /* Trimitem informatii de pe retea.
     *
     * @param[in] buf
     *  Buffer cu infromatiile pe care vrem sa le trimitem
     *
     * @param[in] len
     *  Lungimea buffarului u
     *
     * @param[in] flags
     *  Flagurile pentru operatia de citire
     *
     * @param[in] dest_addr
     *  sockaddr structura umpluta cu informatiile clientului
     *  (unde vrem sa trimitem)
     *
     * @param[in] addrlen
     *  Lungimea structuri sockaddr
     */
    if (this->sock == 0)
    {
        /* Daca nu am pornit servarul */
        throw ServerNotOpen();
    }

    ssize_t msglen = 1;
    /* NOTE(mmicu): msglen nu va fi nici o data negativ in
     * while de asta putem face cast
     */
    while (static_cast<size_t>(msglen) < len)
    {
        /* Cat timp nu s-au trimiti toate informatiile */
        msglen = sendto(this->sock, buf, len, 0,
                        dest_addr, addrlen);

        if (msglen < 0)
        {
            /* Avem o eroare */
            perror("Sending from server !\n");
            throw ServerReadError();
        }
    }


    return msglen;
}
