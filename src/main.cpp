#include "BancoFuncionario.h"
#include "Tela.h"
#include "Menu.h"
#include "Estoque.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

// exibe menu inicial
void exibirMenuInicial();

int main() {
    BancoFuncionario banco;
    Estoque estoque;

    try {
        std::string email, senha;

        int escolha = -1; // inicia o numero sentinela 

        do{
            // Exibindo a tela inicial
            exibirMenuInicial();

            std::cin >> escolha; // loga usuario ou fecha programa

            if(std::cin.fail() || (escolha != 1 && escolha != 0)){
                throw std::invalid_argument("Opção inválida!");
            }

            switch (escolha)
            {
            case 1:
                Tela::limpar(); // limpa tela

                std::cout << "==================================================\n";
                std::cout << "                      LOGIN              \n";
                std::cout << "==================================================\n";

                std::cout << "Digite seu email: ";
                std::cin >> email;
                std::cout << "Digite sua senha: ";
                std::cin >> senha;

                try{
                    Funcionario* funcionario = banco.realizarLogin(email, senha);
                 

                    if (funcionario->getCargo() == "gerente") {
                        Menu::exibirMenuGerente(funcionario, banco, estoque);
                    } else if (funcionario->getCargo() == "caixa" || funcionario->getCargo() == "caixapcd") {
                        Menu::exibirMenuCaixa(funcionario, estoque);
                    } else {
                        std::cout << "Tipo de funcionário não reconhecido.\n";

                        // Espera por 1 segundo
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }catch(std::exception& e){
                    std::cout << "Erro: " << e.what() << "\n";
                    // Espera por 1 segundo
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                break;
            
            case 0:
                return 0; // fecha programa
            }
        }while(true);
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }

    return 0; // fecha programa
}

void exibirMenuInicial(){
    // mostra a data no menu iniciar
    system_clock::time_point today = system_clock::now();
    time_t tt;
    tt = system_clock::to_time_t ( today );

    std::string dataAtual = ctime(&tt);
    dataAtual.pop_back(); // Remove o caractere de nova linha gerado por ctime

    Tela::limpar(); // limpa tela

    // Exibindo a tela inicial
    std::cout << "**************************************************\n";
    std::cout << "                  SUPERMERCADO DA CCP             \n\n";
    std::cout << "**************************************************\n";
    std::cout << "                " << dataAtual << "              \n";
    std::cout << "**************************************************\n";
    std::cout << "1. LOGIN\n";
    std::cout << "0. SAIR\n";
    std::cout << "**************************************************\n";
    std::cout << "Escolha uma opção: ";
}
