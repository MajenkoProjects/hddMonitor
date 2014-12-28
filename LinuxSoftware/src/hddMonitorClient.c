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
