#include "Caixa.h"
#include <iostream>

// construtor
Caixa::Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
const std::string& email, const std::string& senha) : Funcionario(nome, id, cpf, email, senha) {}

// método para finalizar a compra do cliente
void Caixa::finalizarCompra(){
    // implementar ...
}

// método para gerar relatório de vendas do caixa
void Caixa::gerarRelatorio(){
    // implementar ...
}

// Método que retorna o cargo
std::string Caixa::getCargo() const{
    return "Caixa";
}

