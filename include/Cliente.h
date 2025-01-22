#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

class Cliente{
private:
    const std::string nome;
    const std::string cpf;
public:
    Cliente(const std::string nome, const std::string cpf);
};
#endif