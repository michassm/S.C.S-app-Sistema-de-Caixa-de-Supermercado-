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
