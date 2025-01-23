#include "Caixa.h"
#include <iostream>
#include <iomanip>

// construtor
Caixa::Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha) : Funcionario(nome, id, cpf, email, senha) {}

// Registra uma venda no histórico
void Caixa::registrarVenda(const std::string& cliente, const std::map<std::string, std::pair<int, double>>& resumoCarrinho, 
double valorTotal, const std::string& dataHora) {
    vendas.push_back({cliente, resumoCarrinho, valorTotal, dataHora});
}

// método para gerar relatório de vendas do caixa
void Caixa::gerarRelatorio() const{
    std::cout << "Relatorio Mensal de Vendas\n";
    std::cout << "--------------------------------------------------------------------------\n";
    for (const auto& venda : vendas) {
        std::cout << "Cliente: " << venda.cliente << "\n";
        std::cout << "Data/Hora: " << venda.dataHora << "\n";
        std::cout << "--------------------------------------------------------------------------\n";
        std::cout << "Produtos vendidos:\n";
        std::cout << "Quantidade  Produto                            Preco Unitario  Preco Total\n";

        for (const auto& [produto, detalhes] : venda.resumoCarrinho) {
            int quantidade = detalhes.first;
            double precoUnitario = detalhes.second;
            double precoTotal = quantidade * precoUnitario;

            std::cout << std::setw(10) << std::left << quantidade << "  "
                      << std::setw(35) << std::left << produto
                      << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                      << std::setw(16) << precoTotal << std::endl;
        }

        std::cout << "--------------------------------------------------------------------------\n";
        std::cout << "Total da venda: " << std::fixed << std::setprecision(2) << venda.valorTotal << "\n";
        std::cout << "--------------------------------------------------------------------------\n";
    }
}

// Método que retorna o cargo
std::string Caixa::getCargo() const{
    return "Caixa";
}

