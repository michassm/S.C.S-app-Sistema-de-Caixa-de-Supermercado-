#include <stdio.h>
/*
    PARTICIPANTES: MERCADÃO DA ESKINA WYDEN
        Michel da Silva Moreira         RA: 202601387642
        Rute Carboni Moreira            RA: 202601733461
        Amanda Araujo Sanches           RA: 202602499452
        Manuelly Tristão de Carvalho    RA: 202602113643
        Daniel Savala Lhoretti          RA: 202601486021
*/

// ===================== STRUCTS =====================

/* Representa um produto disponível no sistema.
   Armazena código identificador, nome e preço unitário. */
typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

/* Representa um operador (funcionário) responsável pelo caixa.
   Armazena código identificador e nome do operador. */
typedef struct {
    int codigo;
    char nome[30];
} Operador;

/* Representa um caixa do estabelecimento.
   Armazena o número do caixa, seu estado (aberto/fechado)
   e o índice do operador responsável no vetor de operadores. */
typedef struct {
    int numero;
    int aberto;       // 1 = aberto, 0 = fechado
    int operador_idx; // índice no vetor global de operadores
} Caixa;

/* Representa um item dentro de uma compra em andamento.
   Armazena o índice do produto no vetor global, a quantidade
   escolhida e o subtotal calculado (preço × quantidade). */
typedef struct {
    int produto_idx; // índice no vetor global de produtos
    int quantidade;
    float subtotal;
} ItemCompra;

// ===================== DADOS GLOBAIS =====================

/* Vetor que armazena todos os produtos cadastrados no sistema. */
Produto produtos[100];
int qtd_produtos = 0; // contador de produtos cadastrados

/* Vetor que armazena todos os operadores que abriram caixas. */
Operador operadores[10];
int qtd_operadores = 0; // contador de operadores registrados

/* Vetor que armazena todos os caixas criados na sessão. */
Caixa caixas[10];
int qtd_caixas = 0; // contador de caixas criados

/* Índices do caixa e operador ativos no momento.
   -1 indica que nenhum caixa/operador está ativo. */
int caixa_atual    = -1;
int operador_atual = -1;

/* Vetor dos itens da compra em andamento. */
ItemCompra itens_compra[100];
int qtd_itens_compra = 0; // quantidade de itens na compra atual

/* Totais financeiros da sessão. */
float total_compra = 0;  // valor total da compra atual
float faturamento  = 0;  // valor acumulado de todas as compras do dia

// ===================== FUNÇÕES =====================

/* Abre um novo caixa, cadastrando o operador responsável.
   Impede abertura se já existe um caixa ativo, ou se os
   limites máximos de operadores ou caixas foram atingidos. */
void abrir_caixa() {

    /* Verifica se já existe um caixa aberto na sessão atual */
    if (caixa_atual != -1 && caixas[caixa_atual].aberto) {
        printf("\nCaixa ja esta aberto!\n");
        return;
    }

    /* Verifica se o limite máximo de operadores foi atingido */
    if (qtd_operadores >= 10) {
        printf("\nLimite de operadores atingido!\n");
        return;
    }

    /* Verifica se o limite máximo de caixas foi atingido */
    if (qtd_caixas >= 10) {
        printf("\nLimite de caixas atingido!\n");
        return;
    }

    /* Lê os dados do operador: código e nome */
    printf("\n--- Dados do Operador ---\n");
    printf("Codigo do operador: ");
    scanf("%d", &operadores[qtd_operadores].codigo);
    printf("Nome do operador: ");
    scanf("%s", operadores[qtd_operadores].nome);

    /* Registra o operador atual e incrementa o contador */
    operador_atual = qtd_operadores;
    qtd_operadores++;

    /* Lê o número do caixa que será aberto */
    printf("\n--- Dados do Caixa ---\n");
    printf("Numero do caixa: ");
    scanf("%d", &caixas[qtd_caixas].numero);

    /* Marca o caixa como aberto e associa ao operador cadastrado */
    caixas[qtd_caixas].aberto       = 1;
    caixas[qtd_caixas].operador_idx = operador_atual;

    /* Registra o caixa atual e incrementa o contador */
    caixa_atual = qtd_caixas;
    qtd_caixas++;

    /* Confirma a abertura do caixa com os dados informados */
    printf("\nCaixa %d aberto com sucesso pelo operador %s!\n",
           caixas[caixa_atual].numero,
           operadores[operador_atual].nome);
}

