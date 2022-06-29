// Trabalho Pratico - Redes
// Douglas Aquino e Indra Rani Araujo

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

/*
    limite máximo do tamanho do buffer que o servidor deve ter 
    para conseguir armazenar a maior requisição do cliente e o 
    seu maior retorno 
*/
#define MAXBUF 1024

// tamanho do vetor com os endereços possíveis para retorno de dados
#define quantidade_urls 6
// tamanho máximo dos endereços para retornar ao cliente
#define tamanho_urls 100

//vetor com os endereços possíveis para retorno de dados
char urls[quantidade_urls][tamanho_urls] = {{"https://i.pinimg.com/474x/92/d3/e9/92d3e94d587d6626b2b8f17162ec3cde.jpg"}, 
                                                {"https://i.pinimg.com/originals/b3/ff/3a/b3ff3accc46377d013a80eb9519c8c1c.jpg"}
                                            };
 
//estrutura de mensagem de pedido originária do cliente
typedef struct mensagem_pedido {
    char metodo[7]; 
    char url[100];
    char versao[3];
}mensagem_pedido;

// mensagem do cliente recebida
mensagem_pedido mensagem_cliente; 

//estrutura de mensagem de resposta do servidor
typedef struct mensagem_resposta {
    char codigo_estado[7];
    char versao[3];
    char frase[20];
    char dado[100];
}mensagem_resposta;

// mensagem do servidor a ser enviada
mensagem_resposta mensagem_servidor;

//execução do método GET do pedido
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

//leitura de cada parte da string com a mensagem de pedido do cliente
void ler_mensagem(char mensagem[]){

    char msg[strlen(mensagem)];
    strcpy(msg,mensagem);
    printf("%s\n",msg);

    char *tk = strtok(msg,";");
    
    strcpy(mensagem_cliente.metodo, tk);
    tk = strtok(NULL,";");// Passa para a próxima parte da mensagem
    strcpy(mensagem_cliente.url, tk);
    tk = strtok(NULL,";");
    strcpy(mensagem_cliente.versao, tk);

    printf("Metodo: %s\nURL: %s\nVersão: %s\n",mensagem_cliente.metodo,mensagem_cliente.url,mensagem_cliente.versao);

    //Utilizando a mesma versão do cliente e repassando essa informação para a mensagem
    stpcpy(mensagem_servidor.versao, mensagem_cliente.versao);

}

//criação da mensagem de resposta do servidor
const char* escrever_mensagem (){
    char mensagem[200]; 

    strcat(mensagem, mensagem_servidor.versao);
    strcat(mensagem, mensagem_servidor.codigo_estado);
    strcat(mensagem,mensagem_servidor.frase);
    strcat(mensagem, mensagem_servidor.dado);

    return mensagem;
}

//decisão sobre qual método deve ser usado de acordo com a mensagem de pedido
void metodo(mensagem_pedido mensagem){
    if(strcmp(mensagem.metodo, "GET")==0){
        metodo_get(mensagem.url);
    }else{
        strcpy(mensagem_servidor.codigo_estado, "400");
        strcpy(mensagem_servidor.frase, "Erro de sintaxe");
    }
}


int main(){
    //armazena o file descriptor retornado pela função socket do servidor
    int socket_novo;          
    //armazena o file descriptor retornado pela função socket do cliente
    int socket_cliente;   //Socket do cliente
    //Estrutura de dados da biblioteca <netinet/in.h> - Servidor
    struct sockaddr_in endereco_servidor;   
    //Estrutura de dados da biblioteca <netinet/in.h> - Cliente 
    struct sockaddr_in endereco_cliente;  
    //tamanho do endereço do socket do cliente
    int tamanho_endereco = sizeof(endereco_cliente);
    //mensagens de escrita e leitura 
    char mensagem_write[200], mensagem_read[200];

    /*
        chama a função socket(domínio, tipo, protocolo) da biblioteca
        AF_INET: endereço da internet
        SOCK_STREMA: conexão duplex com transmissão de bytes
        0: protocolo não especificado padrão que vai ser escolhido pela função
            como o mais apropriado
    */
    socket_novo = socket(AF_INET, SOCK_STREAM, 0);    //Cria socket (AF_INET: ipv4; SOCK_STREAM: TCP; 0: IP)

    bzero(&endereco_servidor, sizeof(endereco_servidor));             //Zera tudo que está no self (\0)
    
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(PORT);
    endereco_servidor.sin_addr.s_addr = INADDR_ANY;
    
    bind(socket_novo, (struct sockaddr*)&endereco_servidor, sizeof(endereco_servidor));  
    listen(socket_novo, 5);    


    while(1){
        socket_cliente = accept(socket_novo, (struct sockaddr*)&endereco_servidor, (socklen_t*)&tamanho_endereco);
        printf("Cliente conectado: %ls \n", &socket_cliente);
        recv(socket_cliente, mensagem_read, MAXBUF, MSG_OOB);
        printf("Mensagem recebida do cliente: %s \n", mensagem_read);
        ler_mensagem(mensagem_read);
        metodo(mensagem_cliente);
        strcpy(mensagem_write, escrever_mensagem());
        send(socket_cliente, mensagem_write, strlen(mensagem_write)+1, MSG_OOB);
        close(socket_cliente);
    }
    
    

    
    return 0;
    
}