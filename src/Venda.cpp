#include "Venda.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <map>

// método para iniciar o carrinho 
void Venda::iniciarVenda(Funcionario* caixa, Estoque& estoque){
    // armazena nome e cpf do cliente
    std::string nome_cliente;
    std::string cpf_cliente;

    // pedir nome do cliente
    std::cout << "Nome do cliente: ";
    std::cin.ignore(); // Limpa buffer
    std::getline(std::cin, nome_cliente);

    // pedir cpf do cliente
    std::cout << "CPF do cliente: ";
    std::cin >> cpf_cliente;
    
    // cria cliente
    Cliente cliente(nome_cliente, cpf_cliente);

    // adiciona os produtos no carrinho
    abrirCarrinho(cliente, caixa, estoque);

    // atualiza o estoque no arquivo
    estoque.salvarEstoqueNoArquivo("data/estoque.txt");
    
}

// abre o carrinho de compras
void Venda::abrirCarrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque){
    // armazena nome do lote(produto)
    std::string nome_produto{""};

    // vetor que armazena os produtos do carrinho
    std::vector<Produto> produto_carrinho;

    // guarda o valor total do carrinho
    double valor_total = 0;

    while(true){
        // guarda a quantidade de produtos de um tipo
        int quantidade_produto = 0;

        // procura os produtos no estoque
        std::cout << "Insira o nome do produto (ok para finalizar compra): ";
        std::cin.ignore(); // limpa buffer
        std::getline(std::cin, nome_produto);

        // verificar problema na entrada de dados aqui!

        //depuração
        std::cout << nome_produto << std::endl;

        if(nome_produto == "ok"){
            break;      // quebra a execução do loop
        }

        // busca no estoque
        Lote* lote = estoque.buscarProdutoPorNome(nome_produto);

        try{
            if(lote == nullptr){
                throw std::invalid_argument("Produto nao encontrado.");
            }
            if(lote->verificarValidade()){
                throw std::runtime_error("Produtos deste lote estao fora da validade.");
            }

            // confirma a quantidade de produtos a ser adicionada no carrinho
            std::cout << "Adicionar qual quantidade de " << lote->getNome() << " no carrinho? (0 para voltar): ";
            std::cin >> quantidade_produto;

            // trata erro de entrada
            if (std::cin.fail() || quantidade_produto < 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Entrada invalida. Insira um numero valido.");
            }

            // adiciona produtos no carrinho
            for(int i=0; i<quantidade_produto; i++){
                produto_carrinho.push_back(lote->pesquisarProduto(i));
            }

            // calcula o valor parcial do carrinho
            valor_total += lote->getProdutosPreco()*quantidade_produto;

            
            // Imprime o carrinho atualizado com agrupamento por nome de produto
            std::cout << "--------------------------------------------------------------------------\n";
            std::cout << "Carrinho atualizado:\n";
            std::cout << "Quantidade  Produto             Preco Unitario  Preco Total\n";

            std::map<std::string, std::pair<int, double>> resumoCarrinho;  // Map para agrupar produtos

            // Agrupa os produtos por nome e soma quantidades
            for (const auto& produto : produto_carrinho) {
                if (resumoCarrinho.find(produto.getNome()) == resumoCarrinho.end()) {
                    resumoCarrinho[produto.getNome()] = {1, produto.getPreco()};  // Adiciona o produto
                } else {
                    resumoCarrinho[produto.getNome()].first++;  // Incrementa a quantidade
                }
            }

            // Exibe os produtos agrupados
            double subtotal = 0;  // Subtotal acumulado
            for (const auto& [nome, detalhes] : resumoCarrinho) {
                int quantidade = detalhes.first;
                double precoUnitario = detalhes.second;
                double precoTotal = quantidade * precoUnitario;

                std::cout << std::setw(10) << std::left << quantidade << "  "
                        << std::setw(20) << std::left << nome
                        << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                        << std::setw(16) << precoTotal << std::endl;

                subtotal += precoTotal;  // Soma ao subtotal
            }

            std::cout << "--------------------------------------------------------------------------\n";
            std::cout << "Subtotal acumulado: " << std::fixed << std::setprecision(2) << subtotal << std::endl;
            std::cout << "--------------------------------------------------------------------------\n";



            // remove os produtos do lote
            lote->removerProdutos(quantidade_produto);


        }catch(std::exception& e){
            std::cerr << "Erro: " << e.what() << std::endl;
        }    
    }        
}