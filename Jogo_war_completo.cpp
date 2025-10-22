// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE C�DIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o c�digo em fun��es especializadas.
// - Implementar um sistema de miss�es para um jogador.
// - Criar uma fun��o para verificar se a miss�o foi cumprida.
// - Utilizar passagem por refer�ncia (ponteiros) para modificar dados e
//   passagem por valor/refer�ncia constante (const) para apenas ler.
// - Foco em: Design de software, modulariza��o, const correctness, l�gica de jogo.
//
// ============================================================================

// Inclus�o das bibliotecas padr�o necess�rias para entrada/sa�da, aloca��o de mem�ria, manipula��o de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o n�mero de territ�rios, miss�es e tamanho m�ximo de strings, facilitando a manuten��o.
#define MaxTerritorios 5
#define MaxString 90
#define Maxdes 100
#define MaxMissoes 5

int RodadasDefendidas = 0;
int RodadasAtaque = 0;
//variavel que impede aparecer o aviso quando nao solicitado 
int naoaparecer = 0;


// --- Estrutura de Dados ---
// Define a estrutura para um territ�rio, contendo seu nome, a cor do ex�rcito que o domina e o n�mero de tropas.
struct territorio 
{
	char nome [MaxString];
	char cor[10];
	int tropas;
}; 

//Define a estrutura para a miss�o, contendo ID, exercito que sera atribuido a miss�o, objetivo e quem sofrera a miss�o se tiver um especifico 
struct Missoes
{
	int ID;
	char ExercitoEscolhido[MaxString];
	char objetivo[MaxString];
	char sofredor[MaxString];

};

//Vetor de string de miss�es
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


// --- Prot�tipos das Fun��es ---
// Declara��es antecipadas de todas as fun��es que ser�o usadas no programa, organizadas por categoria.
// Fun��es de setup e gerenciamento de mem�ria:
// Fun��es de interface com o usu�rio:
// Fun��es de l�gica principal do jogo:
// Fun��o utilit�ria:


// --- Implementa��o das Fun��es ---

// limparBufferEntrada():
// Fun��o utilit�ria para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void LimparBuffer ()
{
	int c;
	while((c=getchar()) != '\n' && c != EOF);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de a��es dispon�veis para o jogador.
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
// Aloca dinamicamente a mem�ria para o vetor de territ�rios usando calloc.
// Retorna um ponteiro para a mem�ria alocada ou NULL em caso de falha.
struct territorio* alocarMapa()
{
	return (struct territorio*) calloc(MaxTerritorios, sizeof(struct territorio));
}

struct Missoes* alocarMissoes()
{
	return (struct Missoes*) malloc(MaxMissoes * sizeof(struct Missoes));
}


// inicializarTerritorios():
// Preenche os dados iniciais de cada territ�rio no mapa (nome, cor do ex�rcito, n�mero de tropas).
// Esta fun��o modifica o mapa passado por refer�ncia (ponteiro).
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
// Mostra o estado atual de todos os territ�rios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a fun��o apenas leia os dados do mapa, sem modific�-los.
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
// Gerencia a interface para a a��o de ataque, solicitando ao jogador os territ�rios de origem e destino.
// Chama a fun��o simularAtaque() para executar a l�gica da batalha.
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
		printf("voc� ser� levado a fase de ataque novamente\n\n");
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
						if (strcmp(missao[0].objetivo, "Defender territ�rio por 3 rodadas") == 0)
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
								printf("\nvoc� ser� levado a fase de ataque novamente\n");
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
// Executa a l�gica de uma batalha entre dois territ�rios.
// Realiza valida��es, rola os dados, compara os resultados e atualiza o n�mero de tropas.
// Se um territ�rio for conquistado, atualiza seu dono e move uma tropa.
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
				// Se s� tem 1, n�o pode mover, apenas ocupa com 1
				Territorios[defensor - 1].tropas = 1;
			}
			// Em caso de n�mero par, divide igualmente
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
				// Se s� tem 1, n�o pode mover, apenas ocupa com 1
				Territorios[atacante - 1].tropas = 1;
			}
			// Em caso de n�mero par, divide igualmente
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
	printf("voc� ser� levado a proxima fase\n");
	printf("Digite ENTER para continuar...\n");
	getchar();		
}

// sortearMissao():
// Sorteia e retorna um ID de miss�o aleat�rio para o jogador.
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
// Exibe a descri��o da miss�o atual do jogador com base no ID da miss�o sorteada.
void exibirMisao(struct Missoes *missao, struct territorio *Territorios, int *MissaoCumprida)
{
	int SemJogador =1 ; //verifica se � possivel cumprir a miss�o, 1 para sem jogador e 0 se h� jogador
		
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
		printf(" -> Miss�o: %s", missao[0].objetivo);
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
		printf(" -> Miss�o: %s", missao[0].objetivo);
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
// Verifica se o jogador cumpriu os requisitos de sua miss�o atual.
// Implementa a l�gica para cada tipo de miss�o (destruir um ex�rcito ou conquistar um n�mero de territ�rios).
// Retorna 1 (verdadeiro) se a miss�o foi cumprida, e 0 (falso) caso contr�rio.
void verificarVitoria(struct territorio *Territorios,struct Missoes *missostruct,int *MissaoCumprida)
{
	int SemJogador = 1; //verifica se � possivel cumprir a miss�o, 1 para sem jogador e 0 se h� jogador
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
// Libera a mem�ria previamente alocada para o mapa usando free.
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



// --- Fun��o Principal (main) ---
// Fun��o principal que orquestra o fluxo do jogo, chamando as outras fun��es em ordem.
int main() {
    // 1. Configura��o Inicial (Setup):
    // - Define o locale para portugu�s.
    // - Inicializa a semente para gera��o de n�meros aleat�rios com base no tempo atual.
    // - Aloca a mem�ria para o mapa do mundo e verifica se a aloca��o foi bem-sucedida.
    // - Preenche os territ�rios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua miss�o secreta.
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

    // 2. La�o Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua at� o jogador sair (op��o 0) ou vencer.
    // - A cada itera��o, exibe o mapa, a miss�o e o menu de a��es.
    // - L� a escolha do jogador e usa um 'switch' para chamar a fun��o apropriada:
    //   - Op��o 1: Inicia a fase de ataque.
    //   - Op��o 2: Verifica se a condi��o de vit�ria foi alcan�ada e informa o jogador.
    //   - Op��o 0: Encerra o jogo.
    // - Pausa a execu��o para que o jogador possa ler os resultados antes da pr�xima rodada.
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
			printf("Valor invalido, voc� sera mandado de volta ao menu\n");
			printf("Digite ENTER para continuar...\n");
			getchar();	
		break;
		}	
		
	};
	
    // 3. Limpeza:
    // - Ao final do jogo, libera a mem�ria alocada para o mapa para evitar vazamentos de mem�ria.
	liberarMemoria(Territorios, MaxTerritorios, missaoJogador);
    return 0;
}


