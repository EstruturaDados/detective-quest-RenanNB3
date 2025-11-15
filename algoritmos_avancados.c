#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 31

typedef struct sala{
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    struct sala *esq;
    struct sala *dir;
}Sala;

typedef struct pistaNode{
    char conteudo[100];
    struct pistaNode*esq;
    struct pistaNode*dir;

}PistaNode;

typedef struct HashNode{
    char chave[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashNode *prox;
}HashNode;

Sala* criarSala(const char *nome, const char *pista);
void explorarSalas(Sala *raiz, PistaNode **pistas, HashNode *tabela[]);
PistaNode* inserirPista(PistaNode *raiz, const char *texto);
PistaNode* adicionarPista(PistaNode *raiz, const char *texto); // wrapper/alias
void exibirPistas(PistaNode *raiz);
void liberarPistas(PistaNode *raiz);

unsigned int hash(const char *s);
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito);
const char* encontrarSuspeito(HashNode *tabela[], const char *pista);
void liberarHash(HashNode *tabela[]);

void verificarSuspeitoFinal(PistaNode *pistas, HashNode *tabela[]);

void limparLinhaEntrada();
void str_trim(char *s);

Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro de alocação para Sala\n");
        exit(1);
    }
    strncpy(s->nome, nome, MAX_NOME-1); s->nome[MAX_NOME-1] = '\0';
    if (pista) {
        strncpy(s->pista, pista, MAX_PISTA-1); s->pista[MAX_PISTA-1] = '\0';
    } else {
        s->pista[0] = '\0';
    }
    s->esq = s->dir = NULL;
    return s;
}

PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if (raiz == NULL) {
        PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
        if (!n) { fprintf(stderr, "Erro de alocação para PistaNode\n"); exit(1); }
        strncpy(n->conteudo, texto, MAX_PISTA-1); n->conteudo[MAX_PISTA-1] = '\0';
        n->esq = n->dir = NULL;
        return n;
    }
    int cmp = strcmp(texto, raiz->conteudo);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, texto);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, texto);
    return raiz;
}

// Insere pista na BST em ordem alfabética
PistaNode* adicionarPista(PistaNode *raiz, const char *texto) {
    return inserirPista(raiz, texto);
}

// exibirPistas() – imprime a árvore de pistas em ordem alfabética.
void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->conteudo);
    exibirPistas(raiz->dir);
}

void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

// Hash simples (djb2)
unsigned int hash(const char *s) {
    unsigned long h = 5381;
    while (*s) h = ((h << 5) + h) + (unsigned char)(*s++);
    return (unsigned int)(h % HASH_SIZE);
}

// inserirNaHash() – insere associação pista/suspeito na tabela hash.
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    unsigned int idx = hash(pista);
    HashNode *n = (HashNode*) malloc(sizeof(HashNode));
    if (!n) { fprintf(stderr, "Erro de alocação para HashNode\n"); exit(1); }
    strncpy(n->chave, pista, MAX_PISTA-1); n->chave[MAX_PISTA-1] = '\0';
    strncpy(n->suspeito, suspeito, MAX_NOME-1); n->suspeito[MAX_NOME-1] = '\0';
    n->prox = tabela[idx];
    tabela[idx] = n;
}

// encontrarSuspeito() – consulta o suspeito correspondente a uma pista.
const char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    unsigned int idx = hash(pista);
    HashNode *p = tabela[idx];
    while (p) {
        if (strcmp(p->chave, pista) == 0) return p->suspeito;
        p = p->prox;
    }
    return NULL; // não encontrado
}

void liberarHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *p = tabela[i];
        while (p) {
            HashNode *t = p->prox;
            free(p);
            p = t;
        }
        tabela[i] = NULL;
    }
}

