#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>



class Produto{
private:
    std::string nome;
    int id;
    
    double preco;
    std::string descricao;
public:
    // Construtor
    Produto(const std::string& nome, const int& id, double preco, const std::string& descricao);

    // Métodos getters
    std::string getNome() const;
    int getId() const;
    
    double getPreco() const;
    std::string getDescricao() const;

    // Calcula o preço com desconto aplicado
    double getPrecoComDesconto() const;    
};

#endif