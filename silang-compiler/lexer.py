from rply import LexerGenerator


class Lexer(LexerGenerator):
    def __init__(self):
        super(Lexer, self).__init__()
        self._add_tokens()

    def _add_tokens(self):

        self.add('NUMBER', r'\d+')
        self.add('CAPABILITY', r'position')
        self.add('KEYWORD', r'type')
        self.add('KEYWORD', r'region')
        self.add('KEYWORD', r'present')
        self.add('KEYWORD', r'blueprint')
        self.add('KEYWORD', r'shape')
        self.add('OPEN_PAREN', r'\(')
        self.add('CLOSE_PAREN', r'\)')
        self.add('OPEN_SB', r'\[')
        self.add('CLOSE_SB', r'\]')
        self.add('IDENTIFIER', r'[a-zA-Z]+')
        self.add('SEPARATOR', "\,")
        self.add('END', r'\;')
        self.add('UNI_LINK', r'->')
        self.add('BI_LINK', r'<->')
        self.add('ASSIGN_PROPERTY', r'=>')
        self.add('ASSIGN_VALUE', r':=')
        self.add('ASSIGN_ATTRIBUTE', r'&')

        self.ignore('\s+')

    def get_build(self):
        return self.build()


if __name__ == '__main__':
    pass

"""
Token('PRINT', 'print')
Token('OPEN_PAREN', '(')
Token('NUMBER', '4')
Token('SUM', '+')
Token('NUMBER', '4')
Token('SUB', '-')
Token('NUMBER', '2')
Token('CLOSE_PAREN', ')')
Token('SEMI_COLON', ';')
"""