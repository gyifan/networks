/*
 * CSCI 363 Computer Networks
 * Lab 2
 * Yifan Ge
 * 01/28/2014
 * 
 * File name: wrappers.h
 *
 */

#ifndef _WRAPPERS_H_
#define _WRAPPERS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int wrp_socket(int domain, int type, int protocol);

int wrp_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int wrp_listen(int sockfd, int backlog);

int wrp_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int wrp_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t readn(int fd, void *vptr, size_t n);

ssize_t wrp_readn(int fd, void *ptr, size_t nbytes);

ssize_t writen(int fd, const void *vptr, size_t n);

void wrp_writen(int fd, void *ptr, size_t nbytes);
#endif /* __WRAPPERS__ */
