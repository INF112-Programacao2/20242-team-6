#ifndef LOTE_H
#define LOTE_H

#include <string>
#include <vector>
#include <chrono>
#include "Produto.h"

class Lote{
private:
    std::string nome;  // nome do produto
    std::string codigo;
    std::string dataValidadeStr;
    int tamanho;
    std::vector<Produto> produtos;
    std::chrono::system_clock::time_point dataValidade; 
    
public:
    // Construtor
    Lote(std::string nome, std::string codigo, const std::string& dataValidadeStr, int tamanho);
    
    // Métodos getters
    std::string getNome() const;
    std::string getCodigo() const;
    int getTamanho() const;
    std::string getValidade() const;

    // retorna um produto do lote pelo índice
    const Produto& pesquisarProduto(int indice) const;

    // Método para adicionar um produto ao lote
    void adicionarProduto(const Produto& produto);

    // Método para remover uma quantidade de produtos do lote
    void removerProdutos(int quantidade);

    // cria os produtos que serão adicionados ao lote
    void preencherLote(const std::string& nome, const int& id, double preco, const std::string& descricao);

    // Método para verificar se o produto está vencido
    bool verificarValidade() const;    
};

#endif