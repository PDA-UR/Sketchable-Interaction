from rply import ParserGenerator

from ast import *


class Parser:
    def __init__(self):
        self.pg = ParserGenerator(
            ['CAPABILITY', 'IDENTIFIER', 'SEPARATOR', 'END',
             'UNI_LINK', 'BI_LINK', 'ASSIGN_PROPERTY', 'ASSIGN_VALUE',
             'ASSIGN_ATTRIBUTE', 'OPEN_PAREN', 'CLOSE_PAREN', 'OPEN_SB',
             'CLOSE_SB', 'NUMBER', 'KEYWORD'
             ]
        )

    def parse(self):
        @self.pg.production('program : expression operator expression END')
        def program(p):
            return Statement(p[0], p[2], p[1], p[3])

        @self.pg.production('expression : expression operator expression')
        @self.pg.production('expression : IDENTIFIER operator IDENTIFIER')
        @self.pg.production('expression : CAPABILITY SEPARATOR CAPABILITY')
        @self.pg.production('expression : IDENTIFIER operator KEYWORD')
        @self.pg.production('expression : KEYWORD OPEN_PAREN KEYWORD CLOSE_PAREN')
        @self.pg.production('expression : KEYWORD OPEN_PAREN IDENTIFIER CLOSE_PAREN')
        @self.pg.production('expression : OPEN_SB CLOSE_SB')
        @self.pg.production('expression : OPEN_SB expression CLOSE_SB')
        @self.pg.production('expression : KEYWORD')
        @self.pg.production('expression : point SEPARATOR expression')
        @self.pg.production('expression : point')
        def expression(p):
            print(p)
            return None

        @self.pg.production('operator : UNI_LINK')
        @self.pg.production('operator : BI_LINK')
        @self.pg.production('operator : ASSIGN_PROPERTY')
        @self.pg.production('operator : ASSIGN_ATTRIBUTE')
        @self.pg.production('operator : ASSIGN_VALUE')
        def operator(p):
            return Operator(p[0])

        @self.pg.production('point : OPEN_PAREN NUMBER SEPARATOR NUMBER CLOSE_PAREN')
        def point(p):
            print(p)
            return Point()

        @self.pg.error
        def error_handle(token):
            raise ValueError(token)

    def get_parser(self):
        return self.pg.build()


if __name__ == '__main__':
    pass
