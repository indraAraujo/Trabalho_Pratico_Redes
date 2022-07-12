/*  Trabalho Pratico - Redes 2022/1
        Douglas Aquino      - 1901570782
        Indra Rani Araujo   - 190
*/

//                  BIBLIOTECAS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>

//                  VARIAVEIS GLOBAIS
#define PORT 8080                   //
struct sockaddr_in endereco_server; //
struct sockaddr_in endereco_cliente;//
int socket_cliente;
int socket_server;

/*
    Limite máximo do tamanho do buffer que o servidor deve ter 
    para conseguir armazenar a maior requisição do cliente e o 
    seu maior retorno 
*/#define MAXBUF 1024               //

//mensagens de escrita e leitura 
char mensagem_write[MAXBUF];        //
char mensagem_read[MAXBUF];         //
char *mensagem;

//Estrutura de mensagem de pedido do cliente
typedef struct mensagem_pedido{
    char metodo[10]; 
    char url[100];
    char versao[10];
}mensagem_pedido;

//Estrutura de mensagem de resposta do servidor
typedef struct mensagem_resposta {
    char codigo_estado[3];
    char versao[10];
    char frase[50];
    char *dado;
}mensagem_resposta;

// mensagem do servidor a ser enviada
mensagem_resposta mensagem_servidor;
// mensagem do cliente recebida
mensagem_pedido mensagem_cliente; 


//                  FUNÇÕES E PROCEDIMENTOS
void *atribuirCliente(); //Inicia o tratamento do cliente em uma thread do servidor
void ler_mensagem(char mensagem[]);     //Separa o que nos interessa da mensagem do cliente
int metodo(mensagem_pedido mensagem);   //Seleciona o método ideal para resposta
void metodo_get(char solicitacao[]);    //Método get
void escrever_mensagem();       //Monta, baseada na estrutura do protocolo, a mensagem a ser enviada pro cliente
void *atribuirCliente ();

int main(int argc, char *argv[]){
   pthread_t thread_id;
//                  CRIAÇÃO DO SOCKET
     socket_server = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_server==-1){
            perror("[!] ERRO AO CRIAR O SOCKET\n");
            close(socket_server);
            exit(1);
        }else{
            printf("Socket criado com sucesso! :D\n");
        }

//                  ADICIONANDO ENDEREÇO
    endereco_server.sin_family = AF_INET;
    endereco_server.sin_addr.s_addr = INADDR_ANY;   //localhost
    endereco_server.sin_port = htons(PORT);
        if(bind(socket_server, (struct sockaddr *) &endereco_server, sizeof(endereco_server))<0){
            perror("[!] BIND() ERROR \n");
            close(socket_server);
            exit(1);
        } else {
            printf("Endereço adicionado com sucesso! :D\n");
        }

//                  AGUARDA UM CLIENTE
    listen(socket_server, 2);
        printf("AGUARDANDO CONEXÃO . . .\n");
    
//                  CONECTANDO . . .
    int tamanho = sizeof(struct sockaddr_in);
    

    while ((socket_cliente = accept(socket_server, (struct sockaddr *)&endereco_cliente, (socklen_t *)&tamanho))){
//  Aceita conexões e salva em um novo socket
        if (socket_cliente < 0){
            perror("[!] ERRO AO ACEITAR CONEXÃO\n");
            close(socket_server);
            exit(1);
        } else {
            printf("Conexão aceita com sucesso! :D\nCliente: %i\n",socket_cliente);
            //printf("Número de conexões: %i",);
            pthread_create(&thread_id, NULL, atribuirCliente, NULL);
        }
    }
    printf("ENCERRANDO SERVIDOR . . . \n\n");
    close(socket_server);

    return 0;
}

void *atribuirCliente (){
    printf("NOVA THREAD DO SERVIDOR CRIADA");

    memset(mensagem_read, 0x0, MAXBUF);
            int receive_result = recv(socket_cliente, mensagem_read, MAXBUF, 0);
                if (receive_result < 0){
                    perror("[!] MENSAGEM NÃO RECEBIDA\n");
                    close(socket_server);
                    exit(1);
                } else {
                    printf("Mensagem recebida com sucesso! :D\n\n");

                    //mensagem_read[receive_result] = '\0';
                    printf("Mensagem recebida do cliente:\n\n");
                    ler_mensagem(mensagem_read);

                    int tipo_metodo;
                    tipo_metodo = metodo(mensagem_cliente);
                    /*  Retorna     0   para    erro
                        Retorna     1   para    GET
                        Retorna    <X>  para    <OUTRO METODO>
                    */
                    if (tipo_metodo!=0){
                        printf("Metodo executado com sucesso! :D\n");

                        switch (tipo_metodo){
                            case 1:
                                metodo_get(mensagem_cliente.url);
                                    printf("GET executado com sucesso!\n");
                            break;
                            
                            default:
                            break;
                            }
                    } else {
                        perror("[!] ERRO AO EXECUTAR O MÉTODO SOLICITADO!\n");
                        close(socket_server);
                        close(socket_cliente);
                        exit(1);
                    }
                    escrever_mensagem();
                    /*if(strlen(mensagem)>1023){
                        printf("Atenção, Mensagem grande de mais.. será nescessário repartir o envio\n\n");
                    }
                    
                        printf("Mensagem a ser enviada para o cliente:\n\n%s\n",mensagem_write);
                    */ // <-- Está dando SEGMENTATION FAULT

                    send(socket_cliente,mensagem, strlen(mensagem)+1, 0);
                    
                    close(socket_cliente);
                }
    pthread_exit(NULL);
}

