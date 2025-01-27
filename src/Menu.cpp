#include "CaixaPCD.h"
#include "Menu.h"
#include "Venda.h"
#include "Tela.h"
#include <iostream>
#include <thread>

void Menu::exibirMenuGerente(Funcionario* gerente, BancoFuncionario& banco, Estoque& estoque) {
    int escolha;

    do {
        // Início do tratamento para garantir entrada válida
        while (true) {
            try {
                Tela::limpar(); // limpa tela

                std::cout << "\n==================================================\n";
                std::cout << "                   MENU DO GERENTE              \n";
                std::cout << "==================================================\n";
                std::cout << "1. Administrar Funcionários                  \n";
                std::cout << "2. Gerenciar Estoque                         \n";
                std::cout << "0. Sair                                      \n";
                std::cout << "==================================================\n";
                std::cout << "Escolha uma opção: ";
                std::cin >> escolha;

                // Verifica se a entrada foi inválida
                if (std::cin.fail()) {
                    std::cin.clear(); // limpa a flag de erro
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta a entrada inválida
                    throw std::invalid_argument("Opção inválida, tente novamente!");
                }

                // Saindo do laço de validação se a entrada for válida
                break;

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Espera antes de repetir
            }
        }

        // Processa a escolha após validação
        switch (escolha) {
            case 1:
                // Entra na opção de gerenciar funcionários
                banco.gerenciarFuncionarios(gerente, banco);
                break;
            case 2:
                std::cout << "Entrando no módulo de estoque...\n";
                // Entra na opção de gerenciar estoque
                estoque.gerenciarEstoque(gerente, estoque);
                break;
            case 0:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opção inválida, tente novamente.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
        }

    } while (escolha != 0);

    return; // faz logout
}

void Menu::exibirMenuCaixa(Funcionario* caixa, Estoque& estoque) {
    int escolha;

    do {
        // Bloco de validação de entrada para evitar falhas no std::cin
        while (true) {
            try {
                Tela::limpar(); // limpa tela

                // Menu do Caixa
                std::cout << "==================================================\n";
                std::cout << "                    MENU DO CAIXA                \n";
                std::cout << "==================================================\n";
                std::cout << "1. Iniciar Venda                              \n";
                std::cout << "2. Exibir Relatório de Vendas                 \n";
                std::cout << "0. Sair                                       \n";
                std::cout << "==================================================\n";
                std::cout << "Escolha uma opção: ";

                // Ativa acessibilidade para PCD
                if (caixa->getCargo() == "caixapcd") {
                    CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);
                    std::string nomeArquivo = "features/accessibility/menu_pcd.txt";
                    caixa_pcd->falarTexto(nomeArquivo); // chama TTS
                }

                // Recebe a escolha do usuário
                std::cin >> escolha;

                // Verifica se a entrada foi inválida
                if (std::cin.fail()) {
                    std::cin.clear(); // limpa a flag de erro
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta a entrada inválida
                    throw std::invalid_argument("Opção inválida, tente novamente!");
                }

                // Saindo do laço de validação caso a entrada seja válida
                break;

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1)); // Espera antes de mostrar o menu novamente
            }
        }

        // Processa a escolha após validação
        try {
            switch (escolha) {
                case 1:
                    std::cout << "Iniciando venda...\n";
                    Venda novaVenda;
                    novaVenda.iniciarVenda(caixa, estoque);
                    break;
                case 2:
                    std::cout << "Exibindo relatório de vendas...\n";
                    caixa->exibirRelatorio();
                    break;
                case 0:
                    std::cout << "Saindo...\n";
                    break;
                default:
                    // Ativa acessibilidade para PCD em caso de erro
                    if (caixa->getCargo() == "caixapcd") {
                        CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);
                        std::string nomeArquivo = "features/accessibility/msg_erro01.txt";
                        caixa_pcd->falarTexto(nomeArquivo); // chama TTS
                    }
                    std::cout << "Opção inválida, tente novamente.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Pausa para exibir a mensagem de erro
        }

    } while (escolha != 0);

    return; // faz logout
}