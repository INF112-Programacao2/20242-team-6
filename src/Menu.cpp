#include "CaixaPCD.h"
#include "Menu.h"
#include "Venda.h"
#include "Tela.h"
#include <iostream>
#include <thread>

void Menu::exibirMenuGerente(Funcionario* gerente, BancoFuncionario& banco, Estoque& estoque) {
    int escolha;

    do {

        Tela::limpar(); // limpa tela
        
        std::cout << "\n==================================================\n";
        std::cout << "                   MENU DO GERENTE              \n";
        std::cout << "==================================================\n";
        std::cout << "   1. Administrar Funcionários                  \n";
        std::cout << "   2. Gerenciar Estoque                         \n";
        std::cout << "   0. Sair                                      \n";
        std::cout << "==================================================\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> escolha;

        try{

            if(std::cin.fail()){
                throw std::invalid_argument("Opção inválida, tente novamente!");
            }

            switch (escolha) {
                case 1:
                    // Entra na opção de gerenciar funcionarios
                    banco.gerenciarFuncionarios(gerente, banco);
                    break;
                case 2:
                    std::cout << "Entrando no modulo de estoque...\n";
                    // Entra na opção de gerenciar estoque
                    estoque.gerenciarEstoque(gerente, estoque);
                    break;
                case 0:
                    std::cout << "Saindo...\n";
                    break;
                default:
                    std::cout << "Opção inválida, tente novamente.\n";

                    // Espera por 1 segundo
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            // Espera por 1 segundo
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } while (escolha != 0);

    return; // faz logout
}

void Menu::exibirMenuCaixa(Funcionario* caixa, Estoque& estoque) {
    int escolha;

    do {
        
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

        // abre as opções de acessibilidade para PCD
        if(caixa->getCargo() == "caixapcd"){
            CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

            std::string nomeArquivo = "features/accessibility/menu_pcd.txt";
            caixa_pcd->falarTexto(nomeArquivo); // chama TTS
        }

        // recebe a escolha do usuário
        std::cin >> escolha;

        try{

            if(std::cin.fail()){
                throw std::invalid_argument("Opção inválida, tente novamente!");
            }

            switch (escolha) {
                case 1:
                    std::cout << "Iniciando venda...\n";
                    Venda novaVenda;
                    novaVenda.iniciarVenda(caixa, estoque);
                    break;
                case 2:
                    std::cout << "Exibindo relatorio de venda...\n";
                    caixa->exibirRelatorio();
                    break;
                case 0:
                    std::cout << "Saindo...\n";
                    break;
                default:
                    // abre as opções de acessibilidade para PCD
                    if(caixa->getCargo() == "caixapcd"){
                        CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

                        std::string nomeArquivo = "features/accessibility/msg_erro01.txt";
                        caixa_pcd->falarTexto(nomeArquivo); // chama TTS
                    }
                    std::cout << "Opção inválida, tente novamente.\n";

                    // Espera por 1 segundo
                    std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    } while (escolha != 0);

    return; // faz logout
}