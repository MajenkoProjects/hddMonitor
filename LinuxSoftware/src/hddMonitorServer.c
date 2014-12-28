/*
 * Copyright (c) 2014, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include <fcntl.h>

#define BACKLOG 5
#define PORT_NUM 4444

struct packet {
    uint32_t id;
    uint64_t readValue;
    uint64_t writeValue;
};

int arduino;

void service(int sock) {
    char request[BUFSIZ];
    ssize_t rv;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    bzero((char *)request, sizeof(request));

    rv = recvfrom(sock, request, sizeof(request), 0, (struct sockaddr *)&client_addr, &client_len);
    if (rv == -1) {
        fprintf(stderr, "Error getting packet: %d\n", errno);
        return;
    }

    struct packet *packet = (struct packet *)request;

    char temp[100];
    sprintf(temp, "%d %lu %lu\n", packet->id, (unsigned long)packet->readValue, (unsigned long)packet->writeValue);
    write(arduino, temp, strlen(temp));
}

int main(int argc, char **argv) {
    struct termios options;
    struct sockaddr_in sa;
    int sockfd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/ttyACMx\n", argv[0]);
        return 10;
    }

    bzero(&sa, sizeof(sa));

    sa.sin_family=AF_INET;
    sa.sin_port=htons(4444);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Unable to create socket\n");
        return 10;
    }

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        fprintf(stderr, "Uable to bind socket\n");
        return 10;
    }

    arduino = open(argv[1], O_RDWR|O_NOCTTY);


    fcntl(arduino, F_SETFL, 0);
    tcgetattr(arduino, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    if (tcsetattr(arduino, TCSANOW, &options) != 0) {
        fprintf(stderr, "Can't set up serial\n");
        return 10;
    }

    // All is ready. Let's fork.

    int pid = fork();
    if (pid) {
        return 0;
    }

    while (1) {
        service(sockfd);
    }

    return 0;
}
