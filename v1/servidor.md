//  Fluxo do programa

```c
main (){    

int socket_novo = socket(AF_INET, SOCK_STREAM, 0);
 //"Cria socket    (AF_INET: ipv4; SOCK_STREAM: TCP; 0: IP)"

- atribui um endereço pro cliente saber com o q conectar 
- fica esperando uma requisição
- aceita a requisição

Le_msg() /*Lê a requisição do cliente e separa apenas as informações que nos interessa,
e salva essas informações numa variável global*/

int method = metodo(cliente.metodo) /*Usa a informação salva na variável global 
para informar qual método ser executado, e responde de acordo com o pedido*/

switch(method){
    case 1: //GET -> Cliente está solicitando algo
        
        metodo_get() //verifica se temos o solicitado e retorna o que foi pedido

    break;

    case x: //Outros

    break
}//end switch

} end main;
```

```c
Le_msg(){
    
}
```

```c
int metodo(string NomedoMetodo){ 

if (NomedoMetodo=="GET"){
    return 1;
} 

if (NomedoMetodo=="xxx"){
    return x;
}

}end método;
```

```c
metodo_get(){
    FILE *arquivo = fopen(cliente.url/* Requisição do cliente SEM / */, 'r');
    //"Quando o cliente fizer uma requisição, irá vim no cabeçalho '/solicitação'"
    
    char c; //Usado para ler o arquivo requisitado

    if (arquivo == NULL){
        printf("ERRO 404 - Arquivo não encontrado");
    }else{
        printf("200 - Arquivo aberto!\n");

        do{ //faz a leitura do caracter no arquivo apontado
            c = fgetc(arquivo);
            //salva o caracter lido
            strcat(servidor.dado,c);

        }while (c != EOF);//enquanto não for final de arquivo
        
        strcat(servidor.dado,"\0\n"); //Sinaliza o fim do dado como manda o protocolo
    }

    fclose(arquivo);

}
```
