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
#include <netdb.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


#define MAXHOSTNAME 32
#define PORT_NUM 4444

struct packet {
    uint32_t id;
    uint64_t readValue;
    uint64_t writeValue;
};

int main(int argc, char **argv) {
	int sockfd;
	FILE *f;
	struct packet buffer;
	struct sockaddr_in sa;
    char temp[1024];
    char *tok;
    struct hostent *dest;
    struct in_addr **addr_list;
    char drivepath[100];

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <drive> <id> <host>\n", argv[0]);
        return 10;
    }

    char *drive = argv[1];
    int id = atoi(argv[2]);
    dest = gethostbyname(argv[3]);

    if (dest == NULL) {
        fprintf(stderr, "Error: host %s not found\n", argv[3]);
        return 10;
    }

    addr_list = (struct in_addr **)dest->h_addr_list;

	bzero(&sa, sizeof(sa));
	sa.sin_port = htons(4444);
	sa.sin_addr = *addr_list[0];
	sa.sin_family = AF_INET;
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Unable to create socket: %d\n", errno);
		return 10;
	}

	uint64_t lastRead=0;
	uint64_t lastWrite=0;

    snprintf(drivepath, 100, "/sys/block/%s/stat", drive);
    f = fopen(drivepath, "r");
    if (!f) {
        fprintf(stderr, "Drive not found: %s\n", drive);
        return 10;
    }
    fclose(f);
        
    if (fork()) {
        return 0;
    }

    buffer.id = id;

	while (1) {
		uint64_t rv = 0;
		uint64_t wv = 0;

		f = fopen(drivepath, "r");
		fgets(temp, 1023, f);
		fclose(f);

		int j = 0;
		tok = strtok(temp, " \r\n\t");
		while (tok) {
			if (j == 2) {
				rv = strtoul(tok, NULL, 10);
			}
			if (j == 6) {
				wv = strtoul(tok, NULL, 10);
			}
			j++;
			tok = strtok(NULL, " ");
		}

		if (lastRead == 0) {
			lastRead = rv;
		}
		if (lastWrite == 0) {
			lastWrite = wv;
		}

		buffer.readValue = 0;
		buffer.writeValue = 0;

		if ((rv != lastRead) || (wv != lastWrite)) {
			buffer.readValue = rv - lastRead;
			buffer.writeValue = wv - lastWrite;
			sendto(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, sizeof(sa));
		}
		lastRead = rv;
		lastWrite = wv;
		usleep(100000);
	}
	return 0;
}
