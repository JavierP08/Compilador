import string

NUM_STATES = 8
ERROR_STATE = 7

TokenIds = {
    "class": 1,
    "main": 2,
    "{": 3,
    "}": 4,
    "(": 5,
    ")": 6
}

TT = [[ERROR_STATE for _ in range(256)] for _ in range(NUM_STATES)]
identifier_table = {}
total_identifiers = 0


def start_table():
    for ch in range(256):
        TT[0][ch] = ERROR_STATE
        TT[1][ch] = ERROR_STATE

    TT[0][ord('l')] = 1
    TT[0][ord('d')] = ERROR_STATE
    TT[0][ord('_')] = 1
    TT[0][ord('{')] = 3
    TT[0][ord('}')] = 4
    TT[0][ord('(')] = 5
    TT[0][ord(')')] = 6
    TT[0][ord('b')] = 0

    for ch in [ord('l'), ord('d'), ord('_')]:
        TT[1][ch] = 1
    for ch in [ord('b'), ord('{'), ord('}'), ord('('), ord(')')]:
        TT[1][ch] = 2


def classify(ch):
    if ch in string.ascii_letters:
        return 'l'
    elif ch in string.digits:
        return 'd'
    elif ch.isspace():
        return 'b'
    else:
        return ch


def get_token_id(token):
    return TokenIds.get(token, 7)


def get_or_insert_identifier(word):
    global total_identifiers
    if word not in identifier_table:
        total_identifiers += 1
        identifier_table[word] = total_identifiers
    return identifier_table[word]


def record_token(word, token_file):
    token_id = get_token_id(word)
    if token_id == 7:
        id_identifier = get_or_insert_identifier(word)
        token_file.write(f"<{token_id},{id_identifier}>\n")
    else:
        token_file.write(f"<{token_id}>\n")


def is_acceptance(state):
    return 2 <= state <= 6


def is_error(state):
    return state == ERROR_STATE


def should_advance(state, ch):
    return ch in {'l', 'd', '_'} and state in {0, 1}


def main():
    start_table()

    with open("test_case3.java", "r") as process_file, open("tokens.txt", "w") as token_file:
        content = process_file.read()
        i = 0
        length = len(content)

        while i < length:
            state = 0
            word = ""
            ch = content[i]
            current_ch = ch

            while i < length and not is_acceptance(state) and not is_error(state):
                ch = classify(current_ch)
                state = TT[state][ord(ch)]

                if ch == 'b':
                    i += 1
                    if i < length:
                        current_ch = content[i]
                elif should_advance(state, ch):
                    word += current_ch
                    i += 1
                    if i < length:
                        current_ch = content[i]
                else:
                    break

            if is_acceptance(state):
                if 2 < state < 7:
                    word += current_ch
                    record_token(word, token_file)
                    i += 1
                else:
                    record_token(word, token_file)
            elif is_error(state):
                if word:
                    record_token(word, token_file)
                i += 1


if __name__ == "__main__":
    main()
