#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- 1. Estrutura da Peça ---
// Estrutura para representar uma peça no jogo
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador único da peça
} Peca;

// --- 2. Variáveis Globais de Controle ---

// A. Variáveis da Fila Circular de Peças Futuras
#define MAX_FILA 5 // Tamanho máximo da fila (Future Queue)
Peca fila[MAX_FILA];
int frente = 0;
int tras = -1;
int contador_pecas_fila = 0; // Número atual de elementos na fila

// B. Variáveis da Pilha de Peças Reservadas (LIFO)
#define MAX_PILHA 3 // Capacidade máxima da pilha (Reserve Stack)
Peca pilha[MAX_PILHA];
int topo = -1; // Índice do topo da pilha (-1 indica pilha vazia)

// C. Variável de Geração de Peças
int proximo_id = 0;    // Contador para gerar IDs únicos para as peças


// ===============================================
// === 3. PROTÓTIPOS DE FUNÇÕES (RESOLVE O WARNING DE DECLARAÇÃO IMPLÍCITA) ===
// ===============================================

Peca gerarPeca();

// Protótipos da Fila (Queue)
int filaVazia();
int filaCheia();
int enqueue(Peca novaPeca);
int dequeue(Peca *pecaRemovida);
int reabastecerFila();

// Protótipos da Pilha (Stack)
int pilhaVazia();
int pilhaCheia();
int push(Peca novaPeca);
int pop(Peca *pecaRemovida);

// Protótipos das Operações Estratégicas
int trocarPecaAtual();
int trocaMultipla();

// Protótipos de Exibição e Inicialização
void exibirFila();
void exibirPilha();
void exibirEstadoAtual();
void inicializarEstruturas();


// --- 4. Funções de Manipulação Geral e Geração ---

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * @return A nova peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    int indice_aleatorio = rand() % num_tipos;
    novaPeca.nome = tipos[indice_aleatorio];

    novaPeca.id = proximo_id++;
    return novaPeca;
}

/**
 * @brief Garante que a fila volte ao seu tamanho máximo após uma remoção.
 * @return 1 se a peça foi adicionada com sucesso, 0 caso contrário.
 */
int reabastecerFila() {
    if (!filaCheia()) {
        Peca novaPeca = gerarPeca();
        if (enqueue(novaPeca)) {
            printf("\n\t[REABASTECIMENTO] Nova peça [%c %d] adicionada ao final da fila.", novaPeca.nome, novaPeca.id);
            return 1;
        }
    }
    return 0;
}


// --- 5. Funções da FILA (Queue) ---

/**
 * @brief Verifica se a fila está vazia.
 */
