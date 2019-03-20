
from lexer import Lexer



if __name__ == '__main__':
    text_input = """
    A <-> B => position, position
    """

    lexer = Lexer.Lexer()
    tokens = lexer.get_build().lex(text_input)

    for token in tokens:
        print(token)