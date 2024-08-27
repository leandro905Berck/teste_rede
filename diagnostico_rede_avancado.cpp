#include <iostream>
#include <cstdlib> // Para system()
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <stdexcept>

// Função para executar um comando no sistema e capturar a saida
std::string executarComando(const std::string& comando) {
    std::string resultado;
    auto pipe = _popen(comando.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Nao foi possivel abrir o pipe.");
    }
    try {
        std::array<char, 128> buffer;
        std::string linha;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            linha = buffer.data();
            resultado += linha;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return resultado;
}

// Função para testar conectividade
bool testarConectividade(const std::string& url) {
    std::string comando = "ping -n 1 " + url;
    std::cout << "Testando conectividade com " << url << "..." << std::endl;
    std::string resultado = executarComando(comando);
    std::cout << resultado << std::endl;
    return resultado.find("Resposta de") != std::string::npos;
}

// Funcao para realizar um teste de velocidade
std::string testarVelocidade() {
    std::string comando = "speedtest-cli --simple";
    std::cout << "Executando teste de velocidade..." << std::endl;
    std::string resultado = executarComando(comando);
    std::cout << resultado << std::endl;
    return resultado;
}

// Função para verificar se o domínio está resolvendo
std::string verificarDNS(const std::string& url) {
    std::string comando = "nslookup " + url;
    std::cout << "Verificando DNS para " << url << "..." << std::endl;
    std::string resultado = executarComando(comando);
    std::cout << resultado << std::endl;
    return resultado;
}

// Função para verificar se uma porta está aberta
std::string verificarPorta(const std::string& url, int porta) {
    std::stringstream comando;
    comando << "powershell -Command \"Test-NetConnection -ComputerName " << url << " -Port " << porta << "\"";
    std::cout << "Verificando a porta " << porta << " em " << url << "..." << std::endl;
    std::string resultado = executarComando(comando.str());
    std::cout << resultado << std::endl;
    return resultado;
}

// Função para criar um nome de arquivo com base na data e hora atuais
std::string criarNomeArquivoDataHora() {
    auto agora = std::chrono::system_clock::now();
    auto tempo = std::chrono::system_clock::to_time_t(agora);
    std::tm tm = *std::localtime(&tempo);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

    return "resultados_" + ss.str() + ".txt";
}

// Função para registrar resultados em um arquivo no diretório atual
void registrarResultados(const std::string& resultados) {
    std::string nomeArquivo = criarNomeArquivoDataHora();
    std::ofstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        arquivo << resultados;
        arquivo.close();
    } else {
        std::cerr << "Nao foi possivel abrir o arquivo de resultados para gravacao." << std::endl;
    }
}

int main() {
    std::string url;

    // Solicitar ao usuário o IP ou URL para testar
    std::cout << "Digite o IP ou URL para testar: ";
    std::getline(std::cin, url);

    std::string resultados;

    // Testar conectividade
    bool conexaoOk = testarConectividade(url);
    resultados += "Conexao com " + url + (conexaoOk ? " bem-sucedida.\n" : " falhou.\n");

    // Executar ping
    std::string resultadoPing = executarComando("ping " + url);
    resultados += "\nResultado do ping:\n" + resultadoPing;

    // Executar tracert
    std::string resultadoTracert = executarComando("tracert " + url);
    resultados += "\nResultado do tracert:\n" + resultadoTracert;

    // Testar velocidade
    std::cout << "Deseja realizar um teste de velocidade? (s/n): ";
    char desejaVelocidade;
    std::cin >> desejaVelocidade;
    std::cin.ignore(); // Ignorar o caractere de nova linha

    if (desejaVelocidade == 's' || desejaVelocidade == 'S') {
        std::string resultadoVelocidade = testarVelocidade();
        resultados += "\nResultado do teste de velocidade:\n" + resultadoVelocidade;
    }

    // Verificar DNS
    std::string resultadoDNS = verificarDNS(url);
    resultados += "\nResultado da verificação de DNS:\n" + resultadoDNS;

    // Verificar porta
    std::cout << "Deseja verificar uma porta? (s/n): ";
    char desejaPorta;
    std::cin >> desejaPorta;
    if (desejaPorta == 's' || desejaPorta == 'S') {
        int porta;
        std::cout << "Digite o numero da porta para verificar: ";
        std::cin >> porta;
        std::string resultadoPorta = verificarPorta(url, porta);
        resultados += "\nResultado da verificacao da porta " + std::to_string(porta) + ":\n" + resultadoPorta;
    }

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
        std::cout << "O site nao sera aberto." << std::endl;
    }

    // Registrar todos os resultados no arquivo
    registrarResultados(resultados);

    return 0;
}
