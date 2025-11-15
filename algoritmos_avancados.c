#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.



typedef struct sala{
    char nome[30];
    char pista[100];
    struct sala *esq;
    struct sala *dir;
}Sala;

typedef struct pistaNode{
    char conteudo[100];
    struct pistaNode*esq;
    struct pistaNode*dir;

}PistaNode;




Sala* criarSala(const char *nome, const char *pista){
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if (!nova){
        printf("Erro de alocação\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
    {
        strcpy(nova->pista, pista);
    }
    else{
        strcpy(nova->pista, "");
    }
    
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

PistaNode* criarNoPista(const char *texto) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }

    strcpy(novo->conteudo, texto);
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Insere pista na BST em ordem alfabética
PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if (raiz == NULL)
        return criarNoPista(texto);

    if (strcmp(texto, raiz->conteudo) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (strcmp(texto, raiz->conteudo) > 0)
        raiz->dir = inserirPista(raiz->dir, texto);

    return raiz;
}

// Exibir pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->conteudo);
    exibirPistas(raiz->dir);
}


void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    char escolha;

    while (1) {
        printf("Você está na sala: %s\n", atual->nome);

        // Coletar pista, caso haja
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada nesta sala.\n");
        }

        // Opções
        printf("\nEscolha o próximo caminho:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esq != NULL)
                atual = atual->esq;
            else
                printf("Não existe sala à esquerda!\n");
        }
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->dir != NULL)
                atual = atual->dir;
            else
                printf("Não existe sala à direita!\n");
        }
        else if (escolha == 's' || escolha == 'S') {
            printf("\nEncerrando exploração...\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}



int main() {
 

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    //Criando salas
    Sala *hall        = criarSala("Hall de Entrada", "Pegadas recentes no carpete");
    Sala *salaEstar   = criarSala("Sala de Estar", "Um relógio parado às 23h15");
    Sala *biblioteca  = criarSala("Biblioteca", "Um livro arrancado da estante");
    Sala *cozinha     = criarSala("Cozinha", "Copo quebrado no chão");
    Sala *jardim      = criarSala("Jardim", "Terra remexida próxima às flores");
    Sala *porao       = criarSala("Porão", "Luz fraca piscando sozinha");


    hall->esq = salaEstar;
    hall->dir = biblioteca;
    salaEstar->esq = cozinha;
    salaEstar->dir = jardim;
    biblioteca->dir = porao;

    PistaNode *pistas = NULL;


    // Iniciar exploração
    printf("--- Detective Quest — Exploração da Mansão ---\n");
    explorarSalasComPistas(hall, &pistas);

    printf("Pistas coletadas:\n");

    if (pistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(pistas);


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

    return 0;
}

