/* Copyright (C) Released under the
 * GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * Author: Micu Matei-Marius
 */

#ifndef SERVER_H
#define SERVER_H value
#include "exceptions.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

class Server
{
    private:
        unsigned short port;
        struct sockaddr_in server;
        int sock;
        unsigned int backlog;
    public:
        Server(unsigned short port, unsigned int backlog);
        void start();
        void stop();
        ssize_t read(void *buf, size_t len, int flags,
                  struct sockaddr *src_addr, socklen_t *addrlen);

        ssize_t send(const void *buf, size_t len, int flags,
                     const struct sockaddr *dest_addr, socklen_t addrlen);
        unsigned short get_port();
};
#endif /* ifndef SERVER_H */
