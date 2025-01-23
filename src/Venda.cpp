#include "Venda.h"
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
    // implementar lógica para gerar nota fiscal e salvar as vendas do funcionario
    
    // cria uma nota fiscal
    NotaFiscal nota(caixa, cliente, carrinho);

    // imprime um arquivo com a nota fiscal
    nota.gerarNotaFiscal();
}