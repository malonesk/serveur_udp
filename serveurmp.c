#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


char* createmessage(char* buf) {
    char* t = malloc(8*sizeof(char)+(strlen(buf))*sizeof(char));
    if (t==NULL) {
        perror("malloc");
        createmessage(buf);
    }
    sprintf(t,"Bonjour%s",buf);
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

    while(1) {
        //Creation de la socket
        if ((sock=socket(AF_INET, SOCK_DGRAM, 0))<0) {
            perror("Creation");
            exit(1);
        }
        //initialisation d'une structure sockaddr_in pour le serveur
        struct sockaddr_in serveurin;
        serveurin.sin_family=AF_INET;
        serveurin.sin_port=htons(port);
        serveurin.sin_addr.s_addr=htonl(INADDR_ANY);
        socklen_t len=sizeof(serveurin);

        //Déclaration des variables stockant le nom de domaine et le service correspondant à l'adresse IP du client
        // ces tableaux seront passés en paramètre de getnameinfo
        char host[1024];
        char service[20];

        //la socket et la structure serveur sont liés
        if(bind(sock, (struct sockaddr*)&serveurin,len)<0) {
            perror("bind");
            exit(1);
        }
        //Intilisation d'une structure pour stocker les informations du client
        struct sockaddr_in from;
        //Initialisation/Reinitilisation du buffer stockant le message recu
        char buffer[65535]="";

        //Reception du message du client
        if((recvfrom(sock, buffer, sizeof buffer, 0, (struct sockaddr *)&from, &len)) <= 0) {
            perror("recvfrom()");
            exit(1);
        } else {
            // utilisation de getnameinfo pour obtenir le nom de domaine lié à l'IP du client
            getnameinfo((struct sockaddr*)&from, len, host, sizeof host, service, sizeof service, 0);
            if (strlen(host)!=0) {
                printf("CLIENT: %s:%d (%s)\n", inet_ntoa(from.sin_addr) , ntohs(from.sin_port), host);
            } else {
                //si le nom de domaine n'a pas été renseigné par getnameinfo, on affiche l'adresse IP à la place
                printf("CLIENT: %s:%d (%s)\n", inet_ntoa(from.sin_addr) , ntohs(from.sin_port), inet_ntoa(from.sin_addr));
            }

        }
        //Creation du message de réponse au client a partir du message recu
        char* answer=createmessage(buffer);
        //Envoi de la réponse
        if(sendto(sock, answer, (strlen(answer)), 0, (struct sockaddr *)&from, len) < 0)
        {
            perror("sendto()");
            exit(1);
        }
        //réinitialisation de answer
        answer="";
        close(sock);
    }
}