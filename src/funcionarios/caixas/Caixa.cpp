#include "Caixa.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

// construtor
Caixa::Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha, const double& total_vendido) :
Funcionario(nome, id, cpf, email, senha), total_vendido(total_vendido){}

// Registra uma venda no histórico
void Caixa::registrarVenda(const std::string& cliente, const std::map<std::string, std::pair<int, double>>& resumoCarrinho, 
double valorTotal, const std::string& dataHora) {
    vendas.push_back({cliente, resumoCarrinho, valorTotal, dataHora});
}

// getters
double Caixa::getTotalVendido() const{  return total_vendido; }

// setters
void Caixa::setTotalVendido(double total_vendido){  this->total_vendido = total_vendido; }

// método para gerar relatório de vendas do caixa
void Caixa::gerarRelatorio() const {
    // Remove espaços do nome para usar no nome do arquivo
    std::string nome_sem_espaco = getNome();
    nome_sem_espaco.erase(remove_if(nome_sem_espaco.begin(), nome_sem_espaco.end(), isspace), nome_sem_espaco.end());

    // Define o nome do arquivo
    std::string nomeArquivo = "data/relatorios_caixas/relatorio-" + nome_sem_espaco + ".txt";

    // Verifica se o arquivo já existe para evitar repetição de cabeçalho
    bool arquivoExiste = std::ifstream(nomeArquivo).good();

    // Abre o arquivo para escrita (append mode para manter dados existentes)
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para gerar relatorio.");
    }

    // Escreve o cabeçalho apenas se o arquivo for novo
    if (!arquivoExiste) {
        arquivo << "Relatorio de Vendas\n";
        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "Nome: " << getNome() << std::endl;
        arquivo << "CPF: " << getCpf() << std::endl;
        arquivo << "Cargo: " << getCargo() << std::endl;
        
    }

    // Adiciona os dados de vendas
    for (const auto& venda : vendas) {
        arquivo << "Cliente: " << venda.cliente << "\n";
        arquivo << "Data: " << venda.dataHora << "\n";
        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "Produtos vendidos:\n";
        arquivo << "Quantidade  Produto                            Preco Unitario  Preco Total\n";

        for (const auto& [produto, detalhes] : venda.resumoCarrinho) {
            int quantidade = detalhes.first;
            double precoUnitario = detalhes.second;
            double precoTotal = quantidade * precoUnitario;

            arquivo << std::setw(10) << std::left << quantidade << "  "
                    << std::setw(35) << std::left << produto
                    << std::setw(16) << std::fixed << std::setprecision(2) << precoUnitario
                    << std::setw(16) << precoTotal << std::endl;
        }

        arquivo << "--------------------------------------------------------------------------\n";
        arquivo << "Total da venda: R$" << std::fixed << std::setprecision(2) << venda.valorTotal << "\n";
        arquivo << "--------------------------------------------------------------------------\n\n";
    }

    

    // Fecha o arquivo
    arquivo.close();
}


// Método que retorna o cargo
std::string Caixa::getCargo() const{
    return "caixa";
}

