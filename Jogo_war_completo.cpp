// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MaxTerritorios 5
#define MaxString 90
#define Maxdes 100
#define MaxMissoes 5

int RodadasDefendidas = 0;
int RodadasAtaque = 0;
//variavel que impede aparecer o aviso quando nao solicitado 
int naoaparecer = 0;


// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct territorio 
{
	char nome [MaxString];
	char cor[10];
	int tropas;
}; 

//Define a estrutura para a missão, contendo ID, exercito que sera atribuido a missão, objetivo e quem sofrera a missão se tiver um especifico 
struct Missoes
{
	int ID;
	char ExercitoEscolhido[MaxString];
	char objetivo[MaxString];
	char sofredor[MaxString];

};

//Vetor de string de missões
const char missoesEstrategicas[MaxMissoes][Maxdes] = 
{
	"Conquistar 3 territorios",
    "Ataque 2 territorios",
    "Conquistar todos os territorios do continente escolhido",
    "Reduzir tropas do exercito escolhido para zero",
    "Defender seu territorio por 3 rodadas"
};

//Avisa que esses void existem 
void LimparBuffer();
void exibirMenuPrincipal(int *Opcao);
struct territorio* alocarMapa();
struct Missoes* alocarMissoes();
void inicializarTerritorios(struct territorio *Territorios, int *totalterritorios);
void Mapa (struct territorio *Territorios, int *totalterritorios);
void FasedeAtaque(struct territorio *Territorios, struct Missoes *missao, int *missaocumprida);
void simularAtaque(struct territorio *Territorios, struct Missoes *missao, int defensor, int atacante);
void atribuirMissao(struct territorio *Territorios,struct Missoes *missoesestrategicas, int totalMissoes);
void exibirMisao(struct Missoes *missao, struct territorio *Territorios, int *MissaoCumprida);
void verificarVitoria(struct territorio *Territorios,struct Missoes *missostruct,int *MissaoCumprida);
void liberarMemoria(struct territorio *Territorios, int totalTerritorios, struct Missoes *missoes);
//Se caso um exercito conquistar todos os territorios, o sistema pergunta se quer reiniciar ou sair do jogo
void verificarReiniciar(struct territorio *Territorios,int *opcao,int *MissaoCumprida, int *reiniciar);


// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:


