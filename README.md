# Trabalho_Pratico_Redes

# Discentes: - Douglas Aquino T. Mendes
#            - Indra Rani V. A. dos Santos

O trabalho prático será realizado em dupla, desenvolvido inteiramente na linguagem de programação C com Pthreads, utilizando um ambiente Linux. A organização das atividades previstas será por meio da metodologia SCRUM. Como ferramenta de gestão de projeto, utilizaremos o NOTION, onde será organizado e definido os papéis dos membros nas atividades previstas. Para o versionamento dos códigos será utilizado git na plataforma do GitHub.

- - VERSÕES:

O processo de desenvolvimento deve prever três entregas de versões - três versões de
Produtos Mínimos Viáveis (Minimum Viable Product – MVP) - com intervalos de 14 dias:

• A primeira versão deve corresponder a um servidor capaz de:
- tratar conexões concorrentes (vários clientes simultaneamente),
- seguindo o protocolo HTTP 1.0 (conexões não persistentes);

• A segunda versão deve:
- seguir o protocolo HTTP 1.1
(incorporar a capacidade de tratamento de conexões persistentes e pipelined);

• A terceira versão deve:
- incorporar os mecanismos de Qualidade de Serviço (Quality of Service - QoS) previstos.

- - Detalhamentos: 

enviar objetos (exceto arquivo html) com taxa controlada dependendo da origem da
requisição (conforme arquivo auxiliar contendo IP do cliente e a taxa máxima, em
kbps, que deve ser usada para atendê-lo).
Obs.: caso o IP não esteja cadastrado, atender com taxa de 1000 kbps, se viável;

apresentar visualmente, em tempo real, os clientes que estão sendo atendidos,
informando uma estimativa de atraso fim-a-fim e largura de banda do cliente
(usando estimativa de tempo baseada no Round-Trip-Time (RTT), obtida a partir de
duas requisições consecutivas do mesmo cliente para referentes a uma página
web – solicitação do html e a solicitação do primeiro objeto referenciado neste
html).
Cuidado: browsers podem não solicitar as imagens;

implementar controle de admissão, atendendo um total de clientes simultâneos que
não exceda a vazão máxima do servidor (informada como parâmetro na execução
da ferramenta).
Detalhe: várias conexões simultâneas de um mesmo endereço IP devem dividir a taxa
pré-definida para este endereço.

Os alunos deverão analisar o comportamento da aplicação com múltiplos browsers (ou
outras ferramentas capazes de requisitar objetos por meio do protocolo HTTP),
acessando concorrentemente uma página web (arquivo html e vários arquivos
referenciados no html) no servidor, página esta que deve conter múltiplas imagens com
vários megabytes de tamanho cada.

A execução do servidor deve ser feita preferencialmente em um ambiente experimental
que permita emular múltiplas requisições concorrentes a partir de diferentes hosts
(virtualizados ou sem virtualização), fazendo uso de ferramentas como o wget.
No processo de análise, deverão ser usadas as ferramentas de análise de protocolos
Ethereal (Wireshark), TCPdump e IPtraf. Além disso, deverão, preferencialmente, ser
realizados experimentos em dois cenários: um onde os clientes (browsers) acessem o
servidor através de uma rede cabeada (Ethernet IEEE 802.3) e outro através de rede sem
fio (WiFi IEEE 802.11).