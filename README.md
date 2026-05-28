
Conversa com o Gemini
faça um documento mark down desse codigo usando os comentarios do codigo como base



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

O seu arquivo Markdown foi gerado com sucesso utilizando todos os comentários e as estruturas lógicas do código fornecido como base. Ele está organizado de forma profissional, documentando os participantes, structs, variáveis globais e o fluxo de cada função do sistema.

O arquivo Markdown está pronto
Ícone do app MD
 Documentac...a_Mercadao 
MD

Abaixo, você também pode visualizar a estrutura do conteúdo gerado:

Markdown
# Documentação do Sistema de Caixa — Mercadão da Eskina Wyden

Esta documentação detalha a estrutura de dados, variáveis globais e funções que compõem o sistema de gerenciamento de caixa desenvolvido em linguagem C para o **Mercadão da Eskina Wyden**.

---

## 👥 Participantes
* **Michel da Silva Moreira** — RA: `202601387642`
* **Rute Carboni Moreira** — RA: `202601733461`
* **Amanda Araujo Sanches** — RA: `202602499452`
* **Manuelly Tristão de Carvalho** — RA: `202602113643`
* **Daniel Savala Lhoretti** — RA: `202601486021`

---

## 🏗️ Estruturas de Dados (`Structs`)

O sistema utiliza quatro estruturas principais para organizar os dados do negócio:

### 1. `Produto`
Representa um produto disponível para venda no sistema.
* `int codigo`: Código identificador exclusivo.
* `char nome[30]`: Nome comercial do produto.
* `float preco`: Preço unitário.

### 2. `Operador`
Representa um operador (funcionário) responsável pela operação de um ou mais caixas.
* `int codigo`: Código identificador do funcionário.
* `char nome[30]`: Nome do operador.

### 3. `Caixa`
Representa um ponto de venda (caixa física) do estabelecimento.
* `int numero`: Número identificador do caixa.
* `int aberto`: Estado atual do caixa (`1` para aberto, `0` para fechado).
* `int operador_idx`: Índice correspondente ao operador responsável no vetor global de operadores.

### 4. `ItemCompra`
Representa uma linha ou item pertencente a uma compra que está em andamento.
* `int produto_idx`: Índice associado ao produto correspondente no vetor global de produtos.
* `int quantidade`: Quantidade total do item escolhida pelo cliente.
* `float subtotal`: Valor financeiro calculado para o item (`preço × quantidade`).

---

## 💾 Dados Globais (Variáveis e Vetores)

O controle do estado da aplicação e o armazenamento em memória utilizam as seguintes variáveis:

### Vetores de Cadastros e Limites
* `Produto produtos[100]`: Vetor que armazena todos os produtos cadastrados.
* `int qtd_produtos`: Contador dinâmico de produtos ativos (limite máximo de 100).
* `Operador operadores[10]`: Vetor que registra os operadores que abriram caixas na sessão.
* `int qtd_operadores`: Contador dinâmico de operadores registrados (limite máximo de 10).
* `Caixa caixas[10]`: Vetor com todos os caixas instanciados.
* `int qtd_caixas`: Contador dinâmico de caixas criados (limite máximo de 10).

### Estados de Controle Ativo
* `int caixa_atual`: Índice do caixa que está operando no momento. Inicializado com `-1` (nenhum ativo).
* `int operador_atual`: Índice do operador ativo no momento. Inicializado com `-1` (nenhum ativo).

### Controle de Compra Vigente
* `ItemCompra itens_compra[100]`: Vetor contendo os itens adicionados na compra atual.
* `int qtd_itens_compra`: Quantidade de itens presentes na transação em andamento.

### Indicadores Financeiros
* `float total_compra`: Valor acumulado parcial ou total da compra corrente.
* `float faturamento`: Valor acumulado de todas as compras validadas ao longo do dia na sessão.

---

## ⚙️ Funções do Sistema

Abaixo estão detalhados os comportamentos e regras de negócio de cada uma das rotinas implementadas:

### `abrir_caixa()`
Realiza a abertura de um novo caixa e vincula um funcionário operador a ele.
* **Regras de Validação:**
    * Impede a ação se já houver um caixa marcado como aberto na sessão atual.
    * Bloqueia novos registros se o limite máximo de 10 operadores ou de 10 caixas tiver sido atingido.
* **Fluxo:**
    1. Solicita e lê o código e nome do operador responsável, adicionando-o ao vetor global.
    2. Define o operador atual.
    3. Solicita e lê o número do novo caixa.
    4. Define o estado do caixa como aberto (`1`) e associa o índice do operador.
    5. Exibe mensagem de confirmação em tela.

### `cadastrar_produto()`
Efetua o cadastro de novos itens comercializáveis dentro do sistema do mercado.
* **Regras de Validação:**
    * Exige obrigatoriamente que um caixa esteja aberto antes de aceitar novos produtos.
    * Impede novos registros caso a capacidade total de armazenamento (`100` produtos) tenha sido atingida.
