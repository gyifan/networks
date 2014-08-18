/*
 * Copyright (c) 2012 Bucknell University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: L. Felipe Perrone (perrone@bucknell.edu)
 */

#include "lab2.h"

void printsin(struct sockaddr_in *s, char *str1, char *str2) {
     char ip[16];
     printf("%s\n", str1); fflush(stdout);
     printf("%s: ", str2);fflush(stdout);
     inet_ntop((*s).sin_family, &((*s).sin_addr.s_addr), ip, sizeof(ip));
     printf("ip= %s, port= %d", ip, ntohs(s->sin_port)); fflush(stdout);
     printf("\n"); fflush(stdout);
}
