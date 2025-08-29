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
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    
    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.


// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
 // Alocação dinâmica (calloc zera a memória)
// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 5

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Função para limpar buffer do stdin
void limparBuffer() {
    while(getchar() != '\n');
}

// Exibir mapa dos territórios
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n=== Mapa Atual ===\n");
    for(int i=0; i<tamanho; i++) {
        printf("%d. %s | Cor: %s | Tropas: %d\n",
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtq = rand()%6 + 1;
    int dadoDef = rand()%6 + 1;

    printf("\n%s ataca %s!\n", atacante->nome, defensor->nome);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    if(dadoAtq >= dadoDef) {
        int tropasConquistadas = defensor->tropas / 2;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasConquistadas;
        atacante->tropas -= 1;
        printf("Ataque bem-sucedido! %s agora pertence a %s com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
    } else {
        atacante->tropas -= 1;
        printf("Ataque falhou! %s perde 1 tropa. Tropas restantes: %d\n",
               atacante->nome, atacante->tropas);
    }
}

// Função para atribuir missão ao jogador
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    *destino = (char*) malloc(strlen(missoes[idx]) + 1);
    if(!(*destino)) {
        printf("Erro de alocacao de memoria para missao!\n");
        exit(1);
    }
    strcpy(*destino, missoes[idx]);
}

// Exibir missão do jogador
void exibirMissao(const char* missao) {
    printf("\nSua missão secreta: %s\n", missao);
}

// Verificar se missão foi cumprida (lógica inicial simples)
int verificarMissao(const char* missao, Territorio* mapa, int tamanho) {
    if(strstr(missao, "Conquistar 3 territórios") != NULL) {
        char corJogador[MAX_COR];
        strcpy(corJogador, mapa[0].cor); // Considera primeira cor como do jogador
        int cont = 0;
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0)
                cont++;
        }
        return (cont >= 3);
    } else if(strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].cor, "Vermelha") == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    } else if(strstr(missao, "Conquistar território azul") != NULL) {
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].cor, "Azul") == 0)
                return 1;
        }
        return 0;
    }
    return 0; // missões não implementadas retornam falso
}

// Liberar memória alocada dinamicamente
void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

// Cadastro inicial de territórios
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for(int i=0; i<tamanho; i++) {
        printf("=== Territorio %d ===\n", i+1);
        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("Cor do exército: ");
        fgets(mapa[i].cor, MAX_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

// Menu principal do jogo
void menu(Territorio* mapa, int tamanho, char* missao) {
    int opcao;
    do {
        exibirMapa(mapa, tamanho);
        printf("\nMenu:\n1 - Atacar\n2 - Verificar Missão\n0 - Sair\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: {
                int atq, def;
                printf("Escolha território atacante (1-%d): ", tamanho);
                scanf("%d", &atq);
                limparBuffer();
                printf("Escolha território defensor (1-%d): ", tamanho);
                scanf("%d", &def);
                limparBuffer();

                if(atq>=1 && atq<=tamanho && def>=1 && def<=tamanho)
                    atacar(&mapa[atq-1], &mapa[def-1]);
                else
                    printf("Territórios inválidos!\n");
                break;
            }
            case 2:
                if(verificarMissao(missao, mapa, tamanho))
                    printf("\nParabéns! Missão cumprida!\n");
                else
                    printf("\nMissão ainda não cumprida.\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);
}

int main() {
    srand(time(NULL));

    // Alocação dinâmica dos territórios
    Territorio* mapa = (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if(!mapa) {
        printf("Erro de alocação de memória!\n");
        return 1;
    }

    // Cadastro inicial
    printf("Bem-vindo ao WAR Estruturado com Missões Estratégicas!\n");
    cadastrarTerritorios(mapa, NUM_TERRITORIOS);

    // Criar vetor de missões
    char* missoes[NUM_MISSOES] = {
        "Conquistar 3 territórios",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar território azul",
        "Possuir o maior exército do mapa",
        "Controlar territórios de cores diferentes"
    };

    // Atribuir missão ao jogador
    char* missaoJogador = NULL;
    atribuirMissao(&missaoJogador, missoes, NUM_MISSOES);

    // Exibir missão
    exibirMissao(missaoJogador);

    // Menu principal
    menu(mapa, NUM_TERRITORIOS, missaoJogador);

    // Liberar memória
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

