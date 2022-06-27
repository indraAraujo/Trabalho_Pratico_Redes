// Trabalho Pratico - Redes
// Douglas Aquino e Indra Rani Araujo

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

/*
    limite máximo do tamanho do buffer que o servidor deve ter 
    para conseguir armazenar a maior requisição do cliente e o 
    seu maior retorno 
*/
#define MAXBUF 1024

int main(){
    //printf("- - - Iniciando servidor... [!]");
    int s;          //Socket
    int client_s;   //Socket do cliente
    struct sockaddr_in self;    //Estrutura de dados da biblioteca <netinet/in.h> - Servidor
    struct sockaddr_in client;  //Estrutura de dados da biblioteca <netinet/in.h> - Cliente
    int addrlen = sizeof(client);
    char msg_write[100], msg_read[100];
    
    s = socket(AF_INET, SOCK_STREAM, 0);    //Cria socket (AF_INET: ipv4; SOCK_STREAM: TCP; 0: IP)

    bzero(&self, sizeof(self));             //Zera tudo que está no self (\0)
    
    self.sin_family = AF_INET;
    self.sin_port = htons(9999);
    self.sin_addr.s_addr = INADDR_ANY;
    
    //printf("Antes do listen");
    bind(s, (struct sockaddr*)&self, sizeof(self));     //Sei lá, não entendi
    listen(s, 5);                                       /* s é o socket marcado pra ficar esperando uma conexão
                                                         e 5 é o tamanho maximo da fila de conexões pendentes*/
    
    //printf("Antes do while");
    while (1) {
        client_s = accept(s, (struct sockaddr*)&client, &addrlen);
        printf("cliente: %ls",&client_s);

        read (client_s, msg_read, MAXBUF);
        write (client_s, msg_read, strlen(msg_read)+1);
    
        close(client_s);
    }
    
}

