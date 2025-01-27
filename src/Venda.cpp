#include "Venda.h"
#include "CaixaPCD.h"
#include "NotaFiscal.h"
#include "Tela.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <map>
#include <chrono>

using namespace std::chrono;

// método para iniciar o carrinho 
void Venda::iniciarVenda(Funcionario* caixa, Estoque& estoque){
    // armazena nome e cpf do cliente
    std::string nome_cliente;
    std::string cpf_cliente;

    Tela::limpar(); // limpa tela

    std::cout << "=============================================\n";
    std::cout << "                CADASTRO CLIENTE               \n";
    std::cout << "=============================================\n";

    // abre as opções de acessibilidade para PCD
    if(caixa->getCargo() == "caixapcd"){
        CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

        std::string nomeArquivo = "features/accessibility/cadastro_cliente.txt";
        caixa_pcd->falarTexto(nomeArquivo); // chama TTS
    }

    // pedir nome do cliente
    std::cout << "Nome do cliente: ";
    std::cin.ignore(); // Limpa buffer
    std::getline(std::cin, nome_cliente);

    // pedir cpf do cliente
    std::cout << "CPF do cliente: ";
    std::cin >> cpf_cliente;

    /*criar lógica aqui para validar cpf:
    1) pega só os 11 primeiros números (123.567.890-90) -> (12356789090)
    2) tranforma no formato cpf (XXX.XXX.XXX-XX)
    0bs: garantir que tenha exatos 11 numeros */
    
    std::cin.ignore(); // Limpa buffer após receber cpf

    // cria cliente
    Cliente cliente(nome_cliente, cpf_cliente);

    // cria um carrinho e inicia a venda
    Carrinho carrinho(cliente, caixa, estoque);
    carrinho.gerenciarCarrinho(estoque);

    // atualiza o estoque no arquivo
    estoque.salvarEstoqueNoArquivo("data/estoque.txt");
    
    // finaliza venda e gera a nota fiscal pro cliente
    finalizarVenda(cliente, caixa, estoque, carrinho);
}

// finaliza a venda, gera a nota fiscal, e o relatório do caixa
void Venda::finalizarVenda(Cliente& cliente, Funcionario* caixa, Estoque& estoque, Carrinho& carrinho){
    // Garante que o Funcionario é um Caixa
    Caixa* novo_caixa = dynamic_cast<Caixa*>(caixa);
    if (novo_caixa) {
        // Cria uma nota fiscal
        NotaFiscal nota(caixa, cliente, carrinho);

        // cria lógica para pegar hora atual e usar como string
        system_clock::time_point dataHora = system_clock::now();
        std::time_t tt = system_clock::to_time_t(dataHora);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&tt), "%H:%M %d/%m/%Y");
        std::string dataHoraStr = ss.str();

        if(!carrinho.getResumoCarrinho().empty()){         // verifica se a venda não foi cancelada para gerar relatório e nota fiscal
            // Imprime um arquivo com a nota fiscal
            nota.gerarNotaFiscal(dataHoraStr);

            // guarda o total vendido pelo caixa
            double total = carrinho.getValorTotal() + novo_caixa->getTotalVendido();
            novo_caixa->setTotalVendido(total);

            // Registra a venda pelo caixa
            novo_caixa->registrarVenda(cliente.getNome(), carrinho.getResumoCarrinho(), carrinho.getValorTotal(), dataHoraStr);
        }

        Tela::limpar(); // limpa tela

        // Exibe mensagem de finalização (adaptado para caixa_pcd)
        // Cria o buffer para armazenar a saída
        std::ostringstream buffer;

        // verifica se a venda foi feita ou cancelada
        if(!carrinho.getResumoCarrinho().empty()){ // verifica se a venda não foi cancelada

            // Escreve as mensagens no buffer
            buffer << "Venda finalizada com sucesso!\n"
                << "Cliente: " << cliente.getNome() << ".\n"
                << "Total da compra: " << std::fixed << std::setprecision(2) << carrinho.getValorTotal() << " reais.\n"
                << "Pressione Enter para continuar...\n";
        }else{

            // Escreve as mensagens no buffer
            buffer << "Venda cancelada!\n"
                << "Pressione Enter para continuar...\n";

        }

        // Chama a função para registrar a saída
        caixa->registrarSaida(buffer.str(), "features/accessibility/venda_finalizada.txt");
        
        // abre as opções de acessibilidade para PCD
        if(caixa->getCargo() == "caixapcd"){
            CaixaPcd* caixa_pcd = dynamic_cast<CaixaPcd*>(caixa);

            std::string nomeArquivo = "features/accessibility/venda_finalizada.txt";
            caixa_pcd->falarTexto(nomeArquivo); // chama TTS
        }
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
    } else {
        std::cerr << "Funcionário fornecido não é um Caixa.\n";
    }
}