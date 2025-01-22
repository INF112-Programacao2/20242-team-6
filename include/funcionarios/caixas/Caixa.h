#ifndef CAIXA_H
#define CAIXA_H

#include "Funcionario.h"

class Caixa : public Funcionario{
public:
    // construtor
    Caixa(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha);

    // método para finalizar a compra do cliente
    virtual void finalizarCompra();

    // método para gerar relatório de vendas do caixa
    virtual void gerarRelatorio() override;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;
};

#endif