/*
 * CSCI 363 Computer Networks
 * Lab 2
 * Yifan Ge
 * 01/28/2014
 * 
 * File name: wrappers.c
 *
 */

#include "wrappers.h"

int wrp_socket(int domain, int type, int protocol){
	int fd;

	if( (fd = socket(domain, type, protocol)) < 0){
		perror("open socket error");
		exit(-1);
	}
	return fd;
}

int wrp_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
	int retval;

	if( (retval = bind(sockfd, addr, addrlen)) < 0){
		perror("bind error");
		exit(-1);
	}
	return retval;
}

int wrp_listen(int sockfd, int backlog){
	int retval;
	
	if( (retval = listen(sockfd, backlog)) < 0){
		perror("listen error");
		exit(-1);
	}
	return retval;
}

int wrp_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
	int newSockfd;

	if( (newSockfd = accept(sockfd, addr, addrlen)) < 0){
		perror("accept error");
		exit(-1);
	}
	return newSockfd;
}

int wrp_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
	int retval;

	if( (retval = connect(sockfd, addr, addrlen)) < 0){
		perror("connect error");
		exit(-1);
	}
	return retval;
}
