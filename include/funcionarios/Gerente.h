#ifndef GERENTE_H
#define GERENTE_H

#include "Funcionario.h"

class Gerente : public Funcionario {
public:
    // Construtor
    Gerente(const std::string& nome, const std::string& id, const std::string& cpf,
    const std::string& email, const std::string& senha);

    // gerar relatorio dos funcionarios
    virtual void gerarRelatorio() const override;

    // Método que retorna o cargo
    virtual std::string getCargo() const override;

    // método para exibir o relatoório na tela
    virtual void exibirRelatorio() const override;

};

#endif