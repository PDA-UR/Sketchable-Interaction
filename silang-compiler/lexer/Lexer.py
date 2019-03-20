from rply import LexerGenerator


class Lexer(LexerGenerator):
    def __init__(self):
        super(Lexer, self).__init__()
        self._add_tokens()

    def _add_tokens(self):
        self.add('statement', r'identifier operator identifier operator capability "," capability')
        self.add('identifier', r'[a-zA-Z]+')
        self.add('uni_link', r'->')
        self.add('bi_link', r'<->')
        self.add('assign_property', r'=>')
        self.add('capability', r'position')
        self.add('comma', r',')
        self.ignore('\s+')

    def get_build(self):
        return self.build()
