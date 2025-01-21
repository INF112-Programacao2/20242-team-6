#include "CaixaPCD.h"

// construtor
CaixaPcd::CaixaPcd(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha) : Caixa(nome, id, cpf, email, senha) {}

// método para finalizar a compra do cliente
void CaixaPcd::finalizarCompra(){
    // implementar...
}

// método para gerar relatório de vendas do caixa
void CaixaPcd::gerarRelatorio(){
    // implementar ...
}

// Método que retorna o cargo
std::string CaixaPcd::getCargo() const{
    return "CaixaPCD";
}
