#include "CaixaPCD.h"
#include "Menu.h"
#include "Venda.h"
#include <iostream>

void Menu::exibirMenuGerente(Funcionario* gerente, BancoFuncionario& banco, Estoque& estoque) {
    int escolha;

    do {
        std::cout << "\nMenu do Gerente:\n"
                  << "1. Administrar Funcionarios\n"
                  << "2. Gerenciar Estoque\n"
                  << "0. Sair\n"
                  << "Escolha uma opcao: ";
        std::cin >> escolha;

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
                std::cout << "Opcao invalida. Tente novamente.\n";
            }
    } while (escolha != 0);
}

void Menu::exibirMenuCaixa(Funcionario* caixa, Estoque& estoque) {
    int escolha;

    do {

        // abre o menu inicial para o caixa
        std::cout << "\nMenu do Caixa:\n"
                  << "1. Iniciar Venda\n"
                  << "2. Exibir Relatorio de Vendas\n"
                  << "0. Sair\n"
                  << "Escolha uma opcao: ";

        // abre as opções de acessibilidade para PCD
        if(caixa->getCargo() == "caixapcd"){
            CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

            std::string nomeArquivo = "features/accessibility/menu_pcd.txt";
            caixa_pcd->falarTexto(nomeArquivo); // chama TTS
        }

        std::cin >> escolha;

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
                std::cout << "Opcao invalida. Tente novamente.\n";
        }
    } while (escolha != 0);
}