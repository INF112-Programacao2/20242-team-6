#include "Carrinho.h"
#include "Tela.h"
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

// gerencia o carrinho durante a compra
void Carrinho::realizarCompra(){
    // armazena nome do lote(produto)
    std::string nome_produto;

    // guarda o valor total do carrinho
    setValorTotal(0);

    while(true){
        // guarda a quantidade de produtos de um tipo
        setQuantidadeProduto(0);

        Lote* lote;     // recebe o lote dos produtos do carrinho

        std::pair<Lote*, int> parLote;  // Cria um par com o lote e a quantidade desejada

        int escolha; // entrada do usuário

        std::cout << "1. Adicionar Produto\n"
                  << "2. Remover Produto\n"
                  << "3. Finalizar Compra\n"
                  << "0. Cancelar Compra\n"
                  << "Escolha uma opcao: ";
        std::cin >> escolha;
        std::cin.ignore(); // limpa o buffer 
        try{
            switch (escolha)
            {
            case 1: // adiciona produto ao carrinho

                // procura os produtos no estoque
                std::cout << "Insira o nome do produto: ";
                std::getline(std::cin, nome_produto);
                
                // busca produto no estoque
                lote = getEstoque().buscarProdutoPorNome(nome_produto);

                if(lote->getTamanho() <= 0){
                    throw std::runtime_error("Lote vazio.");
                }

                if(lote == nullptr){
                    throw std::invalid_argument("Produto não encontrado.");
                }
                if(lote->verificarValidade()){
                    throw std::runtime_error("Produtos deste lote estão fora da validade.");
                }

                // confirma a quantidade de produtos a ser adicionada no carrinho
                std::cout << "Adicionar qual quantidade de " << lote->getNome() << " no carrinho? ";
                std::cin >> quantidade_produto;

                // trata erro de entrada
                if (std::cin.fail() || quantidade_produto < 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("Entrada inválida. Insira um número válido.");
                }

                std::cin.ignore(); // limpa o buffer para a proxima entrada

                // guarda o lote no par
                parLote = std::make_pair(lote, quantidade_produto);

                // Adiciona o par ao vetor de pares
                paresLote.push_back(parLote);

                // adiciona produtos no carrinho
                for(int i=0; i<quantidade_produto; i++){
                    produto_carrinho.push_back(lote->pesquisarProduto(i));
                }

                // calcula o valor parcial do carrinho
                valor_total += lote->getProdutosPreco()*quantidade_produto;

                // Limpa o map para evitar acúmulo
                resumoCarrinho.clear();

                // Agrupa os produtos por nome e soma quantidades
                for (const auto& produto : produto_carrinho) {
                    if (resumoCarrinho.find(produto.getNome()) == resumoCarrinho.end()) {
                        resumoCarrinho[produto.getNome()] = {1, produto.getPreco()};  // Adiciona o produto
                    } else {
                        resumoCarrinho[produto.getNome()].first++;  // Incrementa a quantidade
                    }
                }   

                break; // fim de adicionar produto
            
            case 2: // removeprodutos do carrinho

                // procura o produto no carrinho
                std::cout << "Nome do produto que deseja remover: ";
                std::getline(std::cin, nome_produto);

                // confirma a quantidade de produtos a ser removida no carrinho
                std::cout << "remover qual quantidade de " << nome_produto << " no carrinho? ";
                std::cin >> quantidade_produto;

                // trata erro de entrada
                if (std::cin.fail() || quantidade_produto < 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::invalid_argument("Entrada inválida. Insira um número válido.");
                }

                std::cin.ignore(); // limpa o buffer para a proxima entrada

                // remove a quantidade de produto especificada
                removerProdutoCarrinho(nome_produto, quantidade_produto);

                for (auto& par : paresLote) {
                    if (par.first == lote) {  // Verifica se o lote é o mesmo
                        par.second -= quantidade_produto;  // Atualiza a quantidade
                    }
                }
                break; // fim de remover produto
            
            case 3: // finalizar compra

                for (const auto& par : paresLote) {
                    Lote* lote = par.first; // Ponteiro para o lote
                    int quantidade = par.second; // Quantidade retirada
                    lote->removerProdutos(quantidade); // Remove permanentemente do lote
                }

                return; // termina compra 

            case 0: // cancelar compra

                // Limpa o mapa carrinho
                resumoCarrinho.clear();

                return;
            }

            // continua execução do carrinho após adicionar ou remover produtos

            Tela::limpar(); // limpa tela

            // Imprime o carrinho atualizado com agrupamento por nome de produto
            std::cout << "--------------------------------------------------------------------------\n";
            std::cout << "Quantidade  Produto                            Preço Unitário  Preço Total\n";

            
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

        }catch(std::exception& e){
                std::cerr << "Erro: " << e.what() << std::endl;
            }

    }     
}

// remover produto do carrinho de compras
void Carrinho::removerProdutoCarrinho(const std::string& nomeProduto, int quantidadeRemover) {
    // Verifica se o produto existe no resumoCarrinho
    auto it = resumoCarrinho.find(nomeProduto);
    if (it != resumoCarrinho.end()) {
        // Decrementa a quantidade, mas não deixe a quantidade abaixo de 0
        if (it->second.first >= quantidadeRemover) {
            it->second.first -= quantidadeRemover;

            // Se a quantidade chegar a 0, remove o produto
            if (it->second.first == 0) {
                resumoCarrinho.erase(it);
            }
        } else {
            std::cout << "quantidade insuficiente para remover.\n";
        }

        // Atualiza o vetor produto_carrinho, removendo as instâncias do produto
        int quantidadeRemanescente = quantidadeRemover;
        for (auto itVec = produto_carrinho.begin(); itVec != produto_carrinho.end() && quantidadeRemanescente > 0; ) {
            if (itVec->getNome() == nomeProduto) {
                itVec = produto_carrinho.erase(itVec);
                quantidadeRemanescente--;  // Decrementa a quantidade removida
            } else {
                ++itVec;
            }
        }

        if (quantidadeRemanescente > 0) {
            std::cout << "Nao foi possivel remover a quantidade total solicitada. Restam " 
                      << quantidadeRemanescente << " itens a serem removidos.\n";
        }
    } else {
        std::cout << "Produto não encontrado.\n";
    }
}