/* Cadastra um novo produto no sistema.
   Só é permitido com um caixa aberto e dentro do limite de 100 produtos. */
void cadastrar_produto() {

    /* Verifica se há um caixa aberto antes de permitir o cadastro */
    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nAbra o caixa primeiro!\n");
        return;
    }

    /* Verifica se o limite máximo de produtos foi atingido */
    if (qtd_produtos >= 100) {
        printf("\nLimite de produtos atingido!\n");
        return;
    }

    /* Lê os dados do produto: código, nome e preço */
    printf("\nCodigo do produto: ");
    scanf("%d", &produtos[qtd_produtos].codigo);

    printf("Nome do produto: ");
    scanf("%s", produtos[qtd_produtos].nome);

    printf("Preco do produto: ");
    scanf("%f", &produtos[qtd_produtos].preco);

    /* Incrementa o contador de produtos cadastrados */
    qtd_produtos++;

    printf("\nProduto cadastrado com sucesso!\n");
}

/* Registra uma compra, permitindo adicionar múltiplos produtos.
   Busca cada produto pelo código, calcula o subtotal e acumula
   o total da compra. Ao final, soma ao faturamento do dia. */
void iniciar_compra() {

    /* Verifica se o caixa está aberto antes de iniciar a compra */
    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nCaixa fechado!\n");
        return;
    }

    /* Limpa os dados da compra anterior para iniciar uma nova */
    qtd_itens_compra = 0;
    total_compra     = 0;

    int continuar = 1;

    /* Loop principal: continua adicionando produtos enquanto o
       operador confirmar que deseja adicionar mais itens */
    while (continuar) {

        int codigo, quantidade, encontrado, i;

        printf("\nDigite o codigo do produto: ");
        scanf("%d", &codigo);

        encontrado = -1; // -1 indica que o produto ainda não foi encontrado

        /* Percorre o vetor de produtos procurando pelo código informado */
        for (i = 0; i < qtd_produtos; i++) {
            if (codigo == produtos[i].codigo) {
                encontrado = i; // guarda o índice do produto encontrado
                break;
            }
        }

        /* Se o produto foi encontrado, solicita a quantidade e registra o item */
        if (encontrado != -1) {

            printf("Quantidade: ");
            scanf("%d", &quantidade);

            /* Calcula o subtotal do item (preço × quantidade) */
            float subtotal = produtos[encontrado].preco * quantidade;

            /* Armazena o item na lista de compras com seus dados */
            itens_compra[qtd_itens_compra].produto_idx = encontrado;
            itens_compra[qtd_itens_compra].quantidade  = quantidade;
            itens_compra[qtd_itens_compra].subtotal    = subtotal;
            qtd_itens_compra++;

            /* Acumula o subtotal no total geral da compra */
            total_compra += subtotal;

            printf("Produto adicionado! Subtotal: R$ %.2f\n", subtotal);

        } else {
            /* Informa que o código digitado não corresponde a nenhum produto */
            printf("\nProduto nao encontrado!\n");
        }

        /* Pergunta se o operador deseja continuar adicionando produtos */
        printf("\nDeseja adicionar mais produtos a compra? (1 - Sim / 0 - Nao): ");
        scanf("%d", &continuar);
    }

    /* Se ao menos um produto foi adicionado, finaliza e soma ao faturamento */
    if (qtd_itens_compra > 0) {
        faturamento += total_compra;
        printf("\nCompra finalizada! Total: R$ %.2f\n", total_compra);
    } else {
        /* Informa que a compra foi encerrada sem nenhum item */
        printf("\nNenhum produto foi adicionado a compra.\n");
    }
}

