#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>


int main(int argc, char* argv[]) {
    char* address;
    int port;
    char buffer[65535];
    if (argc!=3) {
        perror("USAGE:");
        exit(1);
    } else {
        address=argv[1];
        port=atoi(argv[2]);
    }
    int sock;
    if ((sock=socket(AF_INET, SOCK_DGRAM, 0))<0) {
        perror("Creation");
        exit(1);
    }
    struct sockaddr_in to;
    to.sin_family=AF_INET;
    to.sin_port=htons(port);
    inet_aton(address, &to.sin_addr);
    socklen_t len = sizeof to;

    int nblus=read(0,buffer,sizeof buffer);
    buffer[nblus]='\0';
    if(sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&to, len) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    char buf[65535];
    int n;
    if((n = recvfrom(sock, buf, sizeof buf, 0, (struct sockaddr*)&to, &len)) < 0)
    {
        perror("recvfrom()");
        exit(1);
    } else {
        printf("Reponse : %s\n", buf);
    }
    exit(0);
}