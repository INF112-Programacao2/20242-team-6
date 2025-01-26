#ifndef CARRINHO_H
#define CARRINHO_H

#include "Cliente.h"
#include "Funcionario.h"
#include "Estoque.h"
#include "Produto.h"
#include <vector>
#include <map>

class Carrinho{
private:
    Cliente& cliente;
    Funcionario* caixa;
    Estoque& estoque;
    std::vector<Produto> produto_carrinho; // Produtos no carrinho
    std::map<std::string, std::pair<int, double>> resumoCarrinho;  // Map para agrupar produtos
    std::vector<std::pair<Lote*, int>> paresLote; // Rastreamento dos lotes e quantidades
    double valor_total;
    int quantidade_produto;
public:
    // construtor
    Carrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque);
    
    // getters
    Cliente& getCliente() const;
    Funcionario* getCaixa() const;
    Estoque& getEstoque() const;
    const std::map<std::string, std::pair<int, double>>& getResumoCarrinho() const;
    double getValorTotal() const;
    int getQuantidadeProduto() const;

    // setters
    void setValorTotal(double valor_total);
    void setQuantidadeProduto(int quantidade_produto);

    // gerencia o carrinho durante a compra
    void gerenciarCarrinho(Estoque& estoque);

    void adicionarProdutoAoCarrinho(const std::string& nomeProduto, int quantidade, const std::vector<std::pair<Lote*, int>>& lotes);

    bool removerProdutoDoCarrinho(const std::string& nomeProduto, int quantidade, Estoque& estoque);

    void finalizarCompra(Estoque& estoque);

    void cancelarCompra(Estoque& estoque);
};

#endif