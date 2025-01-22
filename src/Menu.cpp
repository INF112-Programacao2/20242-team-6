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
        std::cout << "\nMenu do Caixa:\n"
                  << "1. Iniciar Venda\n"
                  << "2. Gerar Relatorio de Vendas Mensal\n"
                  << "0. Sair\n"
                  << "Escolha uma opcao: ";
        std::cin >> escolha;

        switch (escolha) {
            case 1:
                std::cout << "Iniciando venda...\n";
                Venda novaVenda;
                novaVenda.iniciarVenda(caixa, estoque);
                break;
            case 2:
                std::cout << "Gerando relatorio de vendas mensal...\n";
                // Implemente a geração de relatórios
                break;
            case 0:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
        }
    } while (escolha != 0);
}