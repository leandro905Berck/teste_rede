#include <iostream>
#include <cstdlib> // Para system() e getenv()
#include <string>

void executarComando(const std::string& comando) {
    std::cout << "Executando: " << comando << std::endl;
    system(comando.c_str());
}

bool testarConectividade(const std::string& url) {
    std::string comando = "ping -n 1 " + url;
    std::cout << "Testando conectividade com " << url << "..." << std::endl;
    int resultado = system(comando.c_str());
    return (resultado == 0);
}

int main() {
    std::string url;
    
    // Solicitar ao usuário o IP ou URL para testar
    std::cout << "Digite o IP ou URL para testar: ";
    std::getline(std::cin, url);

    // Testar conectividade
    if (testarConectividade(url)) {
        std::cout << "Conexão com " << url << " bem-sucedida." << std::endl;
    } else {
        std::cout << "Falha ao conectar com " << url << "." << std::endl;
    }
    
    // Executar ping
    std::string comandoPing = "ping " + url;
    executarComando(comandoPing);

    // Executar tracert
    std::string comandoTracert = "tracert " + url;
    executarComando(comandoTracert);

    // Perguntar ao usuário se deseja abrir o site
    std::cout << "Deseja abrir o site ISP Tools para testes mais avancados? (s/n): ";
    char resposta;
    std::cin >> resposta;
    
    if (resposta == 's' || resposta == 'S') {
        // Montar a URL para abrir no navegador
        std::string urlFinal = "http://www.isp.tools/caiu#" + url;
        std::string comandoAbrirSite = "start " + urlFinal;
        system(comandoAbrirSite.c_str());
    } else {
        std::cout << "O site não será aberto." << std::endl;
    }

    return 0;
}