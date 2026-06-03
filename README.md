Projeto WAR Estruturado
Sobre o Projeto

O Projeto WAR Estruturado é uma implementação simplificada do clássico jogo de estratégia WAR, desenvolvida em C/C++. O projeto foi criado com foco na prática de conceitos fundamentais de programação, como modularização, uso de funções, ponteiros, estruturas de dados, alocação dinâmica de memória e lógica de jogos.

Durante a partida, o jogador recebe uma missão estratégica aleatória e deve cumprir seu objetivo através da conquista e defesa de territórios.

Funcionalidades
Mapa composto por 5 territórios.
Sistema de exércitos com cores diferentes.
Distribuição aleatória de tropas.
Sistema de batalhas baseado em rolagem de dados.
Conquista de territórios.
Sistema de missões estratégicas.
Verificação automática de cumprimento das missões.
Reinício da partida quando um exército domina o mapa.
Arquivos do Repositório
Arquivo	Descrição
Jogo_war_completo.cpp	Código-fonte completo do projeto
Jogo_war_completo.exe	Executável do jogo para Windows
README.md	Documentação do projeto
Como Executar
Utilizando o Executável
Baixe o arquivo Jogo_war_completo.exe.
Execute o arquivo.
Utilize o terminal para interagir com o jogo.
Compilando o Código-Fonte

Caso deseje compilar o projeto manualmente:

g++ Jogo_war_completo.cpp -o Jogo_war_completo

Para executar:

./Jogo_war_completo

No Windows:

Jogo_war_completo.exe
Estruturas Utilizadas
Território
struct territorio
{
    char nome[90];
    char cor[10];
    int tropas;
};
Missões
struct Missoes
{
    int ID;
    char ExercitoEscolhido[90];
    char objetivo[90];
    char sofredor[90];
};
Missões Disponíveis

O jogo pode sortear uma das seguintes missões:

Conquistar 3 territórios.
Atacar 2 territórios.
Conquistar um continente específico.
Eliminar todas as tropas de um exército específico.
Defender um território por 3 rodadas.
Conceitos Aplicados
Programação Estruturada
Modularização de Código
Ponteiros
Structs
Alocação Dinâmica de Memória
Manipulação de Strings
Geração de Números Aleatórios
Controle de Fluxo
Simulação de Jogos
Tecnologias Utilizadas
C/C++
Biblioteca stdio.h
Biblioteca stdlib.h
Biblioteca string.h
Biblioteca time.h
Autor

Projeto desenvolvido para fins acadêmicos com o objetivo de praticar conceitos fundamentais de programação e desenvolvimento de jogos em console.
