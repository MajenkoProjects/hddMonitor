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
    sprintf(temp, "%s %lu %lu\n", inet_ntoa(client_addr.sin_addr), packet->readValue, packet->writeValue);
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

    while (1) {
        service(sockfd);
    }

    return 0;
}