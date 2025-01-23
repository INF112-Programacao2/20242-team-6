#ifndef CAIXA_H
#define CAIXA_H

#include "Funcionario.h"
#include <map>

// Estrutura para guardar os detalhes de uma venda
struct DetalheVenda {
    std::string cliente;
    std::map<std::string, std::pair<int, double>> resumoCarrinho; // Produto -> {quantidade, preço unitário}
    double valorTotal;
    std::string dataHora; // Data e hora da venda
};

class Caixa : public Funcionario{
private:
    std::vector<DetalheVenda> vendas; // Histórico de vendas

public:
    // construtor
    Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha);

    // Registra uma venda no histórico
    void registrarVenda(const std::string& cliente, const std::map<std::string, std::pair<int, double>>& resumoCarrinho, 
    double valorTotal, const std::string& dataHora); 

    // método para gerar relatório de vendas do caixa
    virtual void gerarRelatorio() const override;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;
};

#endif