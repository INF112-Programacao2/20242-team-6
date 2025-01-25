#include "Estoque.h"
#include "Tela.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <thread>

// construtor inicializa o banco de dados com alguns funcionários
Estoque::Estoque() {
    try {
        carregarEstoqueDoArquivo("data/estoque.txt");
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar estoque: " << e.what() << "\n";
        std::cerr << "Iniciando com banco de dados vazio.\n";
    }
}

// gerencia o estoque (gerente tem permissão)
void Estoque::gerenciarEstoque(Funcionario* gerente, Estoque& estoque) {
    int escolha;
    do {
        Tela::limpar(); // limpa tela

        std::cout << "=============================================\n";
        std::cout << "          GERENCIAMENTO DE ESTOQUE           \n";
        std::cout << "=============================================\n";
        std::cout << " 1. Adicionar novo lote de um produto        \n";
        std::cout << " 2. Remover lote                             \n";
        std::cout << " 0. Sair                                     \n";
        std::cout << "=============================================\n";
        std::cout << "Escolha uma opção: ";

        std::cin >> escolha;

        // Verifica falha na entrada
        if (std::cin.fail()) {
            std::cin.clear(); // Limpa flag de erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora entrada inválida
            std::cout << "Entrada inválida. Por favor, digite um número.\n"; // precisa do buffer
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Espera por 2 segundos
            continue; // Volta ao início do loop
        }

        if (escolha == 1) {
            // Adicionar novo lote
            std::string nome, codigo, data_validade, descricao;
            int tamanho;
            double preco;
            size_t limite_nome = 70;    // guarda tamanho máximo do nome do produto
            size_t limite_descricao = 100; // guarda tamanho máximo da descrição

            Tela::limpar(); // limpa tela

            try {
                std::cout << "Nome: ";
                std::cin.ignore(); // Limpa buffer
                std::getline(std::cin, nome);

                if (nome.size() > limite_nome) { // garante que o tamanho do nome seja controlado
                    throw std::out_of_range("Nome do produto deve ter no máximo " + std::to_string(limite_nome) + " caracteres\n");
                    std::this_thread::sleep_for(std::chrono::seconds(2)); // Espera por 2 segundos
                }

                std::cout << "Codigo do lote: ";
                std::cin >> codigo;

                std::cout << "Data de validade (dd/mm/aaaa): ";
                std::cin >> data_validade;

                std::cout << "Quantidade: ";
                std::cin >> tamanho;

                if (std::cin.fail()) {
                    throw std::invalid_argument("Entrada inválida para quantidade.");
                }

                if (tamanho <= 0) { // garante tamanho válido
                    throw std::invalid_argument("Quantidade de produtos deve ser maior que zero.");
                }

                std::cout << "Preço da unidade: ";
                std::cin >> preco;

                if (std::cin.fail()) {
                    throw std::invalid_argument("Entrada inválida para preço.");
                }

                if (preco <= 0.0) { // garante preço válido
                    throw std::invalid_argument("Preço dos produtos deve ser maior que zero.");
                }

                std::cout << "Descrição: ";
                std::cin.ignore(); // Limpa buffer
                std::getline(std::cin, descricao);

                if (descricao.size() > limite_descricao) { // garante que o tamanho da descrição seja controlado
                    throw std::out_of_range("Descrição do produto deve ter no máximo " + std::to_string(limite_descricao) + " caracteres\n");
                }

                // cria um lote temporário no heap para armazenar no estoque.
                std::unique_ptr<Lote> lote = std::make_unique<Lote>(nome, codigo, data_validade, tamanho);

                for (int i = 0; i < tamanho; i++) {
                    lote->preencherLote(nome, i + 1, preco, descricao); // preenche o lote com os produtos
                }

                if (lote->verificarValidade()) { // garante produto dentro da validade
                    throw std::runtime_error("Lote com produtos fora da validade.");
                }

                estoque.adicionarLote(gerente, std::move(lote));    // adiciona lote no estoque
                salvarEstoqueNoArquivo("data/estoque.txt"); // salva o lote no arquivo texto

            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }

        } else if (escolha == 2) {
            // Remover lote
            std::string codigo;
            std::cout << "Codigo do lote a ser removido: ";
            std::cin >> codigo;

            // tratar excessão

            try {
                estoque.removerLote(gerente, codigo);
                salvarEstoqueNoArquivo("data/estoque.txt"); // salva o lote no arquivo texto
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << "\n";
            }
        }
    } while (escolha != 0);
}