void ler_mensagem(char mensagem[]){
    char msg[strlen(mensagem)]; //Cria uma string do tamanho nescessário
    strcpy(msg,mensagem);       //Copia a mensagem do cliente
    printf("%s\n",msg);         //

    char *tk = strtok(msg," ");
    
    strcpy(mensagem_cliente.metodo, tk);
    tk = strtok(NULL," ");// Passa para a próxima parte da mensagem
    strcpy(mensagem_cliente.url, tk);
    tk = strtok(NULL,"\n");
    strcpy(mensagem_cliente.versao, tk);

    printf("Metodo: %s\nURL: %s\nVersão: %s\n\n",mensagem_cliente.metodo,mensagem_cliente.url,mensagem_cliente.versao);
    
    stpcpy(mensagem_servidor.versao, mensagem_cliente.versao);

    printf("Versão no servidor: %s\n\n",mensagem_servidor.versao);
    
}

int metodo(mensagem_pedido mensagem){
    if (strcmp(mensagem.metodo, "GET")==0){
        printf("O METODO É UM GET!\n");
        strcpy(mensagem_servidor.codigo_estado, "200 ");
        strcpy(mensagem_servidor.frase, "OK ");
        return 1;
    } else {
        strcpy(mensagem_servidor.codigo_estado, "400 ");
        strcpy(mensagem_servidor.frase, "Erro de sintaxe ");
        printf("[!] ERRO: MÉTODO NÃO IDENTIFICADO\n");
        return 0;
    }
}

void metodo_get(char solicitacao[]){

    char linha[100];

    printf("Solicitação: %s\n",solicitacao);

    char nome_do_arquivo[strlen(solicitacao-1)];    //Cria uma string de tamanho adequado
    strcpy(nome_do_arquivo,&solicitacao[1]);        //Copia a solicitação SEM '/'
        printf("URL: %s\n",nome_do_arquivo);

    if (strcmp(solicitacao,"/")==0){
        printf("Solicitação da main pela localhost\n");
        FILE *arquivo = fopen("main.html","r");
            if(arquivo == NULL){
                printf("[!] ERRO AO ABRIR O ARQUIVO\n");
            } else {
                printf("Arquivo aberto com sucesso! :D\nARQUIVO:\n\n");
                
                fseek(arquivo, 0, SEEK_END);//Vai para o final do arquivo
                long int res = ftell(arquivo);
                fseek(arquivo, 0, SEEK_SET);//Volta para o início
                printf("Tamanho do Arquino: %li\n",res);

                char conteudo[res];

                while(!feof(arquivo)){
                    fgets(linha,100,arquivo); 
                    printf("%s",linha);
                    strcat(conteudo,linha);
                }
                printf("\n\n");
                fclose(arquivo);
                mensagem_servidor.dado = malloc(sizeof(conteudo));
                mensagem_servidor.dado[0] = '\0';
                strcpy(mensagem_servidor.dado,conteudo);
            }
    } else {
        printf("Solicitação feita em localhost:%i%s\n",PORT,solicitacao);
        /*
        FILE *arquivo;
        char conteudo[strlen(arquivo)];

        //[!] TERMINAR CODIGO PARA SOLICITAÇÃO DE OUTRAS PAGINAS
        
        return conteudo;
        */
    }
}

void escrever_mensagem(){
    mensagem = malloc(sizeof(mensagem_servidor.versao)
    + sizeof(mensagem_servidor.codigo_estado) 
    + sizeof(mensagem_servidor.frase)
    + sizeof("mensagem_servidor.dado\n") );
    mensagem[0] = '\0';

    printf("\nCarta em branco: %s\n",mensagem);
    printf("Informações no SERVIDOR:\n");
    printf("S_VERSÃO: %s\n",mensagem_servidor.versao);
    printf("S_COD: %s\n",mensagem_servidor.codigo_estado);
    printf("S_STATUS: %s\n\n",mensagem_servidor.frase);

    strcat(mensagem, mensagem_servidor.codigo_estado);
    strcat(mensagem, "HTTP/1.1 ");
    strcat(mensagem, mensagem_servidor.frase);
    strcat(mensagem, mensagem_servidor.dado);
    mensagem[strlen(mensagem)]='\0';

    printf("\nMensagem preparada para o envio: %s\n- - - END MENSAGEM - - -\n",mensagem);
} 