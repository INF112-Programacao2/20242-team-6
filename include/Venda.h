#ifndef VENDA_H
#define VENDA_H

#include "Funcionario.h"
#include "Estoque.h"
#include "Cliente.h"

class Venda{
public:
    // método para iniciar a venda 
    void iniciarVenda(Funcionario* caixa, Estoque& estoque);

    // abre o carrinho de compras
    void abrirCarrinho(Cliente& cliente, Funcionario* caixa, Estoque& estoque);
};

#endif