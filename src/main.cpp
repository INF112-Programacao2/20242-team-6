#include "BancoFuncionario.h"
#include "Menu.h"
#include "Estoque.h"
#include <chrono>

using namespace std::chrono;

int main() {
    BancoFuncionario banco;
    Estoque estoque;

    try {
        std::string email, senha;

        // mostra a data no menu iniciar
        system_clock::time_point today = system_clock::now();
        time_t tt;
        tt = system_clock::to_time_t ( today );
        std::cout << "Data de hoje: " << ctime(&tt) << std::endl;

        std::cout << "Bem-vindo ao sistema de gerenciamento.\n";
        std::cout << "Digite seu email: ";
        std::cin >> email;
        std::cout << "Digite sua senha: ";
        std::cin >> senha;

        Funcionario* funcionario = banco.realizarLogin(email, senha);

        if (funcionario->getCargo() == "Gerente") {
            Menu::exibirMenuGerente(funcionario, banco, estoque);
        } else if (funcionario->getCargo() == "Caixa" || funcionario->getCargo() == "CaixaPCD") {
            Menu::exibirMenuCaixa(funcionario, estoque);
        } else {
            std::cout << "Tipo de funcionario nao reconhecido.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }

    return 0;
}
