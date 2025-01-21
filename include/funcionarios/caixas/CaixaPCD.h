#ifndef CAIXA_PCD_H
#define CAIXA_PCD_H

#include "Caixa.h"

class CaixaPcd : public Caixa{
public:
    // construtor
    CaixaPcd(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha);

    // método para finalizar a compra do cliente
    virtual void finalizarCompra() override;

    // método para gerar relatório de vendas do caixa
    virtual void gerarRelatorio() override;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;
};

#endif