/* Emite o cupom fiscal da última compra realizada.
   Exibe informações do caixa, operador, todos os itens
   com seus preços e quantidades, e o total da compra. */
void emitir_cupom() {

    /* Verifica se existe uma compra com itens para emitir o cupom */
    if (qtd_itens_compra == 0) {
        printf("\nNenhuma compra realizada!\n");
        return;
    }

    int i;

    printf("\n======= CUPOM FISCAL =======\n");

    /* Exibe os dados do caixa e do operador responsável */
    if (caixa_atual != -1) {
        printf("Caixa    : %d\n", caixas[caixa_atual].numero);

        int op_idx = caixas[caixa_atual].operador_idx;
        if (op_idx != -1) {
            printf("Operador : [%d] %s\n",
                   operadores[op_idx].codigo,
                   operadores[op_idx].nome);
        }
    }

    /* Exibe o cabeçalho da tabela de itens */
    printf("----------------------------\n");
    printf("%-6s %-20s %8s %5s %10s\n",
           "Cod.", "Produto", "Preco", "Qtd", "Subtotal");
    printf("----------------------------\n");

    /* Percorre todos os itens da compra e imprime cada linha do cupom */
    for (i = 0; i < qtd_itens_compra; i++) {
        int idx = itens_compra[i].produto_idx; // índice do produto no vetor global

        /* Imprime código, nome, preço unitário, quantidade e subtotal do item */
        printf("%-6d %-20s R$%6.2f %5d R$%8.2f\n",
               produtos[idx].codigo,
               produtos[idx].nome,
               produtos[idx].preco,
               itens_compra[i].quantidade,
               itens_compra[i].subtotal);
    }

    /* Exibe o valor total da compra ao final do cupom */
    printf("----------------------------\n");
    printf("Total  : R$ %.2f\n", total_compra);
    printf("============================\n");
}

/* Fecha o caixa atual, exibindo o faturamento total acumulado.
   Impede o fechamento se o caixa já estiver fechado.
   Reseta os índices de caixa e operador ativos. */
void fechar_caixa() {

    /* Verifica se o caixa está realmente aberto antes de fechar */
    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nCaixa ja esta fechado!\n");
        return;
    }

    /* Marca o caixa como fechado */
    caixas[caixa_atual].aberto = 0;

    /* Exibe o número do caixa fechado e o faturamento total da sessão */
    printf("\nCaixa %d fechado!\n", caixas[caixa_atual].numero);
    printf("Faturamento total: R$ %.2f\n", faturamento);

    /* Reseta os índices ativos, indicando que não há caixa/operador em uso */
    caixa_atual    = -1;
    operador_atual = -1;
}

// ===================== MAIN =====================

/* Função principal: exibe o menu em loop e chama a função
   correspondente à opção escolhida pelo usuário.
   O sistema continua rodando até que a opção 6 (Sair) seja escolhida. */
int main() {

    int opcao;

    do {

        /* Exibe o menu de opções do sistema de caixa */
        printf("\n========= CAIXA =========\n");
        printf("1 - Abrir o Caixa\n");
        printf("2 - Cadastrar Produto\n");
        printf("3 - Iniciar Compra\n");
        printf("4 - Emitir Cupom Fiscal\n");
        printf("5 - Fechar o Caixa\n");
        printf("6 - Sair\n");
        printf("=========================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        /* Direciona para a função correta conforme a opção digitada */
        switch (opcao) {
            case 1: abrir_caixa();       break; // abre o caixa com um operador
            case 2: cadastrar_produto(); break; // cadastra um novo produto
            case 3: iniciar_compra();    break; // registra uma nova compra
            case 4: emitir_cupom();      break; // imprime o cupom da última compra
            case 5: fechar_caixa();      break; // fecha o caixa e exibe faturamento
            case 6: printf("\nEncerrando sistema...\n"); break; // encerra o programa
            default: printf("\nOpcao invalida!\n"); // trata entradas fora do intervalo
        }

    } while (opcao != 6); // repete até o usuário escolher sair

    return 0;
}
