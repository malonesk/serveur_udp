#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* message(char* buf) {
    char mes[26];
    int i=0;
    int cpt=0;
    while (buf[i]!='\0') {
        cpt++;
        i++;
    }
    sprintf(mes,"Taille du message : %d", cpt);
    printf("%s",mes);
    return mes;
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

    struct sockaddr_in* from=malloc(sizeof(serveurin));
    char buffer[65535];
    int n;

    if((n = recvfrom(sock, buffer, sizeof buffer, 0, (struct sockaddr *)&from, &len)) < 0)
    {
        perror("recvfrom()");
        exit(1);
    } else {
        printf("Recu : %s\n", buffer);
    }

    //buffer[n] = '\0';

/* traitement */
    char* answer = malloc(65535*sizeof(char));
    answer=message(buffer);

    if(sendto(sock, answer, strlen(answer), 0, (struct sockaddr *)&from, len) < 0)
    {
        perror("sendto()");
        exit(1);
    }

    exit(0);
}