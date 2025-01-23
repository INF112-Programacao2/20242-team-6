#include "Carrinho.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// construtor
Carrinho::Carrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque) : cliente(cliente), caixa(caixa), estoque(estoque){}

// getters
Cliente& Carrinho::getCliente() const{    return cliente; }
Funcionario* Carrinho::getCaixa() const{    return caixa; }
Estoque& Carrinho::getEstoque() const{     return estoque; }
const std::map<std::string, std::pair<int, double>>& Carrinho::getResumoCarrinho() const{   return resumoCarrinho; }
double Carrinho::getValorTotal() const{     return valor_total;}
int Carrinho::getQuantidadeProduto() const{     return quantidade_produto;}

// setters
void Carrinho::setValorTotal(double valor_total){   this->valor_total = valor_total; }
void Carrinho::setQuantidadeProduto(int quantidade_produto){    this->quantidade_produto = quantidade_produto; }

// 
void Carrinho::realizarCompra(){
    // armazena nome do lote(produto)
    std::string nome_produto{""};

    // guarda o valor total do carrinho
    setValorTotal(0);

    while(true){
        // guarda a quantidade de produtos de um tipo
        setQuantidadeProduto(0);

        // procura os produtos no estoque
        std::cout << "Insira o nome do produto (ok para finalizar compra): ";
        std::getline(std::cin, nome_produto);

        if(nome_produto == "ok"){
            break;      // quebra a execução do loop
        }

        // busca no estoque
        Lote* lote = getEstoque().buscarProdutoPorNome(nome_produto);

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

            std::cin.ignore(); // limpa o buffer para a proxima entrada

            // adiciona produtos no carrinho
            for(int i=0; i<quantidade_produto; i++){
                produto_carrinho.push_back(lote->pesquisarProduto(i));
            }

            // calcula o valor parcial do carrinho
            valor_total += lote->getProdutosPreco()*quantidade_produto;

            // Limpa o map para evitar acúmulo
            resumoCarrinho.clear();
            
            // Imprime o carrinho atualizado com agrupamento por nome de produto
            std::cout << "--------------------------------------------------------------------------\n";
            std::cout << "Carrinho atualizado:\n";
            std::cout << "Quantidade  Produto                            Preco Unitario  Preco Total\n";

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
                        << std::setw(35) << std::left << nome
                        << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                        << std::setw(16) << precoTotal << std::endl;

                subtotal += precoTotal;  // Soma ao subtotal
                setValorTotal(subtotal); // guarda valor total
            }

            std::cout << "--------------------------------------------------------------------------\n";
            std::cout << "Subtotal acumulado: R$" << std::fixed << std::setprecision(2) << subtotal << std::endl;
            std::cout << "--------------------------------------------------------------------------\n";



            // remove os produtos do lote
            lote->removerProdutos(quantidade_produto);


        }catch(std::exception& e){
            std::cerr << "Erro: " << e.what() << std::endl;
        }    
    }     
}

