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

int main(int argc, char* argv[]) {
    char* address;
    int port;
    char buffer[65535];
    if (argc<4) {
        perror("USAGE: client <adresse> <port> <message>\n");
        exit(1);
    } else {
        address=argv[1];
        port=atoi(argv[2]);
        int i=3;
        //initialisation du buffer avec les argument correspondant au message que le client veut envoyer
        while (i<=argc-1) {
            sprintf(buffer,"%s %s", buffer, argv[i]);
            i++;
        }
    }
    int sock;
    //Creation de la socket
    if ((sock=socket(AF_INET, SOCK_DGRAM, 0))<0) {
        perror("Creation");
        exit(1);
    }
    //déclaration et initialisation d'une structure permettant de stocker le informations du serveur à atteindre
    struct sockaddr_in to;
    to.sin_family=AF_INET;
    to.sin_port=htons(port);
    inet_aton(address, &to.sin_addr);
    socklen_t len = sizeof to;
    //déclaration d'une nouvelle structure affectée à l'appelle de getaddrinfo aux nouvelles informations du serveur
    //dans le cas ou l'on voudrait se connecter a un serveur via un nom de domaine
    struct addrinfo* next;
    //déclaration et initilisation d'une structure addrinfo représentant les infos serveur, passée en parametre de getaddrinfo
    struct addrinfo addrinfo_to;
    addrinfo_to.ai_family=AF_INET;
    addrinfo_to.ai_socktype=SOCK_DGRAM;
    addrinfo_to.ai_protocol=IPPROTO_UDP;
    addrinfo_to.ai_addrlen=len;
    addrinfo_to.ai_addr=(struct sockaddr *)&to;
    addrinfo_to.ai_canonname=address;
    addrinfo_to.ai_next=next;
    //recuperation des informations correspondant au serveur, notamment l'adresse IP correspondante
    //si le client cherche a se connecter a un nom de domaine
    if(getaddrinfo(address,argv[2],&addrinfo_to,&next)<0) {
        perror("getaddrinfo");
        exit(1);
    }
    //extraction d'une structure sockaddr a partir de la structure addrinfo contenant les nouvelle infos serveur
    struct sockaddr* newto;
    newto=next->ai_addr;
    //envoi du message à l'adresse pointée par la nouvelle structure newto
    if(sendto(sock, buffer, strlen(buffer), 0, newto, len) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    //déclaration du buffer de reception de la reponse serveur
    char buf[65535];
    //reception de la reponse serveur
    if((recvfrom(sock, buf, sizeof buf, 0, newto, &len)) < 0)
    {
        perror("recvfrom()");
        exit(1);
    }
    //Affichage de la réponse serveur
    printf("%s\n", buf);
    close(sock);

    exit(0);
}