* **Fluxo:**
    1. Solicita e faz a leitura de: Código, Nome e Preço Unitário.
    2. Adiciona os dados na posição livre do vetor global e incrementa o contador `qtd_produtos`.

### `iniciar_compra()`
Abre e gerencia uma transação de venda de itens ao consumidor.
* **Regras de Validação:**
    * Requer que o caixa ativo esteja aberto para permitir o início da operação.
* **Fluxo:**
    1. Limpa os registros de itens e zera as variáveis da compra anterior (`qtd_itens_compra = 0`, `total_compra = 0`).
    2. Entra em um loop interativo (`while`) onde o operador informa o código do produto.
    3. Varre o banco de dados interno procurando o código informado.
    4. Caso localizado, lê a quantidade, calcula o subtotal (`preço × quantidade`), adiciona a estrutura no vetor de itens ativos e atualiza o `total_compra`.
    5. Caso não localizado, emite um aviso de produto inexistente.
    6. Pergunta ao operador se ele deseja encerrar ou continuar adicionando itens.
    7. No fechamento do loop, caso haja itens válidos, o valor acumulado é integralizado no `faturamento` total do dia.

### `emitir_cupom()`
Gera em tela o espelho do Cupom Fiscal referente à última compra realizada com sucesso.
* **Regras de Validação:**
    * Retorna erro e interrompe a execução caso nenhuma compra tenha sido registrada na sessão.
* **Fluxo:**
    1. Exibe um cabeçalho formatado com as informações de identificação do Caixa e os dados do Operador encarregado.
    2. Monta uma tabela tabulada contendo as colunas: Código (`Cod.`), Nome (`Produto`), Preço Unitário, Quantidade adquirida e o respectivo Subtotal.
    3. Imprime uma listagem detalhada percorrendo todos os produtos da transação atual.
    4. Finaliza apresentando a soma final formatada em reais (`Total: R$`).

### `fechar_caixa()`
Responsável pelo encerramento das atividades do caixa corrente.
* **Regras de Validação:**
    * Impede a execução do fluxo se o caixa selecionado já constar como fechado ou desativado.
* **Fluxo:**
    1. Altera a flag de estado `aberto` do caixa ativo para `0` (fechado).
    2. Apresenta o relatório de fechamento exibindo o número do caixa desativado juntamente ao saldo de `faturamento total` obtido na sessão de vendas.
    3. Reseta os ponteiros de índices (`caixa_atual` e `operador_atual`) de volta para `-1`.

### `main()`
Ponto de entrada (`Entry Point`) padrão do programa em C.
* **Fluxo:**
    1. Exec



O Gemini é uma IA e pode cometer erros.

# Documentação do Sistema de Caixa — Mercadão da Eskina Wyden

Esta documentação detalha a estrutura de dados, variáveis globais e funções que compõem o sistema de gerenciamento de caixa desenvolvido em linguagem C para o **Mercadão da Eskina Wyden**.

---

## 👥 Participantes
* **Michel da Silva Moreira** — RA: `202601387642`
* **Rute Carboni Moreira** — RA: `202601733461`
* **Amanda Araujo Sanches** — RA: `202602499452`
* **Manuelly Tristão de Carvalho** — RA: `202602113643`
* **Daniel Savala Lhoretti** — RA: `202601486021`

---

## 🏗️ Estruturas de Dados (`Structs`)

O sistema utiliza quatro estruturas principais para organizar os dados do negócio:

### 1. `Produto`
Representa um produto disponível para venda no sistema.
* `int codigo`: Código identificador exclusivo.
* `char nome[30]`: Nome comercial do produto.
* `float preco`: Preço unitário.

### 2. `Operador`
Representa um operador (funcionário) responsável pela operação de um ou mais caixas.
* `int codigo`: Código identificador do funcionário.
* `char nome[30]`: Nome do operador.

### 3. `Caixa`
Representa um ponto de venda (caixa física) do estabelecimento.
* `int numero`: Número identificador do caixa.
* `int aberto`: Estado atual do caixa (`1` para aberto, `0` para fechado).
* `int operador_idx`: Índice correspondente ao operador responsável no vetor global de operadores.

### 4. `ItemCompra`
Representa uma linha ou item pertencente a uma compra que está em andamento.
* `int produto_idx`: Índice associado ao produto correspondente no vetor global de produtos.
* `int quantidade`: Quantidade total do item escolhida pelo cliente.
* `float subtotal`: Valor financeiro calculado para o item (`preço × quantidade`).

---

## 💾 Dados Globais (Variáveis e Vetores)

O controle do estado da aplicação e o armazenamento em memória utilizam as seguintes variáveis:

### Vetores de Cadastros e Limites
* `Produto produtos[100]`: Vetor que armazena todos os produtos cadastrados.
* `int qtd_produtos`: Contador dinâmico de produtos ativos (limite máximo de 100).
* `Operador operadores[10]`: Vetor que registra os operadores que abriram caixas na sessão.
* `int qtd_operadores`: Contador dinâmico de operadores registrados (limite máximo de 10).
* `Caixa caixas[10]`: Vetor com todos os caixas instanciados.
* `int qtd_caixas`: Contador dinâmico de caixas criados (limite máximo de 10).

