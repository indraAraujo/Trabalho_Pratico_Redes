// Trabalho Pratico - Redes
// Douglas Aquino e Indra Rani Araujo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>

/*
    limite máximo do tamanho do buffer que o servidor deve ter 
    para conseguir armazenar a maior requisição do cliente e o 
    seu maior retorno 
*/
#define MAXBUF 1024

#define quantidade_urls 6
#define tamanho_urls 100

char urls[quantidade_urls][tamanho_urls] = {{"https://i.pinimg.com/474x/92/d3/e9/92d3e94d587d6626b2b8f17162ec3cde.jpg"}, 
                                                {"https://i.pinimg.com/originals/b3/ff/3a/b3ff3accc46377d013a80eb9519c8c1c.jpg"}
                                            };
 
typedef struct mensagem_pedido {
    char metodo[7]; 
    char url[100];
    char versao[3];
}mensagem_pedido;

mensagem_pedido mensagem_cliente; 

typedef struct mensagem_resposta {
    char codigo_estado[7];
    char versao[3];
    char frase[20];
    char dado[100];
}mensagem_resposta;

mensagem_resposta mensagem_servidor;

void metodo_get(char posicao[]){
    int posicao_url = atoi(posicao);
    if(posicao_url > quantidade_urls){
        strcpy(mensagem_servidor.codigo_estado, "404");
        strcpy(mensagem_servidor.frase, "Não foi possível encontrar este endereço");
    }else{
        strcpy(mensagem_servidor.dado, urls[posicao_url]);
        strcpy(mensagem_servidor.codigo_estado, "200");
        strcpy(mensagem_servidor.frase, "OK");
    }
}

void ler_mensagem(char mensagem[]){
    char msg[strlen(mensagem)];
    strcpy(msg,mensagem);
    printf("%s\n",msg);
/*
    char *tk = strtok(msg, ";");
    
    strcpy(mensagem_cliente.metodo, tk);
    tk = strtok(NULL,";");// Passa para a próxima parte da mensagem
    strcpy(mensagem_cliente.url, tk);
    tk = strtok(NULL,";");
    strcpy(mensagem_cliente.versao, tk);

    printf("Metodo: %s\nURL: %s\nVersão: %s\n",mensagem_cliente.metodo,mensagem_cliente.url,mensagem_cliente.versao);

    //Utilizando a mesma versão do cliente e repassando essa informação para a mensagem
    stpcpy(mensagem_servidor.versao, mensagem_cliente.versao);
*/
}

const char* escrever_mensagem (){
    char mensagem[200] = "main.html"; 
    /*
    strcat(mensagem, mensagem_servidor.versao);
    strcat(mensagem, mensagem_servidor.codigo_estado);
    strcat(mensagem,mensagem_servidor.frase);
    strcat(mensagem, mensagem_servidor.dado);
    */

    return mensagem;
}

void metodo(mensagem_pedido mensagem){
    if(strcmp(mensagem.metodo, "GET")){
        metodo_get(mensagem.url);
    }else{
        strcpy(mensagem_servidor.codigo_estado, "400");
        strcpy(mensagem_servidor.frase, "Erro de sintaxe");
    }
}

void TESTES(){
    //printf("Testes ok\n");
    ler_mensagem("GET;https://i.pinimg.com/474x/92/d3/e9/92d3e94d587d6626b2b8f17162ec3cde.jpg;1.1");
}

int main(){
    
    
    int s;          //Socket
    int client_s;   //Socket do cliente
    struct sockaddr_in self;    //Estrutura de dados da biblioteca <netinet/in.h> - Servidor
    struct sockaddr_in client;  //Estrutura de dados da biblioteca <ne                      tinet/in.h> - Cliente
    int addrlen = sizeof(client);
    char msg_write[200], msg_read[200];
    
    s = socket(AF_INET, SOCK_STREAM, 0);    //Cria socket (AF_INET: ipv4; SOCK_STREAM: TCP; 0: IP)

    bzero(&self, sizeof(self));             //Zera tudo que está no self (\0)
    
    self.sin_family = AF_INET;
    self.sin_port = htons(9999);
    self.sin_addr.s_addr = INADDR_ANY;
    
    //printf("Antes do listen");
    bind(s, (struct sockaddr*)&self, sizeof(self));     //Sei lá, não entendi
    listen(s, 5);                                       /* s é o socket marcado pra ficar esperando uma conexão
                                                           e 5 é o tamanho maximo da fila de conexões pendentes
                                                        */
    while (1) {
        client_s = accept(s, (struct sockaddr*)&client, &addrlen);
        printf("cliente: %ls",&client_s);

        read (client_s, msg_read, MAXBUF);
        ler_mensagem(msg_read);
        metodo(mensagem_cliente);
        strcpy(msg_write, escrever_mensagem());
        write (client_s, msg_write, strlen(msg_write)+1);
        close(client_s);
    }
    
    return 0;
    
}

