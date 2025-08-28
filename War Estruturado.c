#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -----------------------------
// Estruturas e Assinaturas
// -----------------------------

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

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

    printf("\nRolagem — %s (A) tirou %d x %s (D) tirou %d\n",
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

// -----------------------------
// Programa Principal
// -----------------------------

int main(void) {
    srand((unsigned)time(NULL));

    printf("=== WAR Estruturado — Simulação de Ataques ===\n");

    int n = lerInteiroPositivo("Quantos territórios deseja cadastrar? ");

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

    liberarMemoria(mapa);
    printf("\nMemória liberada. Encerrando...\n");
    return EXIT_SUCCESS;
}
