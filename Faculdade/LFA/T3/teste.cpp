#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct AFN { // Estrutura do AFN
    set<string> estados;
    set<char> alfabeto;
    string estadoInicial;
    set<string> estadosFinais;
    map<string, map<char, set<string>>> transicoes;
    bool afn_epsilon;

    AFN(set<string> estados, set<char> alfabeto, string estadoInicial, set<string> estadosFinais, bool afn_epsilon)
        : estados(estados), alfabeto(alfabeto), estadoInicial(estadoInicial), estadosFinais(estadosFinais), afn_epsilon(afn_epsilon) {}
};

struct AFD { // Estrutura do AFD
    set<string> estados;
    set<char> alfabeto;
    string estadoInicial;
    set<string> estadosFinais;
    map<string, map<char, string>> transicoes;

    AFD(set<char> alfabeto) : alfabeto(alfabeto) {}
};

// Função para calcular o fecho-ε de um estado
set<string> calcularFechoEpsilon(const string& estado, const map<string, set<string>>& epsilonTransicoes) {
    set<string> fecho;
    queue<string> fila;
    fila.push(estado);
    fecho.insert(estado);

    while (!fila.empty()) {
        string atual = fila.front();
        fila.pop();

        auto it = epsilonTransicoes.find(atual);
        if (it != epsilonTransicoes.end()) {
            for (const string& proxEstado : it->second) {
                if (fecho.find(proxEstado) == fecho.end()) {
                    fecho.insert(proxEstado);
                    fila.push(proxEstado);
                }
            }
        }
    }

    return fecho;
}

string conjuntoParaString(const set<string>& conjunto) {
    string result = "{";
    for (const auto& elem : conjunto) {
        result += elem + ",";
    }
    if (result.length() > 1) result.pop_back();
    result += "}";
    return result;
}

AFD converterAFNparaAFD(AFN afn) {
    AFD afd(afn.alfabeto);
    map<set<string>, string> estadosConvertidos;
    map<string, set<string>> epsilonTransicoes;

    // Constroi a tabela de transições epsilon para cada estado
    if (afn.afn_epsilon) {
        for (const auto& estado : afn.estados) {
            if (afn.transicoes[estado].find('~') != afn.transicoes[estado].end()) {
                epsilonTransicoes[estado] = afn.transicoes[estado]['~'];
            }
        }
    }

    // Cálculo dos fechos-ε
    map<string, set<string>> fechosEpsilon;
    for (const string& estado : afn.estados) {
        fechosEpsilon[estado] = calcularFechoEpsilon(estado, epsilonTransicoes);
    }

    // Configurando o estado inicial do AFD
    set<string> fechoInicial = fechosEpsilon[afn.estadoInicial];
    queue<set<string>> fila;
    fila.push(fechoInicial);
    string nomeEstadoInicial = conjuntoParaString(fechoInicial);
    estadosConvertidos[fechoInicial] = nomeEstadoInicial;
    afd.estadoInicial = nomeEstadoInicial;
    afd.estados.insert(nomeEstadoInicial);

    while (!fila.empty()) {
        set<string> estadoAtual = fila.front();
        fila.pop();
        string estadoAFD = estadosConvertidos[estadoAtual];

        for (char simbolo : afn.alfabeto) {
            if (simbolo == '~') continue; // Ignorar o símbolo de epsilon
            set<string> novoEstadoConjunto;
            for (const string& estado : estadoAtual) {
                auto it = afn.transicoes[estado].find(simbolo);
                if (it != afn.transicoes[estado].end()) {
                    for (const string& destino : it->second) {
                        novoEstadoConjunto.insert(fechosEpsilon[destino].begin(), fechosEpsilon[destino].end());
                    }
                }
            }
            if (!novoEstadoConjunto.empty()) {
                string nomeNovoEstado = conjuntoParaString(novoEstadoConjunto);
                if (estadosConvertidos.find(novoEstadoConjunto) == estadosConvertidos.end()) {
                    estadosConvertidos[novoEstadoConjunto] = nomeNovoEstado;
                    fila.push(novoEstadoConjunto);
                    afd.estados.insert(nomeNovoEstado);
                }
                afd.transicoes[estadoAFD][simbolo] = nomeNovoEstado;
            } else {
                afd.transicoes[estadoAFD][simbolo] = "{}";
            }
        }
    }

    // Determinando os estados finais do AFD
    for (const auto& par : estadosConvertidos) {
        for (const string& estado : par.first) {
            if (afn.estadosFinais.find(estado) != afn.estadosFinais.end()) {
                afd.estadosFinais.insert(par.second);
                break;
            }
        }
    }

    return afd;
}

