#include "BancoFuncionario.h"
#include <fstream>
#include <sstream>
#include <iostream>

// construtor inicializa o banco de dados com alguns funcionários
BancoFuncionario::BancoFuncionario() {
    try {
        carregarFuncionariosDoArquivo("funcionarios.txt");
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar funcionários: " << e.what() << "\n";
        std::cerr << "Iniciando com banco de dados vazio.\n";
    }
}

// gerencia funcionarios
void BancoFuncionario::gerenciarFuncionarios(Funcionario* gerente, BancoFuncionario& banco) {
    int escolha;
    do {
        std::cout << "\n1. Adicionar Funcionario\n"
                  << "2. Remover Funcionario\n"
                  << "0. Sair\n"
                  << "Escolha uma opção: ";
        std::cin >> escolha;

        if (escolha == 1) {
            // Adicionar novo funcionário
            std::string nome, id, cpf, email, senha, cargo;
            std::cout << "Nome: ";
            std::cin.ignore(); // Limpa buffer
            std::getline(std::cin, nome);
            std::cout << "ID: ";
            std::cin >> id;
            std::cout << "CPF: ";
            std::cin >> cpf;
            std::cout << "Email: ";
            std::cin >> email;
            std::cout << "Senha: ";
            std::cin >> senha;
            std::cout << "Cargo (Gerente, Caixa, CaixaPCD): ";
            std::cin >> cargo;

            try {
                if (cargo == "Gerente") {
                    banco.adicionarFuncionario(gerente, std::make_unique<Gerente>(nome, id, cpf, email, senha));
                } else if (cargo == "Caixa") {
                    banco.adicionarFuncionario(gerente, std::make_unique<Caixa>(nome, id, cpf, email, senha));
                } else if (cargo == "CaixaPCD") {
                    banco.adicionarFuncionario(gerente, std::make_unique<CaixaPcd>(nome, id, cpf, email, senha));
                } else {
                    std::cerr << "Cargo inválido.\n";
                }
                salvarFuncionariosNoArquivo("funcionarios.txt");    // salva o lote no arquivo texto  

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }

        } else if (escolha == 2) {
            // Remover funcionário
            std::string id;
            std::cout << "ID do funcionario a ser removido: ";
            std::cin >> id;

            try {
                banco.removerFuncionario(gerente, id);
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }
        }
    } while (escolha != 0);
}

// Adiciona um funcionário ao banco de dados
void BancoFuncionario::adicionarFuncionario(Funcionario* gerente, std::unique_ptr<Funcionario> novoFuncionario) {
    if (gerente->getCargo() != "Gerente") {
        throw std::runtime_error("Apenas gerentes podem adicionar novos funcionarios.");
    }

    const std::string& id = novoFuncionario->getId();
    if (funcionarios.find(id) != funcionarios.end()) {
        throw std::runtime_error("Ja existe um funcionario com este ID.");
    }

    funcionarios[id] = std::move(novoFuncionario);
    std::cout << "Funcionario adicionado com sucesso.\n";
}

// Remove um funcionário do banco de dados
void BancoFuncionario::removerFuncionario(Funcionario* gerente, const std::string& id) {
    if (gerente->getCargo() != "Gerente") {
        throw std::runtime_error("Apenas gerentes podem remover funcionários.");
    }

    auto it = funcionarios.find(id);
    if (it == funcionarios.end()) {
        throw std::runtime_error("Funcionario nao encontrado.");
    }

    funcionarios.erase(it);
    std::cout << "Funcionario removido com sucesso.\n";
}

// Retorna um ponteiro para o funcionário, se encontrado
Funcionario* BancoFuncionario::buscarFuncionarioPorEmail(const std::string& email) const {
    for (const auto& [id, funcionario] : funcionarios) {
        if (funcionario->getEmail() == email) {
            return funcionario.get();
        }
    }
    return nullptr; // Não encontrado
}

// realiza o login do funcionario
Funcionario* BancoFuncionario::realizarLogin(const std::string& email, const std::string& senha) const {
    Funcionario* funcionario = buscarFuncionarioPorEmail(email);
    if (funcionario && funcionario->validarSenha(senha)) {
        return funcionario;
    }
    throw std::runtime_error("Email ou senha invalidos.");
}

// salva os funcionarios em um arquivo txt (simula o banco de dados)
void BancoFuncionario::salvarFuncionariosNoArquivo(const std::string& nomeArquivo) const {
    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre somente para escrita
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar funcionarios.");
    }

    for (const auto& [id, funcionario] : funcionarios) { // percorre o banco de funcionarios
        arquivo << funcionario->getCargo() << ","
                << funcionario->getNome() << ","
                << funcionario->getId() << ","
                << funcionario->getCpf() << ","
                << funcionario->getEmail() << ","
                << funcionario->getSenha() << "\n";
    }

    arquivo.close();
}

// carrega os funcionarios a partir de um arquivo txt (simula o banco de dados)
void BancoFuncionario::carregarFuncionariosDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo, std::ios::in); // abre somente para leitura
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar funcionarios.");
    }

    funcionarios.clear(); // limpa o banco de funcionarios

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string cargo, nome, id, cpf, email, senha;
        
        std::getline(stream, cargo, ',');
        std::getline(stream, nome, ',');
        std::getline(stream, id, ',');
        std::getline(stream, cpf, ',');
        std::getline(stream, email, ',');
        std::getline(stream, senha, ',');

        if (cargo == "Gerente") {
            funcionarios[id] = std::make_unique<Gerente>(nome, id, cpf, email, senha);
        } else if (cargo == "Caixa") {
            funcionarios[id] = std::make_unique<Caixa>(nome, id, cpf, email, senha);
        } else if (cargo == "CaixaPCD") {
            funcionarios[id] = std::make_unique<CaixaPcd>(nome, id, cpf, email, senha);
        } else {
            throw std::runtime_error("Cargo desconhecido no arquivo.");
        }
    }

    arquivo.close();
}
