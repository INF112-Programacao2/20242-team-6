#include "Lote.h"
#include <iomanip>
#include <sstream>
#include <iostream>


// Construtor que recebe codigo, uma data no formato "dd/mm/aaaa" e o tamanho do lote.
Lote::Lote(std::string nome, std::string codigo, const std::string& dataStr, int tamanho) : nome(nome), codigo(codigo), dataValidadeStr(dataStr), tamanho(tamanho) {
    std::istringstream ss(dataStr);
    std::tm tm = {};
    ss >> std::get_time(&tm, "%d/%m/%Y");
    if (ss.fail()) {
        throw std::invalid_argument("Formato de data invalido. Use o formato 'dd/mm/aaaa'.");
    }
    this->dataValidade = std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Métodos getters
std::string Lote::getNome() const { return nome; }
std::string Lote::getCodigo() const{ return codigo; }
size_t Lote::getTamanho() const{ return tamanho; }
std::string Lote::getValidade() const{ return dataValidadeStr; }

// retorna um produto do lote pelo índice
const Produto& Lote::pesquisarProduto(int indice) const{
    return produtos.at(indice);
}

// Método para adicionar um produto ao lote
void Lote::adicionarProduto(const Produto& produto) {
    if (produtos.size() < getTamanho()) {
        produtos.push_back(produto); // adiciona ao lote
    } else {
        throw std::overflow_error("Lote ja esta cheio.");
    }
}

// Método para remover uma quantidade de produtos do lote
void Lote::removerProdutos(int quantidade){
    if(quantidade <= produtos.size()){
        produtos.erase(produtos.begin(), produtos.begin() + quantidade); // deleta a quantidade de produtos a partir do primeiro elemento
    } else {
        throw std::invalid_argument("Nao e possível remover esta quantidade de produtos do lote.");
    }
}

// cria os produtos que serão adicionados ao lote
void Lote::preencherLote(const std::string& nome, const int& id, double preco, const std::string& descricao){
    Produto novoproduto(nome, id, preco, descricao);
    adicionarProduto(novoproduto);

}

// Método para verificar se o produto está vencido
bool Lote::verificarValidade() const {
    auto agora = std::chrono::system_clock::now();
    return agora > dataValidade;
}