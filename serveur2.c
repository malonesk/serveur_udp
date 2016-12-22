#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* taille(char* buf, char* answer) {
    char* t = malloc(5*sizeof(char));
    int i=0;
    int cpt=0;
    while (buf[i]!='\0') {
        cpt++;
        i++;
    }
    sprintf(t,"%s %d octets",answer,cpt);
    return t;
}

int main(int argc, char* argv[]) {
    int port;
    if (argc != 2) {
        perror("USAGE:");
        exit(1);
    } else {
        port=atoi(argv[1]);
    }
    int sock;
    if ((sock=socket(AF_INET, SOCK_DGRAM, 0))<0) {
        perror("Creation");
        exit(1);
    }

    struct sockaddr_in serveurin;
    serveurin.sin_family=AF_INET;
    serveurin.sin_port=htons(port);
    serveurin.sin_addr.s_addr=htonl(INADDR_ANY);
    socklen_t len=sizeof(serveurin);

    if(bind(sock, (struct sockaddr*)&serveurin,len)<0) {
        perror("bind");
        exit(1);
    }

    struct sockaddr_in from;
    char buffer[65535];
    int n;

    if((n = recvfrom(sock, buffer, sizeof buffer, 0, (struct sockaddr *)&from, &len)) < 0)
    {
        perror("recvfrom()");
        exit(1);
    } else {
        printf("%s %d %s\n", inet_ntoa(from.sin_addr) , ntohs(from.sin_port), buffer);
    }

    sleep(2);

/* traitement */
    char* answer = "lu un message de ";
    char* t=taille(buffer, answer);
  //  sleep(3);
    if(sendto(sock, t, (strlen(t)), 0, (struct sockaddr *)&from, len) < 0)
    {
        perror("sendto()");
        exit(1);
    }


    exit(0);
}