#include "Carrinho.h"
#include "CaixaPCD.h"
#include "Tela.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <thread>

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
void Carrinho::gerenciarCarrinho(Estoque& estoque) {
    int opcao = -1;
    while (opcao != 0) {
        Tela::limpar();
        std::cout << "=============================================" << std::endl;
        std::cout << "                    CARRINHO                 " << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "1. Adicionar Produto\n";
        std::cout << "2. Remover Produto\n";
        std::cout << "3. Finalizar Compra\n";
        std::cout << "0. Cancelar Compra\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Tente novamente.\n";
            continue;
        }

        try {
            switch (opcao) {
                case 1: {
                    std::string nomeProduto;
                    std::cout << "Digite o nome do produto: ";
                    std::cin.ignore();
                    std::getline(std::cin, nomeProduto);
                    std::cout << "Digite a quantidade: ";
                    std::cin >> quantidade_produto;

                    if (std::cin.fail() || quantidade_produto <= 0) {
                        throw std::invalid_argument("Quantidade inválida.");
                    }

                    std::vector<std::pair<Lote *, int>> lotesDisponiveis = estoque.buscarTodosLotesPorNome(nomeProduto); // Retorna todos os lotes disponíveis
                    if (!lotesDisponiveis.empty()) {
                        adicionarProdutoAoCarrinho(nomeProduto, quantidade_produto, lotesDisponiveis);
                    } else {
                        std::cout << "Estoque insuficiente para " << nomeProduto << ".\n";
                    }
                    break;
                }

                case 2: {
                    std::string nomeProduto;
                    int quantidade;
                    std::cout << "Digite o nome do produto a ser removido: ";
                    std::cin.ignore();
                    std::getline(std::cin, nomeProduto);
                    std::cout << "Digite a quantidade: ";
                    std::cin >> quantidade;

                    if (std::cin.fail() || quantidade <= 0) {
                        throw std::invalid_argument("Quantidade invalida.");
                    }

                    if (removerProdutoDoCarrinho(nomeProduto, quantidade, estoque)) {
                        std::cout << quantidade << " unidades do produto " << nomeProduto << " removidas do carrinho.\n";
                    } else {
                        std::cout << "Não foi possível remover o produto " << nomeProduto << ".\n";
                    }
                    break;
                }

                case 3: {
                    // finalizarCompra(estoque);
                    return;
                }

                case 0: {
                    cancelarCompra(estoque);
                    return;
                }

                default:
                    std::cout << "Opcao invalida. Tente novamente.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Erro: " << e.what() << "\n";
        }

        Tela::limpar(); // limpa tela

        // Exibe o carrinho atualizado
        std::cout << "===================================================================================" << std::endl;
        std::cout << "                                CARRINHO             " << std::endl;
        std::cout << "===================================================================================" << std::endl;
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

        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        std::cout << "Subtotal acumulado: R$" << std::fixed << std::setprecision(2) << subtotal << std::endl;
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;

        // Aguarda o usuário pressionar Enter
        std::cout << "Pressione Enter para continuar...";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void Carrinho::adicionarProdutoAoCarrinho(const std::string& nomeProduto, int quantidade, const std::vector<std::pair<Lote*, int>>& lotesDisponiveis) {
    int quantidadeRestante = quantidade; // Quantidade que ainda precisa ser adicionada

    for (const auto& [lote, disponivel] : lotesDisponiveis) {
        if (quantidadeRestante <= 0) break; // Já adicionou tudo o que foi solicitado

        // Calcula a quantidade que pode ser retirada deste lote
        int quantidadeAdicionar = std::min(quantidadeRestante, disponivel);

        // Adiciona os produtos ao carrinho
        for (int i = 0; i < quantidadeAdicionar; ++i) {
            produto_carrinho.emplace_back(lote->pesquisarProduto(i));
        }

        // Atualiza o resumo do carrinho
        resumoCarrinho[nomeProduto].first += quantidadeAdicionar;
        resumoCarrinho[nomeProduto].second = lote->getProdutosPreco();

        // Atualiza o vetor de pares de lotes
        bool loteExistente = false;
        for (auto& par : paresLote) {
            if (par.first == lote) {
                par.second += quantidadeAdicionar;
                loteExistente = true;
                break;
            }
        }
        if (!loteExistente) {
            paresLote.emplace_back(lote, quantidadeAdicionar);
        }

        // Remove os produtos do lote
        lote->removerProdutos(quantidadeAdicionar);

        // Atualiza a quantidade restante a ser adicionada
        quantidadeRestante -= quantidadeAdicionar;
    }

    if (quantidadeRestante > 0) {
        std::cout << "Estoque insuficiente. Apenas " << (quantidade - quantidadeRestante)
                  << " unidades do produto " << nomeProduto << " foram adicionadas ao carrinho.\n";
    } else {
        std::cout << "Todas as " << quantidade << " unidades do produto " << nomeProduto
                  << " foram adicionadas ao carrinho.\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // espera 2 segundos
}

// remove produto do carrinho
bool Carrinho::removerProdutoDoCarrinho(const std::string& nomeProduto, int quantidade, Estoque& estoque) {
    if (resumoCarrinho.find(nomeProduto) == resumoCarrinho.end() || resumoCarrinho[nomeProduto].first < quantidade) {
        return false; // Produto não está no carrinho ou quantidade insuficiente
    }

    resumoCarrinho[nomeProduto].first -= quantidade;

    // Itera sobre os pares de lote para devolver os produtos ao estoque
    for (auto it = paresLote.begin(); it != paresLote.end();) {
        if (it->first->getNome() == nomeProduto) {
            int qtdRestaurar = std::min(it->second, quantidade);
            it->first->adicionarProduto(qtdRestaurar);
            quantidade -= qtdRestaurar;

            // Remove o par do vetor se a quantidade for restaurada completamente
            if (qtdRestaurar == it->second) {
                it = paresLote.erase(it);
            } else {
                it->second -= qtdRestaurar;
                ++it;
            }

            if (quantidade <= 0) break;
        } else {
            ++it;
        }
    }

    return true;
}

void Carrinho::finalizarCompra(Estoque& estoque) {
    for (const auto& par : paresLote) {
        par.first->removerProdutos(par.second); // Remove os produtos do lote
    }
}

void Carrinho::cancelarCompra(Estoque& estoque) {
    for (const auto& par : paresLote) {
        par.first->adicionarProduto(par.second); // Restaura os produtos ao lote
    }

    // Limpa os dados do carrinho após o cancelamento
    produto_carrinho.clear();
    resumoCarrinho.clear();
    paresLote.clear();
}