#import os
import json

#Alunos
#Guilherme Zanan Piveta (48931)
#Arthur Kenji Murakami (48925)

class TuringMachine:
    def __init__(self, states, alphabet, tape_alphabet, transitions, start_state, accept_state, reject_state,
                 blank_symbol):
        self.states = states
        self.alphabet = alphabet
        self.tape_alphabet = tape_alphabet
        self.transitions = transitions
        self.start_state = start_state
        self.accept_state = accept_state
        self.reject_state = reject_state
        self.blank_symbol = blank_symbol
        self.current_state = start_state
        self.tape = []
        self.head_position = 0

    def initialize(self, input_string):
        self.tape = list(input_string) + [self.blank_symbol] * 10
        self.current_state = self.start_state
        self.head_position = 0

    def step(self):
        current_symbol = self.tape[self.head_position]

        key = (self.current_state, current_symbol)
        if key not in self.transitions:
            return False

        next_state, write_symbol, direction = self.transitions[key]
        self.tape[self.head_position] = write_symbol
        self.current_state = next_state

        if direction == 'D' or direction == 'R':
            self.head_position += 1
        elif direction == 'E' or direction == 'L':
            self.head_position = max(0, self.head_position - 1)

        return True

    def run(self):
        while self.current_state != self.accept_state and self.current_state != self.reject_state:
            if not self.step():
                break

        return self.current_state == self.accept_state

    def dict_to_struct(data):
        transitions = {
            tuple(k.split(",")): tuple(v.split(","))
            for k,v in data["transitions"].items()
        }
        return TuringMachine(
            states=data["states"],
            alphabet=data["alphabet"],
            tape_alphabet=data["tapeAlpha"],
            transitions=transitions,
            start_state=data["startState"],
            accept_state=data["accept"],
            reject_state=data["reject"],
            blank_symbol=data["blankSymbol"]
        )


def makeTuringMachine():
    states = input("Digite os estados separados por vírgulas: ").strip().split(",")
    alphabet = input("Digite o alfabeto de entrada separado por vírgulas: ").strip().split(",")
    tape_alphabet = input("Digite o alfabeto da fita separado por vírgulas: ").strip().split(",")
    blank_symbol = input("Digite o símbolo em branco (normalmente '_'): ").strip()
    start_state = input("Digite o estado inicial: ").strip()
    accept_state = input("Digite o estado de aceitação: ").strip()
    reject_state = input("Digite o estado de rejeição: ").strip()

    # Definir as transições
    print("Digite as transições no formato: estado_atual,símbolo_lido -> próximo_estado,símbolo_escrito,ação")
    print("Por exemplo: q0,1 -> q1,0,R (estado q0, lê 1, vai para q1, escreve 0, move para a direita)")
    transitions = {}
    while True:
        transition = input("Digite uma transição (ou pressione Enter para finalizar): ")
        if not transition:
            break
            # Validar o formato da transição
        try:
            left, right = transition.split("->")
            left_parts = left.strip().split(",")
            right_parts = right.strip().split(",")

            if len(left_parts) != 2 or len(right_parts) != 3:
                raise ValueError("Formato incorreto.")

            current_state, read_symbol = left_parts
            next_state, write_symbol, direction = right_parts

            if direction not in {'E', 'D'}:
                raise ValueError("Ação deve ser 'E' ou 'D'.")

            # Adicionar transição válida
            transitions[(current_state.strip(), read_symbol.strip())] = (
                next_state.strip(),
                write_symbol.strip(),
                direction.strip()
            )
        except ValueError as e:
            print(
                f"Erro no formato: {e}. Tente novamente seguindo o formato estado,símbolo -> estado,símbolo,ação.")
    return TuringMachine(states, alphabet, tape_alphabet, transitions, start_state, accept_state, reject_state,blank_symbol)

def saveInFile(tm,filename):

    if tm is not None:
        data = {
            "states": tm.states,
            "alphabet": tm.alphabet,
            "tapeAlpha": tm.tape_alphabet,
            "startState": tm.start_state,
            "blankSymbol": tm.blank_symbol,
            "accept": tm.accept_state,
            "reject": tm.reject_state,
            "transitions": {f"{k[0]},{k[1]}":f"{v[0]},{v[1]},{v[2]}"
                            for k,v in tm.transitions.items()
                            }
        }
        if ".json" not in filename:
            filename = filename + ".json"
        try:
            with open(filename, "w") as json_file:
                json.dump(data,json_file,indent=4)
        except:
            print("Falha ao criar o arquivo")
    else:
        print("Maquina de turing vazia")

def loadFromFile(filename):
    try:
        if ".json" not in filename:
            filename = filename + ".json"
        with open(filename, "r") as json_file:
            data = json.load(json_file)
        return TuringMachine.dict_to_struct(data)
    except FileNotFoundError:
        print(f"{filename} não existe")
    except json.JSONDecodeError:
        print("Arquivo no formato incorreto")
    return None


def main():
    tm = None
    while True:
        print("\nEscolha uma das opçoes")
        print("1. Construir máquina de turing")
        print("2. rodar máquina de turing")
        print("3. salvar MT em um arquivo")
        print("4. carregar MT de um arquivo")
        esc = input().strip()

        if esc == '1':
           tm = makeTuringMachine()
        if esc == '2':
            if tm is not None:
                input_string = input("Digite a cadeia de entrada para a máquina: ")
                tm.initialize(input_string)
                result = tm.run()

                print("Resultado: ", "Aceito" if result else "Rejeitado")
                print("Fita final: ", "".join(tm.tape).strip(tm.blank_symbol))
                print("Estado final: ", tm.current_state)
            else:
                print("Nenhuma máquina de turing encontrada")
        if esc == '3':
            print("digite o nome do arquivo")
            filename = input()
            saveInFile(tm,filename)
        if esc == '4':
            print("digite o nome do arquivo")
            filename = input()
            tm = loadFromFile(filename)


        #os.system('clear')


if __name__ == "__main__":
    main()
