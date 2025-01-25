#include "BancoFuncionario.h"
#include "Tela.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// construtor inicializa o banco de dados com alguns funcionários
BancoFuncionario::BancoFuncionario() {
    try {
        carregarFuncionariosDoArquivo("data/funcionarios.txt");
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar funcionarios: " << e.what() << "\n";
        std::cerr << "Iniciando com banco de dados vazio.\n";
    }
}

// gerencia funcionarios
void BancoFuncionario::gerenciarFuncionarios(Funcionario* gerente, BancoFuncionario& banco) {
    int escolha;
    do {

        Tela::limpar(); // limpa tela

        std::cout << "=============================================\n";
        std::cout << "          GERENCIAMENTO DE FUNCIONARIOS      \n";
        std::cout << "=============================================\n";
        std::cout << "  1. Adicionar Funcionário                  \n";
        std::cout << "  2. Remover Funcionário                    \n";
        std::cout << "  3. Exibir Informação de um Funcionário    \n";
        std::cout << "  4. Top Caixas com Mais Vendas             \n";
        std::cout << "  0. Sair                                   \n";
        std::cout << "=============================================\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> escolha;

        if (escolha == 1) {
            // Adicionar novo funcionário
            std::string nome, id, cpf, email, senha, cargo;

            Tela::limpar(); // limpa tela

            // subtítulo
            std::cout << "ADICIONAR FUNCIONÁRIO\n";

            std::cout << "Cargo (Gerente, Caixa, CaixaPCD): ";
            std::cin >> cargo;

            /*validar cargo:
            1) transforma a entrada em letras minuscalas e
            2) testa se é ou gerente ou caixa ou caixapcd */

            std::cout << "Nome: ";
            std::cin.ignore(); // Limpa buffer
            std::getline(std::cin, nome);

            /*limite o nome para no maximo 50 caracteres*/

            std::cout << "ID: ";
            std::cin >> id;

            /*criar lógica aqui para validar id:
            Inicia com uma letra maiscula que indica o cargo:
            G - gerente
            C - caixa
            P - caixa pcd
            validar se o cargo esta correto
            e segue com - e um numero
            exemplo: G-01
            obs: a função adicionarFuncionario() já valida se o id for repetido
            */

            std::cout << "CPF: ";
            std::cin >> cpf;

            /*criar lógica aqui para validar cpf:
            1) pega só os 11 primeiros números (123.567.890-90) -> (12356789090)
            2) tranforma no formato cpf (XXX.XXX.XXX-XX)
            0bs: garantir que tenha exatos 11 numeros */

            std::cout << "Email: ";
            std::cin >> email;

            /*criar lógica aqui para validar email:
            deve ter o formato XXX@YYY.ZZZ */

            std::cout << "Senha: ";
            std::cin >> senha;

            

            try {
                if (cargo == "gerente") {
                    banco.adicionarFuncionario(gerente, std::make_unique<Gerente>(nome, id, cpf, email, senha));
                } else if (cargo == "caixa") {
                    banco.adicionarFuncionario(gerente, std::make_unique<Caixa>(nome, id, cpf, email, senha, 0.0));
                } else if (cargo == "caixapcd") {
                    banco.adicionarFuncionario(gerente, std::make_unique<CaixaPcd>(nome, id, cpf, email, senha, 0.0));
                } else {
                    std::cerr << "Cargo inválido.\n";
                }
                salvarFuncionariosNoArquivo("data/funcionarios.txt");    // salva o lote no arquivo texto  

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }

        } else if (escolha == 2) {
            // Remover funcionário

            Tela::limpar(); // limpa tela

            // subtítulo
            std::cout << "REMOVER FUNCIONÁRIO\n";

            std::string id;
            std::cout << "ID do funcionário a ser removido: ";
            std::cin >> id;

            try {
                banco.removerFuncionario(gerente, id);
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }
        } else if(escolha == 3){
            // mostrar detalhes de um funcionario
            // pedir nome e buscar no banco de dados
            // implementar uma função membro buscarFuncionarioPorNome() (parecido com buscarFuncionarioPorEmail())
            // e exibir os detalhes dos funcionarios
        }  else if(escolha == 4){
            // implementar um ranking de caixas com mais vendas em dinheiro.
        }


    } while (escolha != 0);
}

