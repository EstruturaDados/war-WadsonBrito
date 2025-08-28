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
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

void limparEntrada(void);
void cadastrarTerritorios(Territorio *mapa, int n);
void exibirTerritorios(const Territorio *mapa, int n);
int  escolherIndice(const char *rotulo, const Territorio *mapa, int n);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa);

// -----------------------------
// Funções Auxiliares
// -----------------------------

// Consome resto da linha do stdin (evita problemas do scanf com fgets)
void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Remove o \n final de uma string lida por fgets, se existir
void tiraQuebraLinha(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

// Valida inteiro não negativo
int lerInteiroPositivo(const char *msg) {
    int valor;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &valor) == 1 && valor > 0) {
            limparEntrada();
            return valor;
        }
        printf("Entrada inválida. Digite um número inteiro positivo.\n");
        limparEntrada();
    }
}

// -----------------------------
// Cadastro e Exibição
// -----------------------------

void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== Cadastro de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório #%d\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        tiraQuebraLinha(mapa[i].nome);
        if (strlen(mapa[i].nome) == 0) snprintf(mapa[i].nome, sizeof(mapa[i].nome), "T%d", i);

        printf("Cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        tiraQuebraLinha(mapa[i].cor);
        if (strlen(mapa[i].cor) == 0) snprintf(mapa[i].cor, sizeof(mapa[i].cor), "Neutro");

        mapa[i].tropas = lerInteiroPositivo("Quantidade de Tropas: ");
    }
}

void exibirTerritorios(const Territorio *mapa, int n) {
    printf("\n=== Estado Atual dos Territórios ===\n");
    printf("%-3s | %-29s | %-9s | %-6s\n", "ID", "Nome", "Cor", "Tropas");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-29s | %-9s | %-6d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// -----------------------------
// Escolha de Índices com Validação
// -----------------------------

int escolherIndice(const char *rotulo, const Territorio *mapa, int n) {
    int idx;
    while (1) {
        printf("%s (ID entre 0 e %d): ", rotulo, n - 1);
        if (scanf("%d", &idx) == 1 && idx >= 0 && idx < n) {
            limparEntrada();
            return idx;
        }
        printf("ID inválido. Tente novamente.\n");
        limparEntrada();
    }
}

// -----------------------------
// Mecânica de Ataque
// -----------------------------

/*
 * Simula um ataque entre dois territórios.
 * Regra:
 *  - Cada lado rola 1 dado (1..6).
 *  - Se o atacante vencer (dadoA > dadoD):
 *      - O defensor muda de cor para a cor do atacante.
 *      - Metade das tropas do atacante é transferida para o defensor (min 1).
 *      - As tropas do atacante diminuem pelo mesmo valor transferido.
 *  - Se empatar ou perder (dadoA <= dadoD):
 *      - O atacante perde 1 tropa (mínimo 0), território defensor permanece.
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (atacante == NULL || defensor == NULL) return;

    if (atacante->tropas <= 0) {
        printf("\n[!] O território atacante não possui tropas para atacar.\n");
        return;
    }

    int dadoA = (rand() % 6) + 1; // 1..6
    int dadoD = (rand() % 6) + 1; // 1..6

    printf("\nDados jogados — %s (Atacante) tirou %d x %s (Defensor) tirou %d\n",
           atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        // Vitória do atacante
        int transferir = atacante->tropas / 2; // metade
        if (transferir < 1) transferir = 1;    // garante transferência mínima
        if (transferir > atacante->tropas) transferir = atacante->tropas; // sanidade

        printf("Vitória do atacante! Transferindo %d tropas para o território defensor.\n", transferir);

        // Mudança de controle
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';

        // Atualização de tropas
        defensor->tropas = transferir; // território conquistado recebe tropas enviadas
        atacante->tropas -= transferir;

    } else {
        // Derrota ou empate — atacante perde 1
        if (atacante->tropas > 0) atacante->tropas -= 1;
        printf("Defensor resistiu! O atacante perdeu 1 tropa.\n");
    }
}

// -----------------------------
// Liberação de Memória
// -----------------------------

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}


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

int main(void) {
    srand((unsigned)time(NULL));

    printf("=== WAR Estruturado — Simulação de Ataques ===\n");

    int n = lerInteiroPositivo("Quantos territórios deseja cadastrar? ");

   

   


// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
 // Alocação dinâmica (calloc zera a memória)
    Territorio *mapa = (Territorio *)calloc(n, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Falha ao alocar memória para os territórios.\n");
        return EXIT_FAILURE;
    }

    cadastrarTerritorios(mapa, n);

    int continuar = 1;
    while (continuar) {
        exibirTerritorios(mapa, n);

        printf("\nEscolha os territórios para o ataque.\n");
        int ida = escolherIndice("ID do Atacante", mapa, n);
        int idd = escolherIndice("ID do Defensor", mapa, n);

        if (ida == idd) {
            printf("\n[!] Território atacante e defensor não podem ser o mesmo. Tente novamente.\n");
        } else if (strcmp(mapa[ida].cor, mapa[idd].cor) == 0) {
            printf("\n[!] Você não pode atacar um território da mesma cor (mesmo dono).\n");
        } else {
            atacar(&mapa[ida], &mapa[idd]);
        }

        exibirTerritorios(mapa, n);

        printf("\nDeseja realizar outro ataque? (1 = sim, 0 = não): ");
        if (scanf("%d", &continuar) != 1) {
            limparEntrada();
            continuar = 0;
        } else {
            limparEntrada();
        }
    }
// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
 liberarMemoria(mapa);
    printf("\nMemória liberada. Encerrando...\n\n");
    return EXIT_SUCCESS;
}
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