// busca o produto(lote) por nome
Lote* Estoque::buscarProdutoPorNome(const std::string& nome){
    for(const auto&[codigo, lote] : estoque){
        if(lote->getNome() == nome){
            return lote.get();
        }
    }
    return nullptr; // Não encontrado
}

// adiciona um lote ao estoque (gerente tem permissão)
void Estoque::adicionarLote(Funcionario* gerente, std::unique_ptr<Lote> lote){
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem adicionar novos funcionarios.");
    }

    const std::string& codigo = lote->getCodigo();
    if (estoque.find(codigo) != estoque.end()) {
        throw std::runtime_error("Ja existe um lote com este codigo.");
    }

    estoque[codigo] = std::move(lote);
    std::cout << "Lote adicionado com sucesso!\n";
}

// remove um lote do estoque
void Estoque::removerLote(Funcionario* gerente, const std::string& codigo){
    if (gerente->getCargo() != "gerente") {
        throw std::runtime_error("Apenas gerentes podem remover funcionários.");
    }

    auto it = estoque.find(codigo);
    if(it == estoque.end()){
        throw std::runtime_error("Lote não existe");
    }

    estoque.erase(it);
    std::cout << "Lote removido.\n";
}

// salva o estoque em um arquivo txt (simula um banco de dados)
void Estoque::salvarEstoqueNoArquivo(const std::string& nomeArquivo) const{
    std::ofstream arquivo(nomeArquivo, std::ios::out); // abre para escrita e concatena o final
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para salvar Estoque.");
    }

    for (const auto& [codigo, lote] : estoque) { // percorre o Estoque e salva lote e produtos
        
        if(lote->getTamanho() > 0){
            arquivo << lote->getNome() << ","
                    << lote-> getCodigo()<< ","
                    << lote->getValidade() << ","
                    << lote->getTamanho() << "\n";
            for(size_t i=0; i<lote->getTamanho(); i++){
                Produto produto = lote->pesquisarProduto(static_cast<int>(i));
                arquivo << produto.getNome() << ","
                    << produto.getId() << ","
                    << produto.getPreco() << ","
                    << produto.getDescricao() << "\n";
            }        
        }
    }
    arquivo.close();
}

// carrega o estoque a partir de um arquivo txt (simula um banco de dados)
void Estoque::carregarEstoqueDoArquivo(const std::string& nomeArquivo){
    
    std::ifstream arquivo(nomeArquivo, std::ios::in); // abre somente para leitura
    if (!arquivo) {
        throw std::runtime_error("Erro ao abrir o arquivo para carregar estoque.");
    }

    estoque.clear(); // limpa o estoque

    std::string linha;
    while (std::getline(arquivo, linha)) {
        std::istringstream stream(linha);
        std::string nome, codigo, validade, descricao, tamanhoStr, idStr, precoStr;
        int id, tamanho;
        double preco;

        try {
            std::getline(stream, nome, ',');
            std::getline(stream, codigo, ',');
            std::getline(stream, validade, ',');
            std::getline(stream, tamanhoStr, ',');

            if (nome.empty() || codigo.empty() || validade.empty() || tamanhoStr.empty()) {
                throw std::runtime_error("Formato inválido no arquivo para o lote.");
            }

            // Converter tamanho
            tamanho = std::stoi(tamanhoStr);

            // Adiciona o lote ao estoque
            std::unique_ptr<Lote> lote = std::make_unique<Lote>(nome, codigo, validade, tamanho);
            estoque[codigo] = std::move(lote);

            for (int i = 0; i < tamanho; i++) {
                if (!std::getline(arquivo, linha)) {
                    throw std::runtime_error("Numero de produtos menor que o tamanho especificado para o lote.");
                }

                std::istringstream stream(linha);
                std::getline(stream, nome, ',');
                std::getline(stream, idStr, ',');
                std::getline(stream, precoStr, ',');
                std::getline(stream, descricao, ',');

                if (nome.empty() || idStr.empty() || precoStr.empty() || descricao.empty()) {
                    throw std::runtime_error("Formato inválido no arquivo para o produto.");
                }

                // Conversões
                id = std::stoi(idStr);
                preco = std::stod(precoStr);


                // Adiciona os produtos no lote
                Produto produto(nome, id, preco, descricao);
                estoque[codigo]->adicionarProduto(produto);

            }

        } catch (const std::exception& e) {
            throw std::runtime_error("Falha ao processar arquivo");
        }
    }

    arquivo.close();


}