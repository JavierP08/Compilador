import string

# Diccionario de transiciones: (estado_actual, clase_caracter) -> nuevo_estado
TT = {}

# Tabla de tokens reservados y sus IDs
token_ids = {
    "class": 1,
    "main": 2,
    "{": 3,
    "}": 4,
    "(": 5,
    ")": 6
}

# Tabla de identificadores
identifier_table = {}
total_of_identifiers = 0

def add_transition(state_from, char_class, state_to):
    TT[(state_from, char_class)] = state_to

def start_table():
    TT.clear()

    # Estado 0
    add_transition(0, 'l', 1)
    add_transition(0, 'd', 7)
    add_transition(0, '_', 1)
    add_transition(0, '{', 3)
    add_transition(0, '}', 4)
    add_transition(0, '(', 5)
    add_transition(0, ')', 6)
    add_transition(0, 'b', 0)

    # Estado 1
    add_transition(1, 'l', 1)
    add_transition(1, 'd', 1)
    add_transition(1, '_', 1)
    add_transition(1, '{', 2)
    add_transition(1, '}', 2)
    add_transition(1, '(', 2)
    add_transition(1, ')', 2)
    add_transition(1, 'b', 2)

def classify(char):
    if char.isalpha():
        return 'l'
    elif char.isdigit():
        return 'd'
    elif char.isspace():
        return 'b'
    elif char in '{}()_':
        return char
    else:
        return char

def get_token_id(token):
    return token_ids.get(token, 7)  # 7 = identificador

def insert_identifier(identifier):
    global total_of_identifiers
    if identifier not in identifier_table:
        total_of_identifiers += 1
        identifier_table[identifier] = total_of_identifiers
    return identifier_table[identifier]

def record_token(token_str, token_file):
    token_id = get_token_id(token_str)
    if token_id == 7:  # identificador
        id_identifier = insert_identifier(token_str)
        token_file.write(f"<{token_id},{id_identifier}>\n")
    else:
        token_file.write(f"<{token_id}>\n")

def acceptance(state):
    return state in {2, 3, 4, 5, 6}

def error(state):
    return state == 7

def advance(state, ch_class):
    return ch_class in {'l', 'd', '_'} and state in {0, 1}

def main():
    start_table()

    with open("ExampleC.txt", "r") as process_file, open("tokens.txt", "w") as token_file:
        content = process_file.read()
        i = 0
        while i < len(content):
            state = 0
            palabra = ""

            while i < len(content) and not acceptance(state) and not error(state):
                current_ch = content[i]
                ch_class = classify(current_ch)
                state = TT.get((state, ch_class), 7)

                if ch_class == 'b':
                    i += 1
                    continue

                if advance(state, ch_class):
                    palabra += current_ch
                    i += 1
                elif not acceptance(state) and not error(state):
                    i += 1

            if acceptance(state):
                if state in {3, 4, 5, 6} and i < len(content):
                    palabra += content[i]
                    i += 1
                record_token(palabra, token_file)

            elif error(state):
                i += 1  # skip error character

if __name__ == "__main__":
    main()