// Adiciona um funcionário ao banco de dados
void BancoFuncionario::adicionarFuncionario(Funcionario* gerente, std::unique_ptr<Funcionario> novoFuncionario) {
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem adicionar novos funcionários.");
    }

    const std::string& id = novoFuncionario->getId();
    if (funcionarios.find(id) != funcionarios.end()) {
        throw std::runtime_error("Já existe um funcionário com este ID.");
    }

    funcionarios[id] = std::move(novoFuncionario);
    std::cout << "Funcionário adicionado com sucesso.\n";
}

// Remove um funcionário do banco de dados
void BancoFuncionario::removerFuncionario(Funcionario* gerente, const std::string& id) {
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem remover funcionários.");
    }

    auto it = funcionarios.find(id);
    if (it == funcionarios.end()) {
        throw std::runtime_error("Funcionário nao encontrado.");
    }

    funcionarios.erase(it);
    std::cout << "Funcionário removido com sucesso.\n";
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
    throw std::runtime_error("Email ou senha inválidos.");
}

// salva os funcionarios em um arquivo txt (simula o banco de dados)
void BancoFuncionario::salvarFuncionariosNoArquivo(const std::string& nomeArquivo) const {
    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre somente para escrita
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar funcionários.");
    }

    for (const auto& [id, funcionario] : funcionarios) { // percorre o banco de funcionarios
        arquivo << funcionario->getCargo() << ","
                << funcionario->getNome() << ","
                << funcionario->getId() << ","
                << funcionario->getCpf() << ","
                << funcionario->getEmail() << ","
                << funcionario->getSenha();

                // Verifica se é do tipo Caixa ou CaixaPCD
                if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixapcd") {
                    const Caixa* caixa = dynamic_cast<const Caixa*>(funcionario.get());
                    if (caixa) {
                        arquivo << "," << std::fixed << std::setprecision(2) << caixa->getTotalVendido();
                    }
                }

        arquivo << "\n"; // Nova linha para o próximo funcionário                
    }

    arquivo.close();
}

// carrega os funcionarios a partir de um arquivo txt (simula o banco de dados)
void BancoFuncionario::carregarFuncionariosDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo, std::ios::in); // abre somente para leitura
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar funcionários.");
    }

    funcionarios.clear(); // limpa o banco de funcionarios

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string cargo, nome, id, cpf, email, senha, valor_total_str;
                double valor_total = 0.0; // inicializa o valor total

        // Lê os campos obrigatórios
        std::getline(stream, cargo, ',');
        std::getline(stream, nome, ',');
        std::getline(stream, id, ',');
        std::getline(stream, cpf, ',');
        std::getline(stream, email, ',');
        std::getline(stream, senha, ',');

        // Se for "Caixa" ou "CaixaPCD", tenta ler o valor total vendido
        if (cargo == "caixa" || cargo == "caixapcd") {
            if (!std::getline(stream, valor_total_str, ',')) {
                throw std::runtime_error("Campo valor_total ausente para um cargo de Caixa.");
            }
            try {
                valor_total = std::stod(valor_total_str);
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Valor total vendido inválido no arquivo.");
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Valor total vendido fora do intervalo válido.");
            }
        }

        if (cargo == "gerente") {
            funcionarios[id] = std::make_unique<Gerente>(nome, id, cpf, email, senha);
        } else if (cargo == "caixa") {
            funcionarios[id] = std::make_unique<Caixa>(nome, id, cpf, email, senha, valor_total);
        } else if (cargo == "caixapcd") {
            funcionarios[id] = std::make_unique<CaixaPcd>(nome, id, cpf, email, senha, valor_total);
        } else {
            throw std::runtime_error("Cargo desconhecido no arquivo.");
        }
    }

    // fecha arquivo
    arquivo.close();
}
