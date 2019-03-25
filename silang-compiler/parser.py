from rply import ParserGenerator

from ast import *


class Parser:
    def __init__(self):
        self.pg = ParserGenerator(
            ['CAPABILITY', 'SEPARATOR', 'END', 'IDENTIFIER',
             'UNI_LINK', 'BI_LINK', 'ASSIGN_PROPERTY', 'ASSIGN_VALUE',
             'ASSIGN_ATTRIBUTE', 'OPEN_PAREN', 'CLOSE_PAREN', 'OPEN_SB',
             'CLOSE_SB', 'NUMBER', 'REGION', 'TYPE', 'PRESENT', 'BLUEPRINT', 'SHAPE'
             ]
        )

    def parse(self):
        @self.pg.production('statement : expression END')
        def statement(state, p):
            return Statement(p[0], state)

        @self.pg.production('expression : IDENTIFIER UNI_LINK IDENTIFIER ASSIGN_PROPERTY CAPABILITY SEPARATOR CAPABILITY')
        @self.pg.production('expression : IDENTIFIER BI_LINK IDENTIFIER ASSIGN_PROPERTY CAPABILITY SEPARATOR CAPABILITY')
        def expression_link_two_regions_by_two_capabilities(state, p):
            op = p[1].getstr()

            if op == '->':
                return UnidirectionalLink(p[0].getstr(), p[2].getstr(), p[4].getstr(), p[6].getstr())

            if op == '<->':
                return BidirectionalLink(p[0].getstr(), p[2].getstr(), p[4].getstr(), p[6].getstr())

        @self.pg.production('expression : IDENTIFIER ASSIGN_VALUE expression')
        def expression_assignment_value(state, p):
            return Assignment(Variable(p[0].getstr()), p[2])

        @self.pg.production('expression : REGION ASSIGN_PROPERTY TYPE OPEN_PAREN IDENTIFIER CLOSE_PAREN')
        def expression_assign_named_type_to_region(state, p):
            return Region(p[4].getstr(), None)

        @self.pg.production('expression : REGION ASSIGN_PROPERTY TYPE OPEN_PAREN IDENTIFIER CLOSE_PAREN ASSIGN_ATTRIBUTE expression')
        def expression_assign_named_type_to_region_with_attribute(state, p):
            return Region(p[4].getstr(), p[7])

        @self.pg.production('expression : SHAPE ASSIGN_PROPERTY TYPE OPEN_PAREN PRESENT CLOSE_PAREN ASSIGN_VALUE expression')
        def expression_assign_present_keyword_type_to_shape_with_points(state, p):
            return Shape(p[4].getstr(), p[-1])

        @self.pg.production('expression : SHAPE ASSIGN_PROPERTY TYPE OPEN_PAREN BLUEPRINT CLOSE_PAREN ASSIGN_VALUE expression')
        def expression_assign_blueprint_keyword_type_to_shape_with_points(state, p):
            return Shape(p[4].getstr(), p[-1])

        @self.pg.production('expression : OPEN_SB CLOSE_SB')
        def expression_empty_point_list(state, p):
            return List([])

        @self.pg.production('expression : OPEN_SB expression CLOSE_SB')
        def expression_point_list(state, p):
            return List(p[1])

        @self.pg.production('expression : point SEPARATOR expression')
        def expression_point_with_separator(state, p):
            return [p[0], p[2]]

        @self.pg.production('expression : point')
        def expression_point_without_separator(state, p):
            return [p[0]]

        @self.pg.production('point : OPEN_PAREN NUMBER SEPARATOR NUMBER CLOSE_PAREN')
        def point(state, p):
            return Point(p[1].getstr(), p[3].getstr())

        @self.pg.error
        def error_handle(token):
            raise ValueError(token, token.getsourcepos(), token.getstr())

    def get_parser(self):
        return self.pg.build()


if __name__ == '__main__':
    pass
