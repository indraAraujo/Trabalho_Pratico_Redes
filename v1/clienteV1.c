// Trabalho Pratico - Redes
// Douglas Aquino e Indra Rani Araujo

//----------------------- CLIENTE -----------------------
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //biblioteca da API de socket
#include <netinet/in.h> //biblioteca com a estrutura
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

/*
    limite máximo do tamanho do buffer que o cliente deve ter 
    para conseguir armazenar a maior requisição e o maior 
    retorno do servidor
*/
#define MAXBUF 1024

//endereço do IP
#define endereco_IP "127.0.0.1"

int main(){
    //armazena o file descriptor retornado pela função socket 
    int new_socket=0; 
    //endereço para o endereço destino (endereço do servidor)
    struct sockaddr_in endereco_destino;
    //mensagens de escrita e leitura 
    char mensagem_write[200], mensagem_read[200];

    /*
        chama a função socket(domínio, tipo, protocolo) da biblioteca
        AF_INET: endereço da internet
        SOCK_STREMA: conexão duplex com transmissão de bytes
        0: protocolo não especificado padrão que vai ser escolhido pela função
            como o mais apropriado
    */
    new_socket = socket(AF_INET, SOCK_STREAM, 0); // tá dando certo
  
    //valida se o scoket foi criado com sucesso
    if(new_socket>0){ //foi criado com sucesso
        /*
            zera na memória as posições aonde está alocado 
            o endereço de destino, para não usar nenhum
            "lixo"
            bzero(posição inicial da variável, tamanho da variável)
        */
        bzero(&endereco_destino, sizeof(endereco_destino));

        /*
            atribui o endereço do tipo de domínio
            para a variável de família do endereço
            AF_INET: endereço da internet
        */
        endereco_destino.sin_family = AF_INET;
        /*
            atribui uma porta  para a variável
            de porta do endereço
            htons: função para converter um inteiro para uma ordem de rede
            9999: porta que o servidor está
        */
        endereco_destino.sin_port = htons(PORT);

        /*
            função que converte um endereço IP de números e pontos
            em um endereço do tipo s_addr
        */
        inet_pton(AF_INET, endereco_IP, &endereco_destino.sin_addr);

        /*
            função para iniciar a conexão com o servidor
            connect(socket, endenreço do socket, tamanho do endereço do socket)
        */
        connect(new_socket, (struct sockaddr*)&endereco_destino, sizeof(endereco_destino)); //dando certo
        
        //recebe o que o cliente quer enviar
        //scanf("%s", mensagem_write);
        strcpy(mensagem_write,"pa v o q acontece");
        printf("Mensagem a ser enviada: %s\n", mensagem_write);

       
            
            /*chama a função de escrever dados na conexão criada
            write(socket, mensagem, tamanho da mensagem)*/
            
        send (new_socket, mensagem_write, strlen(mensagem_write)+1, MSG_OOB);
        
        /*
            chama a função de ler dados da conexão criada
            read(socket, variável para armazenar a mensagem, tamanho máximo)
        */
        recv (new_socket, mensagem_read, MAXBUF, MSG_OOB);

        printf("\nRetorno do servidor: \n");
        printf("%s\n",mensagem_read);
        
        //chama a função para fechar a conexão criada
        close(new_socket);


    }else{ //socket não foi criado com sucesso
        printf("\n Opa! \n Parece que não foi possível criar um socket. Tente novamente");
        return 0;
    }

    return 0;
}