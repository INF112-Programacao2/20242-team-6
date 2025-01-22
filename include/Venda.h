#ifndef VENDA_H
#define VENDA_H

#include "Funcionario.h"
#include "Estoque.h"
#include "Cliente.h"
#include "Carrinho.h"

class Venda{
public:
    // método para iniciar a venda 
    void iniciarVenda(Funcionario* caixa, Estoque& estoque);

    // finaliza a venda e gera a nota fiscal para o cliente
    void finalizarVenda(Cliente& cliente, Funcionario* caixa, Estoque& estoque, Carrinho& carrinho);

};

#endif