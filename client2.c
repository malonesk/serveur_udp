#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <netdb.h>

void alarm_hand(){};

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

    struct addrinfo* next;

    struct addrinfo addrinfo_to;
    addrinfo_to.ai_family=AF_INET;
    addrinfo_to.ai_socktype=SOCK_DGRAM;
    addrinfo_to.ai_protocol=IPPROTO_UDP;
    addrinfo_to.ai_addrlen=len;
    addrinfo_to.ai_addr=(struct sockaddr *)&to;
    addrinfo_to.ai_canonname=address;
    addrinfo_to.ai_next=next;

    if(getaddrinfo(address,argv[2],&addrinfo_to,&next)<0) {
        perror("getaddrinfo");
        exit(1);
    }
    struct sockaddr* newto;
    newto=next->ai_addr;

    read(0,buffer,sizeof buffer);
    if(sendto(sock, buffer, strlen(buffer), 0, newto, len) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    char buf[65535];
    signal(SIGALRM, alarm_hand);
    alarm(2);
    if((recvfrom(sock, buf, sizeof buf, 0, newto, &len)) < 0)
    {
        perror("recvfrom()");
        exit(1);
    }
    if (alarm(2)==0) {
        printf("Le serveur n'a pas repondu a temps.\n");
        exit(1);
    } else {
        printf("%s\n", buf);
    }

    exit(0);
}