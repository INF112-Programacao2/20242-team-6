#include "CaixaPCD.h"

// construtor
CaixaPcd::CaixaPcd(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha, const double& total_vendido) : Caixa(nome, id, cpf, email, senha, total_vendido) {}

// método para gerar relatório de vendas do caixa
void CaixaPcd::gerarRelatorio() const{
    // implementar ...
}

// Método que retorna o cargo
std::string CaixaPcd::getCargo() const{
    return "caixapcd";
}