### Estados de Controle Ativo
* `int caixa_atual`: Índice do caixa que está operando no momento. Inicializado com `-1` (nenhum ativo).
* `int operador_atual`: Índice do operador ativo no momento. Inicializado com `-1` (nenhum ativo).

### Controle de Compra Vigente
* `ItemCompra itens_compra[100]`: Vetor contendo os itens adicionados na compra atual.
* `int qtd_itens_compra`: Quantidade de itens presentes na transação em andamento.

### Indicadores Financeiros
* `float total_compra`: Valor acumulado parcial ou total da compra corrente.
* `float faturamento`: Valor acumulado de todas as compras validadas ao longo do dia na sessão.

---

## ⚙️ Funções do Sistema

Abaixo estão detalhados os comportamentos e regras de negócio de cada uma das rotinas implementadas:

### `abrir_caixa()`
Realiza a abertura de um novo caixa e vincula um funcionário operador a ele.
* **Regras de Validação:**
    * Impede a ação se já houver um caixa marcado como aberto na sessão atual.
    * Bloqueia novos registros se o limite máximo de 10 operadores ou de 10 caixas tiver sido atingido.
* **Fluxo:**
    1. Solicita e lê o código e nome do operador responsável, adicionando-o ao vetor global.
    2. Define o operador atual.
    3. Solicita e lê o número do novo caixa.
    4. Define o estado do caixa como aberto (`1`) e associa o índice do operador.
    5. Exibe mensagem de confirmação em tela.

### `cadastrar_produto()`
Efetua o cadastro de novos itens comercializáveis dentro do sistema do mercado.
* **Regras de Validação:**
    * Exige obrigatoriamente que um caixa esteja aberto antes de aceitar novos produtos.
    * Impede novos registros caso a capacidade total de armazenamento (`100` produtos) tenha sido atingida.
* **Fluxo:**
    1. Solicita e faz a leitura de: Código, Nome e Preço Unitário.
    2. Adiciona os dados na posição livre do vetor global e incrementa o contador `qtd_produtos`.

### `iniciar_compra()`
Abre e gerencia uma transação de venda de itens ao consumidor.
* **Regras de Validação:**
    * Requer que o caixa ativo esteja aberto para permitir o início da operação.
* **Fluxo:**
    1. Limpa os registros de itens e zera as variáveis da compra anterior (`qtd_itens_compra = 0`, `total_compra = 0`).
    2. Entra em um loop interativo (`while`) onde o operador informa o código do produto.
    3. Varre o banco de dados interno procurando o código informado.
    4. Caso localizado, lê a quantidade, calcula o subtotal (`preço × quantidade`), adiciona a estrutura no vetor de itens ativos e atualiza o `total_compra`.
    5. Caso não localizado, emite um aviso de produto inexistente.
    6. Pergunta ao operador se ele deseja encerrar ou continuar adicionando itens.
    7. No fechamento do loop, caso haja itens válidos, o valor acumulado é integralizado no `faturamento` total do dia.

### `emitir_cupom()`
Gera em tela o espelho do Cupom Fiscal referente à última compra realizada com sucesso.
* **Regras de Validação:**
    * Retorna erro e interrompe a execução caso nenhuma compra tenha sido registrada na sessão.
* **Fluxo:**
    1. Exibe um cabeçalho formatado com as informações de identificação do Caixa e os dados do Operador encarregado.
    2. Monta uma tabela tabulada contendo as colunas: Código (`Cod.`), Nome (`Produto`), Preço Unitário, Quantidade adquirida e o respectivo Subtotal.
    3. Imprime uma listagem detalhada percorrendo todos os produtos da transação atual.
    4. Finaliza apresentando a soma final formatada em reais (`Total: R$`).

### `fechar_caixa()`
Responsável pelo encerramento das atividades do caixa corrente.
* **Regras de Validação:**
    * Impede a execução do fluxo se o caixa selecionado já constar como fechado ou desativado.
* **Fluxo:**
    1. Altera a flag de estado `aberto` do caixa ativo para `0` (fechado).
    2. Apresenta o relatório de fechamento exibindo o número do caixa desativado juntamente ao saldo de `faturamento total` obtido na sessão de vendas.
    3. Reseta os ponteiros de índices (`caixa_atual` e `operador_atual`) de volta para `-1`.

### `main()`
Ponto de entrada (`Entry Point`) padrão do programa em C.
* **Fluxo:**
    1. Executa uma estrutura de repetição contínua (`do-while`) responsável por renderizar a interface textual de menu de opções do caixa (Opções de 1 a 6).
    2. Captura a escolha numérica do usuário através do terminal.
    3. Redireciona a chamada do sistema para a respectiva função através de uma estrutura condicional `switch-case`.
    4. Finaliza a execução do programa de forma limpa apenas quando a opção escolhida for igual a `6` (Sair).
Documentacao_Caixa_Mercadao.md
Exibindo Documentacao_Caixa_Mercadao.md.
