
from lexer import Lexer
from parser import Parser
from context import Context

import os


def locate(root):
    source_files = []

    for root, dirs, files in os.walk(root):
        for file in files:
            if file.endswith(".si"):
                source_files.append(os.path.join(root, file))

    return source_files


if __name__ == '__main__':
    source_files = locate(os.path.dirname(os.path.realpath(__file__)))

    for src_file in source_files:
        with open(src_file, 'r') as src:
            c = Context()
            data = ("".join(src.read().split()))

            data = data.split(';')

            if data[-1] is not '':
                raise ValueError("Missing semicolon! {0} `;` required".format(data[-1]))

            for line in data:
                if line == '':
                    continue

                line += ';'

                tokens = Lexer().get_build().lex(line)

                pg = Parser()
                pg.parse()
                parser = pg.get_parser()

                try:
                    parser.parse(tokens, c).eval()
                except TypeError:
                    raise ValueError("Missing semicolon between statements! {0}".format(line))