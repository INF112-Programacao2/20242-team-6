#include "Venda.h"
#include "CaixaPCD.h"
#include "NotaFiscal.h"
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
    carrinho.realizarCompra();

    // atualiza o estoque no arquivo
    estoque.salvarEstoqueNoArquivo("data/estoque.txt");
    
    // finaliza venda e gera a nota fiscal pro cliente
    finalizarVenda(cliente, caixa, estoque, carrinho);
}

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

        // Imprime um arquivo com a nota fiscal
        nota.gerarNotaFiscal();

        // guarda o total vendido pelo caixa
        double total = carrinho.getValorTotal() + novo_caixa->getTotalVendido();
        novo_caixa->setTotalVendido(total);

        // Registra a venda no caixa
        novo_caixa->registrarVenda(cliente.getNome(), carrinho.getResumoCarrinho(), carrinho.getValorTotal(), dataHoraStr);

        // Exibe mensagem de finalização
        std::cout << "Venda finalizada com sucesso!\n";
        std::cout << "Cliente: " << cliente.getNome() << "\n";
        std::cout << "Total da compra: R$" << std::fixed << std::setprecision(2) << carrinho.getValorTotal() << "\n";
    } else {
        std::cerr << "Funcionario fornecido nao e um Caixa.\n";
    }
}