// explorarSalas() – navega pela árvore e ativa o sistema de pistas.
void explorarSalas(Sala *raiz, PistaNode **pistas, HashNode *tabela[]) {
    Sala *atual = raiz;
    char opc;
    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("📍 Pista encontrada: \"%s\"\n", atual->pista);

            // adiciona na BST (evita duplicata)
            *pistas = adicionarPista(*pistas, atual->pista);

            // informa suspeito associado, se houver
            const char *sus = encontrarSuspeito(tabela, atual->pista);
            if (sus) {
                printf("   (essa pista está associada ao suspeito: %s)\n", sus);
            } else {
                printf("   (nenhum suspeito associado a esta pista)\n");
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("\nEscolha: (e)=esquerda  (d)=direita  (s)=sair\nOpção: ");
        scanf(" %c", &opc);
        limparLinhaEntrada();

        if (opc == 'e' || opc == 'E') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há sala à esquerda.\n");
        } else if (opc == 'd' || opc == 'D') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há sala à direita.\n");
        } else if (opc == 's' || opc == 'S') {
            printf("Encerrando exploração.\n");
            return;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

// VerificarSuspeitoFinal() 
void verificarSuspeitoFinal(PistaNode *pistas, HashNode *tabela[]) {
    char acusado[MAX_NOME];
    printf("\n--- Fase de Acusação ---\n");
    printf("Pistas coletadas (A-Z):\n");
    if (!pistas) {
        printf(" Nenhuma pista coletada.\n");
    } else {
        exibirPistas(pistas);
    }

    printf("\nDigite o nome do suspeito que deseja acusar: ");
    if (!fgets(acusado, sizeof(acusado), stdin)) {
        acusado[0] = '\0';
    }
    str_trim(acusado);
    if (acusado[0] == '\0') {
        printf("Nenhum nome informado. Encerrando julgamento.\n");
        return;
    }

    // Contar quantas pistas distintas coletadas apontam para o suspeito acusado.
    // Fazemos percurso in-order da BST e para cada pista consultamos a hash.
    int contador = 0;

    // Percurso iterativo recursivo simples
    // Implementa função recursiva anônima via ponteiro-função não trivial em C:
    // Vou implementar uma função local estática com passagem de contexto via struct.
    struct Context { PistaNode *raiz; HashNode **tabela; const char *acus; int *cnt; } ctx;
    ctx.raiz = pistas; ctx.tabela = tabela; ctx.acus = acusado; ctx.cnt = &contador;

    // Função recursiva auxiliar:
    void aux_contar(PistaNode *n, HashNode *tabela[], const char *acus, int *cnt){
        if (!n) return;
        aux_contar(n->esq, tabela, acus, cnt);
        const char *sus = encontrarSuspeito(tabela, n->conteudo);
        if (sus && strcmp(sus, acus) == 0) {
            (*cnt)++;
        }
        aux_contar(n->dir, tabela, acus, cnt);
    }
    aux_contar(ctx.raiz, ctx.tabela, ctx.acus, ctx.cnt);

    printf("\nVocê acusou: %s\n", acusado);
    if (contador >= 2) {
        printf("Veredito: Ótimo trabalho! Há %d pista(s) que sustentam a acusação. Suspeito considerado CULPADO.\n", contador);
    } else if (contador == 1) {
        printf("Veredito: Apenas 1 pista aponta para este suspeito. Evidências insuficientes para condenação.\n");
    } else {
        printf("Veredito: Nenhuma pista coletada aponta para este suspeito. Acusação infundada.\n");
    }
}

// Funções utilitárias
void limparLinhaEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) ;
}

// Remove espaços iniciais e finais
void str_trim(char *s) {
    // trim right
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || isspace((unsigned char)s[len-1]))) {
        s[--len] = '\0';
    }
    // trim left
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start)+1);
}


int main() {
 
    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

   
    //Criando salas
    Sala *hall = criarSala("Hall de Entrada", "pegadas sujas");
    Sala *salaEstar = criarSala("Sala de Estar", "relógio parado às 23:15");
    Sala *biblioteca = criarSala("Biblioteca", "marca de pó na sapato");
    Sala *cozinha = criarSala("Cozinha", "copo quebrado");
    Sala *jardim = criarSala("Jardim", "terra remexida");
    Sala *porao = criarSala("Porão", "fio cortado");
    Sala *quarto = criarSala("Quarto", "");

    hall->esq = salaEstar;
    hall->dir = biblioteca;
    salaEstar->esq = cozinha;
    salaEstar->dir = jardim;
    biblioteca->dir = porao;
    porao->esq = quarto;

    HashNode *tabela[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i) tabela[i] = NULL;

    inserirNaHash(tabela, "pegadas sujas", "Carlos");
    inserirNaHash(tabela, "relógio parado às 23:15", "Ana");
    inserirNaHash(tabela, "marca de pó na sapato", "Carlos");
    inserirNaHash(tabela, "copo quebrado", "Beatriz");
    inserirNaHash(tabela, "terra remexida", "Daniel");
    inserirNaHash(tabela, "fio cortado", "Carlos");

    PistaNode *pistas = NULL;


    // Iniciar exploração
    printf("--- Detective Quest — Exploração da Mansão ---\n");
    
    printf("Explore a mansão e colete pistas. Ao final, acuse um suspeito.\n");

    explorarSalasComPistas(hall, &pistas, tabela);
    verificarSuspeitoFinal(pistas, tabela);

    liberarPistas(pistas);
    liberarHash(tabela);

    free(quarto);
    free(porao);
    free(jardim);
    free(cozinha);
    free(biblioteca);
    free(salaEstar);
    free(hall);

    return 0;
}

