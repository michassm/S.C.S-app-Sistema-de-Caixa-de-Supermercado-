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

typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct {
    int codigo;
    char nome[30];
} Operador;

typedef struct {
    int numero;
    int aberto;
    int operador_idx;
} Caixa;

typedef struct {
    int produto_idx;
    int quantidade;
    float subtotal;
} ItemCompra;

// ===================== DADOS GLOBAIS =====================

Produto produtos[100];
int qtd_produtos = 0;

Operador operadores[10];
int qtd_operadores = 0;

Caixa caixas[10];
int qtd_caixas = 0;

int caixa_atual    = -1;
int operador_atual = -1;

ItemCompra itens_compra[100];
int qtd_itens_compra = 0;

float total_compra = 0;
float faturamento  = 0;

// ===================== FUNÇÕES =====================

void abrir_caixa() {

    if (caixa_atual != -1 && caixas[caixa_atual].aberto) {
        printf("\nCaixa ja esta aberto!\n");
        return;
    }

    if (qtd_operadores >= 10) {
        printf("\nLimite de operadores atingido!\n");
        return;
    }

    if (qtd_caixas >= 10) {
        printf("\nLimite de caixas atingido!\n");
        return;
    }

    printf("\n--- Dados do Operador ---\n");
    printf("Codigo do operador: ");
    scanf("%d", &operadores[qtd_operadores].codigo);
    printf("Nome do operador: ");
    scanf("%s", operadores[qtd_operadores].nome);

    operador_atual = qtd_operadores;
    qtd_operadores++;

    printf("\n--- Dados do Caixa ---\n");
    printf("Numero do caixa: ");
    scanf("%d", &caixas[qtd_caixas].numero);

    caixas[qtd_caixas].aberto       = 1;
    caixas[qtd_caixas].operador_idx = operador_atual;

    caixa_atual = qtd_caixas;
    qtd_caixas++;

    printf("\nCaixa %d aberto com sucesso pelo operador %s!\n",
           caixas[caixa_atual].numero,
           operadores[operador_atual].nome);
}

void cadastrar_produto() {

    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nAbra o caixa primeiro!\n");
        return;
    }

    if (qtd_produtos >= 100) {
        printf("\nLimite de produtos atingido!\n");
        return;
    }

    printf("\nCodigo do produto: ");
    scanf("%d", &produtos[qtd_produtos].codigo);

    printf("Nome do produto: ");
    scanf("%s", produtos[qtd_produtos].nome);

    printf("Preco do produto: ");
    scanf("%f", &produtos[qtd_produtos].preco);

    qtd_produtos++;

    printf("\nProduto cadastrado com sucesso!\n");
}

void iniciar_compra() {

    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nCaixa fechado!\n");
        return;
    }

    // Limpa compra anterior
    qtd_itens_compra = 0;
    total_compra     = 0;

    int continuar = 1;

    while (continuar) {

        int codigo, quantidade, encontrado, i;

        printf("\nDigite o codigo do produto: ");
        scanf("%d", &codigo);

        encontrado = -1;

        for (i = 0; i < qtd_produtos; i++) {
            if (codigo == produtos[i].codigo) {
                encontrado = i;
                break;
            }
        }

        if (encontrado != -1) {

            printf("Quantidade: ");
            scanf("%d", &quantidade);

            float subtotal = produtos[encontrado].preco * quantidade;

            itens_compra[qtd_itens_compra].produto_idx = encontrado;
            itens_compra[qtd_itens_compra].quantidade  = quantidade;
            itens_compra[qtd_itens_compra].subtotal    = subtotal;
            qtd_itens_compra++;

            total_compra += subtotal;

            printf("Produto adicionado! Subtotal: R$ %.2f\n", subtotal);

        } else {
            printf("\nProduto nao encontrado!\n");
        }

        printf("\nDeseja adicionar mais produtos a compra? (1 - Sim / 0 - Nao): ");
        scanf("%d", &continuar);
    }

    if (qtd_itens_compra > 0) {
        faturamento += total_compra;
        printf("\nCompra finalizada! Total: R$ %.2f\n", total_compra);
    } else {
        printf("\nNenhum produto foi adicionado a compra.\n");
    }
}

void emitir_cupom() {

    if (qtd_itens_compra == 0) {
        printf("\nNenhuma compra realizada!\n");
        return;
    }

    int i;

    printf("\n======= CUPOM FISCAL =======\n");

    if (caixa_atual != -1) {
        printf("Caixa    : %d\n", caixas[caixa_atual].numero);

        int op_idx = caixas[caixa_atual].operador_idx;
        if (op_idx != -1) {
            printf("Operador : [%d] %s\n",
                   operadores[op_idx].codigo,
                   operadores[op_idx].nome);
        }
    }

    printf("----------------------------\n");
    printf("%-6s %-20s %8s %5s %10s\n",
           "Cod.", "Produto", "Preco", "Qtd", "Subtotal");
    printf("----------------------------\n");

    for (i = 0; i < qtd_itens_compra; i++) {
        int idx = itens_compra[i].produto_idx;

        printf("%-6d %-20s R$%6.2f %5d R$%8.2f\n",
               produtos[idx].codigo,
               produtos[idx].nome,
               produtos[idx].preco,
               itens_compra[i].quantidade,
               itens_compra[i].subtotal);
    }

    printf("----------------------------\n");
    printf("Total  : R$ %.2f\n", total_compra);
    printf("============================\n");
}

void fechar_caixa() {

    if (caixa_atual == -1 || !caixas[caixa_atual].aberto) {
        printf("\nCaixa ja esta fechado!\n");
        return;
    }

    caixas[caixa_atual].aberto = 0;

    printf("\nCaixa %d fechado!\n", caixas[caixa_atual].numero);
    printf("Faturamento total: R$ %.2f\n", faturamento);

    caixa_atual    = -1;
    operador_atual = -1;
}

// ===================== MAIN =====================

int main() {

    int opcao;

    do {

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

        switch (opcao) {
            case 1: abrir_caixa();       break;
            case 2: cadastrar_produto(); break;
            case 3: iniciar_compra();    break;
            case 4: emitir_cupom();      break;
            case 5: fechar_caixa();      break;
            case 6: printf("\nEncerrando sistema...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }

    } while (opcao != 6);

    return 0;
}