// Função para converter AFD para expressão regular usando o método de Kleene e Arden
string converterAFDparaExpressaoRegular(const AFD& afd) {
    map<string, map<string, string>> matriz;
    set<string> estados = afd.estados;
    vector<string> estadosVector(estados.begin(), estados.end());

    // Inicialização da matriz
    for (const auto& estado1 : estadosVector) {
        matriz[estado1] = map<string, string>();
        for (const auto& estado2 : estadosVector) {
            matriz[estado1][estado2] = "";
        }
    }

    // Preenchimento da matriz com transições do AFD
    for (const auto& trans : afd.transicoes) {
        string estadoAtual = trans.first;
        for (const auto& simbolo : trans.second) {
            matriz[estadoAtual][simbolo.second] += simbolo.first;
        }
    }

    // Algoritmo de Arden para obter a expressão regular
    for (const auto& estado : estadosVector) {
        for (const auto& destino : estadosVector) {
            string expressao = matriz[estado][destino];
            if (estado == destino) {
                if (expressao.empty()) {
                    matriz[estado][destino] = "ε";
                } else {
                    matriz[estado][destino] = "(" + expressao + ")*";
                }
            } else {
                if (!expressao.empty()) {
                    matriz[estado][destino] = expressao;
                }
            }
        }
    }

    // Montando a expressão regular a partir do estado inicial para os estados finais
    string regExp = "";
    string estadoInicial = afd.estadoInicial;
    for (const auto& finalEstado : afd.estadosFinais) {
        if (!regExp.empty()) regExp += " | ";
        regExp += matriz[estadoInicial][finalEstado];
    }

    return regExp;
}

void mostrarAFD(const AFD& afd) {
    cout << "Estados do AFD: ";
    for (const auto& estado : afd.estados) {
        cout << estado << " ";
    }
    cout << endl;

    cout << "Estado Inicial: " << afd.estadoInicial << endl;

    cout << "Estados Finais: ";
    for (const auto& estado : afd.estadosFinais) {
        cout << estado << " ";
    }
    cout << endl;

    cout << "Transições: " << endl;
    for (const auto& trans : afd.transicoes) {
        for (const auto& simb : trans.second) {
            cout << "δ(" << trans.first << ", " << simb.first << ") -> " << simb.second << endl;
        }
    }
}

int main() {
    char opcao = 's';

    while (opcao == 's' || opcao == 'S') {
        int numEstados, numSimbolos, numEstadosFinais;
        string estadoInicial;
        bool afn_epsilon;

        cout << "O autômato é um AFN-ε? (s/n): ";
        char resposta;
        cin >> resposta;
        afn_epsilon = (resposta == 's' || resposta == 'S');

        cout << "Número de estados no AFN: ";
        cin >> numEstados;

        set<string> estados;
        cout << "Estados (separados por espaço): ";
        for (int i = 0; i < numEstados; i++) {
            string estado;
            cin >> estado;
            estados.insert(estado);
        }

        cout << "Número de símbolos no alfabeto: ";
        cin >> numSimbolos;

        set<char> alfabeto;
        cout << "Símbolos do alfabeto (separados por espaço): ";
        for (int i = 0; i < numSimbolos; i++) {
            char simbolo;
            cin >> simbolo;
            alfabeto.insert(simbolo);
        }

        cout << "Estado inicial: ";
        cin >> estadoInicial;

        cout << "Número de estados finais: ";
        cin >> numEstadosFinais;

        set<string> estadosFinais;
        cout << "Estados finais (separados por espaço): ";
        for (int i = 0; i < numEstadosFinais; i++) {
            string estado;
            cin >> estado;
            estadosFinais.insert(estado);
        }

        AFN afn(estados, alfabeto, estadoInicial, estadosFinais, afn_epsilon);

        // Leitura das transições
        cout << "Número de transições: ";
        int numTransicoes;
        cin >> numTransicoes;

        cout << "Transições (estado origem, símbolo, estado destino): " << endl;
        for (int i = 0; i < numTransicoes; i++) {
            string estadoOrigem, estadoDestino;
            char simbolo;
            cin >> estadoOrigem >> simbolo >> estadoDestino;
            afn.transicoes[estadoOrigem][simbolo].insert(estadoDestino);
        }

        // Conversão para AFD e exibição
        AFD afd = converterAFNparaAFD(afn);
        mostrarAFD(afd);

        // Conversão do AFD para expressão regular
        string expressaoRegular = converterAFDparaExpressaoRegular(afd);
        cout << "Expressão Regular: " << expressaoRegular << endl;

        cout << "Deseja continuar? (s/n): ";
        cin >> opcao;
    }

    return 0;
}