int filaVazia() {
    return contador_pecas_fila == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 */
int filaCheia() {
    return contador_pecas_fila == MAX_FILA;
}

/**
 * @brief Adiciona uma nova peça ao final da fila (enqueue).
 */
int enqueue(Peca novaPeca) {
    if (filaCheia()) { return 0; }
    tras = (tras + 1) % MAX_FILA;
    fila[tras] = novaPeca;
    contador_pecas_fila++;
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 */
int dequeue(Peca *pecaRemovida) {
    if (filaVazia()) {
        printf("\n\t[ERRO] A fila de peças está vazia! Não há peças para jogar/reservar.\n");
        return 0;
    }
    *pecaRemovida = fila[frente];
    frente = (frente + 1) % MAX_FILA;
    contador_pecas_fila--;
    return 1;
}


// --- 6. Funções da PILHA (Stack) ---

/**
 * @brief Verifica se a pilha está vazia.
 */
int pilhaVazia() {
    return topo == -1;
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int pilhaCheia() {
    return topo == MAX_PILHA - 1;
}

/**
 * @brief Adiciona uma peça ao topo da pilha (push).
 */
int push(Peca novaPeca) {
    if (pilhaCheia()) {
        printf("\n\t[ERRO] A Pilha de Reserva está cheia (%d)! Não é possível reservar mais peças.\n", MAX_PILHA);
        return 0;
    }
    topo++;
    pilha[topo] = novaPeca;
    return 1;
}

/**
 * @brief Remove a peça do topo da pilha (pop).
 */
int pop(Peca *pecaRemovida) {
    if (pilhaVazia()) {
        printf("\n\t[ERRO] A Pilha de Reserva está vazia! Não há peças para usar.\n");
        return 0;
    }
    *pecaRemovida = pilha[topo];
    topo--;
    return 1;
}

// --- 7. Novas Funções de Troca ---

/**
 * @brief Troca a peça da frente da fila com a peça no topo da pilha (Troca Simples).
 */
int trocarPecaAtual() {
    if (filaVazia()) {
        printf("\n\t[ERRO] A fila está vazia. Não há peça para trocar.\n");
        return 0;
    }
    if (pilhaVazia()) {
        printf("\n\t[ERRO] A pilha está vazia. Não há peça para trocar.\n");
        return 0;
    }

    // Troca direta: troca o valor no índice 'frente' da fila pelo valor no índice 'topo' da pilha.
    Peca temp = fila[frente];
    fila[frente] = pilha[topo];
    pilha[topo] = temp;

    printf("\n\t[SUCESSO] Troca Simples: Peça [%c %d] da Fila trocada com peça [%c %d] da Pilha.",
           fila[frente].nome, fila[frente].id, temp.nome, temp.id);

    return 1;
}

/**
 * @brief Troca as três primeiras peças da fila com as três peças da pilha (Troca Múltipla).
 */
int trocaMultipla() {
    const int NUM_TROCA = 3;

    if (contador_pecas_fila < NUM_TROCA) {
        printf("\n\t[ERRO] A fila tem menos de %d peças (%d). A troca não pode ser realizada.\n", NUM_TROCA, contador_pecas_fila);
        return 0;
    }
    if (topo + 1 < NUM_TROCA) {
        printf("\n\t[ERRO] A pilha tem menos de %d peças (%d). A troca não pode ser realizada.\n", NUM_TROCA, topo + 1);
        return 0;
    }

    Peca temp;

    // O loop realiza a troca elemento por elemento
    for (int i = 0; i < NUM_TROCA; i++) {
        // Índice da Fila: 'frente' + i (aplicando a lógica circular)
        int idx_fila = (frente + i) % MAX_FILA;

        // Índice da Pilha: 'topo' - i (do topo para a base)
        int idx_pilha = topo - i;

        // Troca
        temp = fila[idx_fila];
        fila[idx_fila] = pilha[idx_pilha];
        pilha[idx_pilha] = temp;
    }

    printf("\n\t[SUCESSO] Troca Múltipla: As %d primeiras peças da Fila foram alternadas com as %d peças da Pilha.",
           NUM_TROCA, NUM_TROCA);

    return 1;
}

// --- 8. Funções de Exibição e Inicialização ---

/**
 * @brief Exibe o estado atual da fila (queue).
 */
void exibirFila() {
    printf("\nFila de Peças Futuras (%d/%d): ", contador_pecas_fila, MAX_FILA);
    if (filaVazia()) { printf("[ Vazia ]"); return; }
    int i = frente;
    int elementos_exibidos = 0;
    while (elementos_exibidos < contador_pecas_fila) {
        printf("[%c %d]", fila[i].nome, fila[i].id);
        i = (i + 1) % MAX_FILA;
        elementos_exibidos++;
        if (elementos_exibidos < contador_pecas_fila) { printf(" -> "); }
    }
}

/**
 * @brief Exibe o estado atual da pilha (stack).
 */
void exibirPilha() {
    printf("\nPilha de Reserva (Topo -> Base) (%d/%d): ", topo + 1, MAX_PILHA);
    if (pilhaVazia()) { printf("[ Vazia ]"); return; }
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d]", pilha[i].nome, pilha[i].id);
        if (i > 0) { printf(" -> "); }
    }
}

/**
 * @brief Exibe o estado completo (Fila e Pilha).
 */
void exibirEstadoAtual() {
    printf("\n============================================\n");
    printf("ESTADO ATUAL DO GERENCIADOR DE PEÇAS\n");
    exibirFila();
    exibirPilha();
    printf("\n============================================\n");
}

/**
 * @brief Inicializa a fila e a pilha.
 */
void inicializarEstruturas() {
    srand(time(NULL));
    printf("Inicializando a fila com %d peças...\n", MAX_FILA);
    for (int i = 0; i < MAX_FILA; i++) {
        Peca novaPeca = gerarPeca();
        fila[i] = novaPeca;
        contador_pecas_fila++;
        tras = i;
    }
    frente = 0;
    printf("Pilha de reserva inicializada (Vazia).\n");
}

// --- 9. Função Principal (main) ---
int main() {
    int opcao;
    Peca pecaOperada;

    inicializarEstruturas();
    exibirEstadoAtual();

    do {
        // Menu de Opções
        printf("\n--- Opções Disponíveis ---\n");
        printf("1. Jogar peça (Dequeue da Fila)\n");
        printf("2. Reservar peça (Fila -> Pilha)\n");
        printf("3. Usar peça reservada (Pop da Pilha)\n");
        printf("4. Trocar peça da frente da fila com o topo da pilha (Troca Simples)\n");
        printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha (Troca Múltipla)\n");
        printf("0. Sair\n");
        printf("----------------------------\n");
        printf("Opção escolhida: ");

        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada inválida. Por favor, digite um número.\n");
            while (getchar() != '\n');
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: // Jogar peça
                if (dequeue(&pecaOperada)) {
                    printf("\n\t[SUCESSO] Peça [%c %d] jogada no tabuleiro.", pecaOperada.nome, pecaOperada.id);
                    reabastecerFila();
                }
                break;

            case 2: // Reservar peça
                if (filaVazia()) {
                    printf("\n\t[ERRO] A fila está vazia. Não há peças para reservar.\n");
                } else if (!pilhaCheia()) {
                    dequeue(&pecaOperada);
                    push(pecaOperada);
                    printf("\n\t[SUCESSO] Peça [%c %d] movida da Fila para a Pilha de Reserva.", pecaOperada.nome, pecaOperada.id);
                    reabastecerFila();
                }
                break;

            case 3: // Usar peça reservada
                pop(&pecaOperada);
                // Usar pecaOperada.id != -1 como um sinal simples de sucesso de pop
                // (Embora não seja o ideal, mantém a consistência com o código anterior)
                if (pecaOperada.id < proximo_id) {
                    printf("\n\t[SUCESSO] Peça [%c %d] usada diretamente da Pilha de Reserva.", pecaOperada.nome, pecaOperada.id);
                }
                break;

            case 4: // Troca Simples
                trocarPecaAtual();
                break;

            case 5: // Troca Múltipla
                trocaMultipla();
                break;

            case 0:
                printf("\nSaindo do gerenciador de peças do Tetris Stack. Até a próxima fase!\n");
                break;

            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        // Exibe o estado após qualquer ação (exceto Sair)
        if (opcao != 0) {
            exibirEstadoAtual();
        }

    } while (opcao != 0);

    return 0;
}