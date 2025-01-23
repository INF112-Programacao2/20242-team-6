#include "Venda.h"
#include "Caixa.h"
#include "NotaFiscal.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <map>

// método para iniciar o carrinho 
void Venda::iniciarVenda(Funcionario* caixa, Estoque& estoque){
    // armazena nome e cpf do cliente
    std::string nome_cliente;
    std::string cpf_cliente;

    // pedir nome do cliente
    std::cout << "Nome do cliente: ";
    std::cin.ignore(); // Limpa buffer
    std::getline(std::cin, nome_cliente);

    // pedir cpf do cliente
    std::cout << "CPF do cliente: ";
    std::cin >> cpf_cliente;
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

        // Imprime um arquivo com a nota fiscal
        nota.gerarNotaFiscal();

        // criar lógica para pegar hora atual (teste)
        std::string dataHora = "2025-01-22 14:35";

        // Registra a venda no caixa
        novo_caixa->registrarVenda(cliente.getNome(), carrinho.getResumoCarrinho(), carrinho.getValorTotal(), dataHora);

        // Exibe mensagem de finalização
        std::cout << "Venda finalizada com sucesso!\n";
        std::cout << "Cliente: " << cliente.getNome() << "\n";
        std::cout << "Total da compra: " << std::fixed << std::setprecision(2) << carrinho.getValorTotal() << "\n";
    } else {
        std::cerr << "Funcionário fornecido nao e um Caixa.\n";
    }
}