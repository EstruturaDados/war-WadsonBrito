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
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define MAX_NOME 64
#define NUM_MISSOES 2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_NOME];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int n);
void inicializarTerritorios(Territorio* mapa, int n);
void liberarMemoria(Territorio* mapa, int n);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int n);
void exibirMissao(int missaoID);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int n);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int n, int missaoID);

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
void limparBufferEntrada();

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand((unsigned int)time(NULL));

    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa, NUM_TERRITORIOS);
    int missaoID = sortearMissao();

    int opcao;
    do {
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(missaoID);
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, missaoID))
                    printf("Parabéns! Você cumpriu sua missão!\n");
                else
                    printf("Missão ainda não concluída. Continue jogando!\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        printf("\n");
    } while(opcao != 0 && !verificarVitoria(mapa, NUM_TERRITORIOS, missaoID));

    liberarMemoria(mapa, NUM_TERRITORIOS);
    return 0;
}


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

Territorio* alocarMapa(int n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

void inicializarTerritorios(Territorio* mapa, int n) {
    for(int i = 0; i < n; i++) {
        printf("Digite o nome do território %d: ", i+1);
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército do território %d: ", i+1);
        fgets(mapa[i].cor, MAX_NOME, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas do território %d: ", i+1);
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

void liberarMemoria(Territorio* mapa, int n) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n--- Menu Principal ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

void exibirMapa(const Territorio* mapa, int n) {
    printf("\n--- Mapa Atual ---\n");
    printf("%-3s %-20s %-15s %s\n", "ID", "Nome", "Cor", "Tropas");
    for(int i = 0; i < n; i++) {
        printf("%-3d %-20s %-15s %d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int missaoID) {
    printf("\n--- Missão Atual ---\n");
    switch(missaoID) {
        case 0:
            printf("Destruir o exército Verde.\n");
            break;
        case 1:
            printf("Conquistar 3 territórios.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

void faseDeAtaque(Territorio* mapa, int n) {
    int idAtacante, idDefensor;
    printf("Escolha o território atacante (1-%d): ", n);
    scanf("%d", &idAtacante);
    limparBufferEntrada();
    printf("Escolha o território defensor (1-%d): ", n);
    scanf("%d", &idDefensor);
    limparBufferEntrada();

    if(idAtacante < 1 || idAtacante > n || idDefensor < 1 || idDefensor > n || idAtacante == idDefensor) {
        printf("Territórios inválidos.\n");
        return;
    }

    simularAtaque(&mapa[idAtacante-1], &mapa[idDefensor-1]);
}

void simularAtaque(Territorio* atacante, Territorio* defensor) {
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("%s ataca %s!\n", atacante->nome, defensor->nome);
    printf("Dado do atacante: %d\n", dadoAtaque);
    printf("Dado do defensor: %d\n", dadoDefesa);

    if(dadoAtaque >= dadoDefesa) {
        defensor->tropas--;
        printf("Atacante vence a rodada!\n");
        if(defensor->tropas <= 0) {
            printf("%s foi conquistado!\n", defensor->nome);
            strncpy(defensor->cor, atacante->cor, MAX_NOME);
            defensor->tropas = 1;
            atacante->tropas--;
        }
    } else {
        printf("Defensor resiste!\n");
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int n, int missaoID) {
    int contador = 0;
    switch(missaoID) {
        case 0: // destruir exército Verde
            for(int i = 0; i < n; i++) {
                if(strcmp(mapa[i].cor, "Verde") == 0)
                    return 0;
            }
            return 1;
        case 1: // conquistar 3 territórios
            for(int i = 0; i < n; i++) {
                if(strcmp(mapa[i].cor, "Jogador") == 0)
                    contador++;
            }
            return (contador >= 3);
        default:
            return 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}