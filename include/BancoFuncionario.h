#ifndef BANCO_FUNCIONARIO_H
#define BANCO_FUNCIONARIO_H

#include "Gerente.h"
#include "CaixaPCD.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <iostream>

class BancoFuncionario {
private:

    std::unordered_map<std::string, std::unique_ptr<Funcionario>> funcionarios;

public:
    // construtor
    BancoFuncionario();

    // Método para gerenciar os funcionarios(gerente tem permissão)
    void gerenciarFuncionarios(Funcionario* gerente, BancoFuncionario& banco);

    // Adiciona um funcionário ao banco de dados
    void adicionarFuncionario(Funcionario* gerente, std::unique_ptr<Funcionario> novoFuncionario);

    // Remove um funcionário do banco de dados
    void removerFuncionario(Funcionario* gerente, const std::string& id);

    // Retorna um ponteiro para o funcionário, se encontrado
    Funcionario* buscarFuncionarioPorEmail(const std::string& email) const;

    // realiza o login do funcionario
    Funcionario* realizarLogin(const std::string& email, const std::string& senha) const;

    // salva os funcionarios em um arquivo txt (simula o banco de dados)
    void salvarFuncionariosNoArquivo(const std::string& nomeArquivo) const;

    // carrega os funcionarios a partir de um arquivo txt (simula o banco de dados)
    void carregarFuncionariosDoArquivo(const std::string& nomeArquivo);
};


#endif