// --- Implementação das Funções ---

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void LimparBuffer ()
{
	int c;
	while((c=getchar()) != '\n' && c != EOF);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal(int *Opcao)
{	
	printf("-----MENU DE ACOES-----\n");
	printf("1. Atacar\n");
	printf("2. Verificar Missao\n");
	printf("3. Sair\n");
	printf("Escolha sua acao: \n");
	scanf("%d", Opcao);
	LimparBuffer();
	
}


// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
struct territorio* alocarMapa()
{
	return (struct territorio*) calloc(MaxTerritorios, sizeof(struct territorio));
}

struct Missoes* alocarMissoes()
{
	return (struct Missoes*) malloc(MaxMissoes * sizeof(struct Missoes));
}


// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(struct territorio *Territorios, int *totalterritorios)
{
	const char *nomes[MaxTerritorios]  = {
    "America", "Europa", "Africa", "Asia", "Oceania"
    };
    const char *cores[MaxTerritorios]  = { "Azul", "Vermelho", "Verde", "Amarelo", "Laranja" };

	
    for (int i = 0; i < MaxTerritorios; i++) {
    
		strncpy(Territorios[i].nome, nomes[i], MaxString - 1);
        Territorios[i].nome[MaxString - 1] = '\0';

        strncpy(Territorios[i].cor, cores[i], sizeof(Territorios[i].cor) - 1);
        Territorios[i].cor[sizeof(Territorios[i].cor) - 1] = '\0';

    	
		int tropasPossiveis[MaxTerritorios];
		for (int i = 0; i < MaxTerritorios; i++) {
    		tropasPossiveis[i] = i + 1;
		}

		
		for (int i = MaxTerritorios - 1; i > 0; i--) {
    		int j = rand() % (i + 1);
    		int temp = tropasPossiveis[i];
    		tropasPossiveis[i] = tropasPossiveis[j];
    		tropasPossiveis[j] = temp;
		}

		
		for (int i = 0; i < MaxTerritorios; i++) {
    		Territorios[i].tropas = tropasPossiveis[i];
		}
	
	}

    *totalterritorios = MaxTerritorios;
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void Mapa (struct territorio *Territorios, int *totalterritorios)
{
	int TotalTerritorios = *totalterritorios;
	printf("\n================ MAPA DO MUNDO ================\n\n");
	
	for(int i = 0; i < TotalTerritorios; i++)
	{	
		printf("%-1d. %-10s (Exercito: %-10s, Tropas: %-1d)\n", 
               i + 1, 
               Territorios[i].nome, 
               Territorios[i].cor, 
               Territorios[i].tropas);
	}
	printf("\n");
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void FasedeAtaque (struct territorio *Territorios, struct Missoes *missao, int *missaocumprida)
{
	int atacante = 0;
	int defensor = 0;
	printf("----- FASE DE ATAQUE -----\n\n");
	printf("Escolha o territorio atacante (1 a 5): ");
	scanf("%d", &atacante);
	LimparBuffer();
	if (atacante < 1 || atacante > 5 )
	{
		printf("\n\nValor invalido!\n");
		printf("você será levado a fase de ataque novamente\n\n");
		printf("Digite ENTER para continuar...\n");
		getchar();
		FasedeAtaque(Territorios, missao, missaocumprida);
		return;
	}
	if(*missaocumprida != 1)
	{
		if (strcmp(missao[0].objetivo, "Conquistar") == 0 || strcmp(missao[0].objetivo, "Ataque") == 0 || strcmp(missao[0].objetivo, "Conquistar continente") == 0 || strcmp(missao[0].objetivo, "Eliminar tropas do exercito") == 0)
		{
			if(strcmp(Territorios[atacante - 1].cor, missao[0].ExercitoEscolhido) != 0)
			{
				int resposta;
				printf("\n\nVoce nao esta usando o exercito para missao, deseja continuar assim?\n");
				printf("1 - SIM \n");
				printf("2 - NAO \n\n");
				printf("Digite sua escolha: ");
				scanf("%d", &resposta);
		    	LimparBuffer();
				switch (resposta)
				{
				case 1:
					printf("\nDigite ENTER para continuar...\n");
					getchar();
				break;
				case 2:
					printf("\nvoce sera levado a fase de ataque novamente\n");
					printf("Digite ENTER para continuar...\n");
					getchar();
					FasedeAtaque(Territorios, missao, missaocumprida);
					return;
				break;
				default:
					printf("\nValor invalido!\n");
					printf("\nvoce sera levado a fase de ataque novamente\n");
					printf("Digite ENTER para continuar...\n");
					getchar();
					FasedeAtaque(Territorios, missao, missaocumprida);
					return;
				break;
				}			

			}
	
		}
	}
	if (atacante != 0)
	{
		printf("\nEscolha o territorio defensor (1 a 5): ");
		scanf("%d", &defensor);
		LimparBuffer();
		if (defensor<1 || defensor > 5 || atacante == defensor || strcmp(Territorios[atacante - 1].cor, Territorios[defensor - 1].cor) == 0)
		{
			printf("\n\nValor invalido!\n");
			printf("voce sera levado a fase de ataque novamente\n\n");
			printf("Digite ENTER para continuar...\n");
			getchar();
			FasedeAtaque(Territorios, missao, missaocumprida);
			return;
		}
		else 
		{
			if(*missaocumprida != 1)
			{
				if (strcmp(missao[0].objetivo, "Conquistar") == 0 || strcmp(missao[0].objetivo, "Defender territorio por 3 rodadas") == 0 || strcmp(missao[0].objetivo, "Conquistar continente") == 0 || strcmp(missao[0].objetivo, "Eliminar tropas do exercito") == 0)
				{
					if(strcmp(Territorios[defensor - 1].cor, missao[0].ExercitoEscolhido) != 0)
					{
						if (strcmp(missao[0].objetivo, "Defender território por 3 rodadas") == 0)
						{
							int resposta;
							printf("\n\nVoce nao esta usando o exercito para missao, deseja continuar assim?\n");
							printf("1 - SIM \n");
							printf("2 - NAO \n\n");
							printf("Digite sua escolha: ");
							scanf("%d", &resposta);
		    				LimparBuffer();
							switch (resposta)
							{
							case 1:
								printf("\nDigite ENTER para continuar...\n");
								getchar();
							break;
							case 2:
								printf("\nvoce sera levado a fase de ataque novamente\n");
								printf("Digite ENTER para continuar...\n");
								getchar();
								FasedeAtaque(Territorios, missao, missaocumprida);
								return;
							break;
							default:
								printf("\nValor invalido!\n");
								printf("\nvoce sera levado a fase de ataque novamente\n");
								printf("Digite ENTER para continuar...\n");
								getchar();
								FasedeAtaque(Territorios, missao, missaocumprida);
								return;
							break;
							}
						}
					
						else if (strcmp(Territorios[atacante - 1].cor, missao[0].ExercitoEscolhido) != 0)
						{
							int resposta;
							printf("\n\nVoce nao esta usando o exercito para missao, deseja continuar assim?\n");
							printf("1 - SIM \n");
							printf("2 - NAO \n\n");
							printf("Digite sua escolha: ");
							scanf("%d", &resposta);
		    				LimparBuffer();
							switch (resposta)
							{
							case 1:
								printf("\nDigite ENTER para continuar...\n");
								getchar();
							break;
							case 2:
								printf("\nvoce sera levado a fase de ataque novamente\n");
								printf("Digite ENTER para continuar...\n");
								getchar();
								FasedeAtaque(Territorios, missao, missaocumprida);
								return;
							break;
							default:
								printf("\nValor invalido!\n");
								printf("\nvocê será levado a fase de ataque novamente\n");
								printf("Digite ENTER para continuar...\n");
								getchar();
								FasedeAtaque(Territorios, missao, missaocumprida);
								return;
							break;
							}

						}
				
					}
	
				}
			}
			simularAtaque(Territorios, missao, defensor, atacante);
		}
		
	}
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(struct territorio *Territorios, struct Missoes *missao, int defensor, int atacante)
{
	printf("\n\n----- RESULTADO DA BATALHA -----\n\n");
	int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
	printf("O atacante %s rolou um dado e tirou: %d\n", Territorios[atacante - 1].nome, dadoAtacante);
	printf("O defensor %s rolou um dado e tirou: %d\n\n", Territorios[defensor - 1].nome, dadoDefensor);
		
	if(dadoAtacante >= dadoDefensor)
	{
		printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n");
		Territorios[defensor - 1].tropas --;


		if(strcmp(Territorios[atacante - 1].cor, missao[0].ExercitoEscolhido) == 0)
		{
			RodadasAtaque++;
		}
		if(Territorios[defensor - 1].tropas == 0)
		{
			printf("CONQUISTA! O territorio %s foi dominado pelo exercito %s!\n", Territorios[defensor - 1].nome, Territorios[atacante - 1].cor);
			strcpy(Territorios[defensor - 1].cor, Territorios[atacante - 1].cor);
					
					
			if(Territorios[atacante - 1 ].tropas == 1)
			{
				// Se só tem 1, não pode mover, apenas ocupa com 1
				Territorios[defensor - 1].tropas = 1;
			}
			// Em caso de número par, divide igualmente
			else if (Territorios[atacante - 1 ].tropas % 2 == 0)
			{
				int mover = Territorios[atacante - 1 ].tropas / 2;
				Territorios[atacante - 1 ].tropas = mover;
				Territorios[defensor - 1].tropas = mover;
			}
			else
			{
				// Divide arredondando: metade+1 para defensor, metade para atacante
				int mover = (Territorios[atacante - 1].tropas / 2) + 1;  
    			Territorios[defensor - 1].tropas = mover;
    			Territorios[atacante - 1].tropas -= mover;
			}
		}
	}
	else if (dadoDefensor > dadoAtacante)
	{
		printf("VITORIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
		Territorios[atacante - 1].tropas --;
		
		if(strcmp(Territorios[defensor - 1].cor, missao[0].ExercitoEscolhido) == 0)
		{
			RodadasDefendidas++;
		}
		if(Territorios[atacante - 1].tropas == 0)
		{
			printf("CONQUISTA! O territorio %s foi dominado pelo exercito %s!\n", Territorios[atacante - 1].nome, Territorios[defensor - 1].cor);
			strcpy(Territorios[atacante - 1].cor, Territorios[defensor - 1].cor);
					
			if(Territorios[defensor - 1 ].tropas == 1)
			{
				// Se só tem 1, não pode mover, apenas ocupa com 1
				Territorios[atacante - 1].tropas = 1;
			}
			// Em caso de número par, divide igualmente
			else if (Territorios[defensor - 1 ].tropas % 2 == 0)
			{
				int mover = Territorios[defensor - 1 ].tropas / 2;
				Territorios[atacante - 1 ].tropas = mover;
				Territorios[defensor - 1].tropas = mover;
			}
			else
			{
				// Divide arredondando: metade+1 para defensor, metade para atacante
				int mover = (Territorios[defensor - 1].tropas / 2) + 1;  
				Territorios[atacante - 1].tropas = mover;
    			Territorios[defensor - 1].tropas -= mover;
			}
		}
	}
	printf("\n");
	printf("você será levado a proxima fase\n");
	printf("Digite ENTER para continuar...\n");
	getchar();		
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
void atribuirMissao(struct territorio *Territorios,struct Missoes *missostruct, int totalMissoes)
{
    int IDsorteado = rand() % MaxMissoes;
	int territorioescolhido = rand() % MaxTerritorios;

    missostruct[0].ID = IDsorteado;
    strncpy(missostruct[0].ExercitoEscolhido, Territorios[territorioescolhido].cor, MaxString- 1);
    missostruct[0].ExercitoEscolhido[MaxString - 1] = '\0';

    switch (IDsorteado)
    {
    case 0:
	    strncpy(missostruct[0].objetivo, "Conquistar", MaxString- 1);
	    missostruct[0].objetivo[MaxString - 1] = '\0';
	    strncpy(missostruct[0].sofredor, "3 territorios", MaxString - 1);
	
    break;
    case 1:
	    strncpy(missostruct[0].objetivo, "Ataque", MaxString- 1);
	    missostruct[0].objetivo[MaxString - 1] = '\0';
	    strncpy(missostruct[0].sofredor, "2 territorios", MaxString - 1);
	    missostruct[0].sofredor[MaxString-1]='\0';
   break;
	
    case 2:
    {
	    strncpy(missostruct[0].objetivo, "Conquistar continente", MaxString - 1);
    	missostruct[0].objetivo[MaxString - 1] = '\0';
    	int escolhida;

            do {
                escolhida = rand() % MaxTerritorios;
            } while (strcmp(Territorios[escolhida].cor, missostruct[0].ExercitoEscolhido) == 0);
            strncpy(missostruct[0].sofredor, Territorios[escolhida].nome, MaxString - 1);
			missostruct[0].sofredor[MaxString - 1] = '\0';
	    
	break;
    }
    case 3:
    {
	    strncpy(missostruct[0].objetivo, "Eliminar tropas do exercito", MaxString - 1);
	    missostruct[0].objetivo[MaxString - 1] = '\0';
		int escolhido;
	     do {
                escolhido = rand() % MaxTerritorios;
            } while (strcmp(Territorios[escolhido].cor, missostruct[0].ExercitoEscolhido) == 0);
            strncpy(missostruct[0].sofredor, Territorios[escolhido].cor, MaxString - 1);
			missostruct[0].sofredor[MaxString - 1] = '\0';
    break;
    }
    case 4:
	{
	    strncpy(missostruct[0].objetivo, "Defender territorio por 3 rodadas", MaxString - 1);
	    missostruct[0].objetivo[MaxString - 1] = '\0';
	
	    strncpy(missostruct[0].sofredor, "Nenhum", MaxString - 1);
	    missostruct[0].sofredor[MaxString - 1] = '\0';
    break;
    }
	}

}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMisao(struct Missoes *missao, struct territorio *Territorios, int *MissaoCumprida)
{
	int SemJogador =1 ; //verifica se é possivel cumprir a missão, 1 para sem jogador e 0 se há jogador
		
	for (int contador = 0; contador < MaxTerritorios; contador++)
	{
		if(strcmp(Territorios[contador].cor, missao[0].ExercitoEscolhido) == 0)
		{
			SemJogador = 0;
		}
	}

	if(SemJogador != 0 )
	{
		if(*MissaoCumprida != 1)
		{
			printf("\nO territorio escolhido perdeu!\nSera atribuido outra missao\n");
	    	atribuirMissao(Territorios, missao, MaxMissoes);
			RodadasAtaque = 0;
			RodadasDefendidas = 0;
			printf("Digite ENTER para continuar");
			getchar();
		}
		
	}

	naoaparecer = 1;
	if(*MissaoCumprida != 1)
	{
		verificarVitoria(Territorios, missao, MissaoCumprida);
	}
			
	if (*MissaoCumprida == 0)
	{
		printf("---------Sua missao (Exercito %s)---------\n\n", missao[0].ExercitoEscolhido);
		printf(" -> Missão: %s", missao[0].objetivo);
		if (strcmp(missao[0].sofredor, "Nenhum") == 0)
		{
			printf("\n\n");
		}
		else
		{
			printf(" %s\n\n", missao[0].sofredor);
		}
		printf("------------------------------------------\n\n");
	}
	else if(*MissaoCumprida == 1)
	{
		printf("---------Sua missao (Exercito %s)---------\n\n", missao[0].ExercitoEscolhido);
		printf(" -> Missão: %s", missao[0].objetivo);
		if (strcmp(missao[0].sofredor, "Nenhum") == 0)
		{
			printf("\n\n");
		}
		else
		{
			printf(" %s\n\n", missao[0].sofredor);
		}
		printf("PARABENS!!! MISSAO CUMPRIDA!!!\n\n");
		printf("------------------------------------------\n\n");
		}
		
		
	
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
void verificarVitoria(struct territorio *Territorios,struct Missoes *missostruct,int *MissaoCumprida)
{
	int SemJogador = 1; //verifica se é possivel cumprir a missão, 1 para sem jogador e 0 se há jogador
	for (int contador = 0; contador < MaxTerritorios; contador++)
	{
		if(strcmp(Territorios[contador].cor, missostruct[0].ExercitoEscolhido) == 0)
		{
			SemJogador = 0;
		}
	}
    if(SemJogador ==0 || *MissaoCumprida == 1)
	{
	    int conquistados = 0;
	    int derrota = 0;
	    switch (missostruct[0].ID)
	    {
		
	    case 0:
		    for (int contador = 0; contador < MaxTerritorios; contador++)
		    {
			    if(strcmp(Territorios[contador].cor, missostruct[0].ExercitoEscolhido) == 0)
			    {
				    conquistados++;
			    }
		    }
		    if(conquistados >= 4 )
		    {
			    printf("Missao cumprida com sucesso!!\n");
			    *MissaoCumprida = 1;
			    printf("Digite ENTER para continuar");
			    getchar();
			
		    }
		    else
		    {
				if (naoaparecer == 0)
				{
			    	printf("Continue tentando, voce ainda nao cumpriu a missao\n");
			    	printf("Digite ENTER para continuar");
			    	getchar();
				}
				else 
				{
					naoaparecer = 0;
				}
		    }
	    break;
    	case 1:
	    	if(RodadasAtaque >= 2 )
		    {
			    printf("Missao cumprida com sucesso!!\n");
			    *MissaoCumprida = 1;
			    printf("Digite ENTER para continuar");
			    getchar();
			
		    }
		    else
		    {
		 	    if (naoaparecer == 0)
				{
			    	printf("Continue tentando, voce ainda nao cumpriu a missao\n");
			    	printf("Digite ENTER para continuar");
			    	getchar();
				}
				else 
				{
					naoaparecer = 0;
				}
			
		    }
	    break;
	    case 2:
		    for (int contador = 0; contador < MaxTerritorios; contador++)
		    {
			    if(strcmp(Territorios[contador].nome, missostruct[0].sofredor) == 0 && strcmp(Territorios[contador].cor, missostruct[0].ExercitoEscolhido)==0)
			    {
				    conquistados++;
     		    }
		    }
		    if(RodadasAtaque > 0 )
		    {
			    printf("Missao cumprida com sucesso!!\n");
			    *MissaoCumprida = 1;
			    printf("Digite ENTER para continuar");
			    getchar();
		    }
		    else
		    {
			    if (naoaparecer == 0)
				{
			    	printf("Continue tentando, voce ainda nao cumpriu a missao\n");
			    	printf("Digite ENTER para continuar");
			    	getchar();
				}
				else 
				{
					naoaparecer = 0;
				}
			
		    }
	    break;
	    case 3:
	    	
		    for (int contador = 0; contador < MaxTerritorios; contador++)
		    {
			    if(strcmp(Territorios[contador].cor, missostruct[0].sofredor) == 0)
			    {
				    derrota = 1;
			    }
		    }
		    if(derrota != 1 )
		    {
			    printf("Missao cumprida com sucesso!!\n");
			    *MissaoCumprida = 1;
			    printf("Digite ENTER para continuar");
			    getchar();
		    }
		    else
		    {
			    if (naoaparecer == 0)
				{
			    	printf("Continue tentando, voce ainda nao cumpriu a missao\n");
			    	printf("Digite ENTER para continuar");
			    	getchar();
				}
				else 
				{
					naoaparecer = 0;
				}
		    }
	    break;
	
	    case 4:
		    if(RodadasDefendidas >= 3 )
		    {
			    printf("Missao cumprida com sucesso!!\n");
			    *MissaoCumprida = 1;
			    printf("Digite ENTER para continuar");
			    getchar();
		    }
		    else
		    {
			    if (naoaparecer == 0)
				{
			    	printf("Continue tentando, voce ainda nao cumpriu a missao\n");
			    	printf("Digite ENTER para continuar");
			    	getchar();
				}
				else 
				{
					naoaparecer = 0;
				}
			
		    }
	    break;
	
	    default:
		    printf("Error!\n");
		    printf("Digite ENTER para continuar");
		    getchar();
			naoaparecer = 0;
		
	    break;
	    }
	}
	else
	{
	    printf("O territorio escolhido perdeu!\nSera atribuido outra missao\n");
		atribuirMissao(Territorios, missostruct,MaxMissoes);
		RodadasAtaque = 0;
		RodadasDefendidas = 0;
		naoaparecer = 0;
		printf("Digite ENTER para continuar");
		getchar();

	}
}

//Se caso um exercito conquistar todos os territorios, o sistema pergunta se quer reiniciar ou sair do jogo
void verificarReiniciar(struct territorio *Territorios,int *opcao,int *MissaoCumprida, int *reiniciar)
{
	int azul = 0;
	int vermelho = 0;
	int verde = 0;
	int amarelo = 0;
	int laranja = 0;
	for (int contador = 0; contador < MaxTerritorios; contador++)
	{
		
		if(strcmp(Territorios[contador].cor, "Azul") == 0)
		{
			azul++;
		}
		else if(strcmp(Territorios[contador].cor, "Vermelho") == 0)
		{
			vermelho++;
		}
		else if(strcmp(Territorios[contador].cor, "Verde") == 0)
		{
			verde++;
		}
		else if(strcmp(Territorios[contador].cor, "Amarelo") == 0)
		{
			amarelo++;
		}
		else if(strcmp(Territorios[contador].cor, "Laranja") == 0)
		{
			laranja++;
		}

		if (azul == 5 || vermelho == 5 || verde == 5 || amarelo == 5 || laranja == 5)
		{
			int resposta;
			printf("\n\nUm exercito dominou todos os territorios. Deseja reiniciar o jogo ou sair?\n");
			printf("1 - Reiniciar\n");
			printf("2 - Sair \n");
			printf("Digite sua escolha: ");
			scanf("%d", &resposta);
			switch (resposta)
			{
			case 1:
				printf("\nSera reiniciado!\n");
				printf("Digite ENTER para continuar...\n");
				getchar();
				*reiniciar = 1;
			break;
			
			case 2:
				*opcao = 3;

			break;
			default:
				printf("\nValor invalido!\n");
				printf("\nSera feita novamente a pergunta\n");
				printf("Digite ENTER para continuar...\n");
				getchar();
				verificarReiniciar(Territorios, opcao, MissaoCumprida, reiniciar);
				return;
				break;
			}
			

		}
	}
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(struct territorio *Territorios, int totalTerritorios, struct Missoes *missoes)
{
    if (Territorios != NULL)
    {
        free(Territorios);
        Territorios = NULL;
    }

    if (missoes != NULL)
    {
        free(missoes);
        missoes = NULL;
    }

    printf("Memoria liberada com sucesso!\n");
}



// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
   	srand(time(NULL)); // inicializa aleatoriedade dos dados

	struct territorio *Territorios;
	int TotalTerritorios = 0;
	int opcao = 0;

	struct Missoes *missaoJogador;
	int MissaoCumprida = 0;
	int reiniciar = 0;

	Territorios = alocarMapa();
	missaoJogador = alocarMissoes();


	if(Territorios == NULL)
	{
		printf("ERRO: Falha ao alocar memoria.\n");
    	return 1;
	}

	inicializarTerritorios(Territorios, &TotalTerritorios);
	atribuirMissao(Territorios, missaoJogador, MaxMissoes);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
	while (opcao != 3){
		verificarReiniciar(Territorios, &opcao, &MissaoCumprida, &reiniciar);
		if (reiniciar == 1)
		{
			inicializarTerritorios(Territorios, &TotalTerritorios);
			atribuirMissao(Territorios, missaoJogador, MaxMissoes);
			reiniciar = 0;
			RodadasDefendidas = 0;
			RodadasAtaque = 0;
			MissaoCumprida = 0;
			

		}
		else if(opcao == 3)
		{
			printf("\nVoce esta saindo do programa\n");
			printf("Digite ENTER para continuar...\n");
			getchar();
			break;
		}
		Mapa(Territorios, &TotalTerritorios);
		exibirMisao(missaoJogador, Territorios, &MissaoCumprida);
		exibirMenuPrincipal(&opcao);

		switch (opcao)
		{
		case 1:
			FasedeAtaque(Territorios, missaoJogador, &MissaoCumprida);
		break;
		
		case 2:
			verificarVitoria(Territorios, missaoJogador, &MissaoCumprida);
		break;

		case 3:
			printf("\nVoce esta saindo do programa\n");
			printf("Digite ENTER para continuar...\n");
			getchar();	
		break;
	
		default:
			printf("Valor invalido, você sera mandado de volta ao menu\n");
			printf("Digite ENTER para continuar...\n");
			getchar();	
		break;
		}	
		
	};
	
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
	liberarMemoria(Territorios, MaxTerritorios, missaoJogador);
    return 0;
}


