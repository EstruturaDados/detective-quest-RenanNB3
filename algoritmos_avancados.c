#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.



typedef struct{
    char nome[30];
    struct sala *esq;
    struct sala *dir;
}Sala;


Sala* criarSala(const char *nome){
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if (!nova){
        printf("Erro de alocação\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Caso seja nó-folha
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Não há mais caminhos a partir daqui.\n");
            printf("Fim da exploração.\n");
            return;
        }

        printf("Escolha um caminho:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esq != NULL) {
                atual = atual->esq;
            } else {
                printf("Não existe sala à esquerda!\n");
            }
        }
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->dir != NULL) {
                atual = atual->dir;
            } else {
                printf("Não existe sala à direita!\n");
            }
        }
        else if (escolha == 's' || escolha == 'S') {
            printf("Saindo da exploração...\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}


int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.
 
    //Criando salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");

    hall->esq = salaEstar;
    hall->dir = biblioteca;
    salaEstar->esq = cozinha;
    salaEstar->dir = jardim;
    biblioteca->dir = porao;

    // Iniciar exploração
    printf("--- Detective Quest — Exploração da Mansão ---\n");
    explorarSalas(hall);

    // Liberação de memória 
    free(cozinha);
    free(jardim);
    free(salaEstar);
    free(porao);
    free(biblioteca);
    free(hall);


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

