#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <string>

using namespace std;

struct AFN { //estrutura do AFN
    set<string> estados;
    set<char> alfabeto;
    string estadoInicial;
    set<string> estadosFinais;
    map<string, map<char, set<string>>> transicoes;
    bool afn_epsilon;

    AFN(set<string> estados, set<char> alfabeto, string estadoInicial, set<string> estadosFinais, bool afn_epsilon)
        : estados(estados), alfabeto(alfabeto), estadoInicial(estadoInicial), estadosFinais(estadosFinais), afn_epsilon(afn_epsilon) {}
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

struct AFD { //Estrutura do AFD
    set<string> estados;
    set<char> alfabeto;
    string estadoInicial;
    set<string> estadosFinais;
    map<string, map<char, string>> transicoes;

    AFD(set<char> alfabeto) : alfabeto(alfabeto) {}
};

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

        cout << "Defina as transições (exemplo: q0 a q1 ou q0 ~ q1 para epsilon): " << endl;
        for (const auto& estado : estados) {
            for (char simbolo : afn.alfabeto) {
                int numTransicoes;
                cout << "Número de transições do estado " << estado << " com o símbolo " << simbolo << ": ";
                cin >> numTransicoes;

                set<string> destinos;
                for (int j = 0; j < numTransicoes; j++) {
                    string destino;
                    cout << "Destino: ";
                    cin >> destino;
                    destinos.insert(destino);
                }
                afn.transicoes[estado][simbolo] = destinos;
            }
            if (afn_epsilon) {
                int numTransicoes;
                cout << "Número de transições do estado " << estado << " com o símbolo ~ (epsilon): ";
                cin >> numTransicoes;

                set<string> destinos;
                for (int j = 0; j < numTransicoes; j++) {
                    string destino;
                    cout << "Destino: ";
                    cin >> destino;
                    destinos.insert(destino);
                }
                afn.transicoes[estado]['~'] = destinos;
            }
        }

        AFD afd = converterAFNparaAFD(afn);

        mostrarAFD(afd);

        cout << "\nDeseja inserir um novo autômato? (s/n): ";
        cin >> opcao;
    }

    cout << "Programa encerrado." << endl;
    return 0;
}
