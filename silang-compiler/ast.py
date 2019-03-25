from rply import *
from rply.token import *
from collections import Iterable


class Statement(BaseBox):
    def __init__(self, expression, state):
        self.value = expression
        self.state = state

    def eval(self):
        self.value.eval(self.state)
        print(self.value)


class Variable(BaseBox):
    def __init__(self, name):
        self.name = str(name)
        self.value = None

    def __repr__(self):
        return 'Variable({0}, {1})'.format(self.name, self.value)

    def get_name(self):
        return self.name

    def eval(self, env):
        if env.variables.get(self.name, None) is not None:
            self.value = env.variables[self.name]
            self.value.eval(env)

            return self.value

        raise NameError("Not yet defined")

    def to_string(self):
        return str(self.name)


class Assignment(BaseBox):
    def __init__(self, variable, expression):
        self.variable = variable
        self.expression = expression

    def __repr__(self):
        return 'Assignment({0}, {1})'.format(self.variable, self.expression)

    def eval(self, env):
        env.variables[self.variable.name] = self.expression
        self.variable.value = self.expression
        self.variable.eval(env)


class Point(BaseBox):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return 'Point({0}, {1})'.format(self.x, self.y)

    def eval(self):
        return self


class List(BaseBox):
    def __init__(self, l):
        self.list = list(self.flatten(l))

    def __repr__(self):
        return 'List({0})'.format(", ".join([str(p) for p in self.list]))

    def eval(self, env):
        return self.list

    def flatten(self, l):
        for x in l:
            if isinstance(x, Iterable) and not isinstance(x, (str, bytes)):
                for sub_x in self.flatten(x):
                    yield sub_x
            else:
                yield x


class Shape(BaseBox):
    def __init__(self, typ, expression):
        self.type = typ
        self.expression = expression

    def eval(self, env):
        self.expression.eval(env)

    def __repr__(self):
        return 'Shape(type({0}), {1})'.format(self.type, self.expression)


class Region(BaseBox):
    def __init__(self, external_name, expression):
        self.external_name = external_name
        self.expression = expression

    def eval(self, env):
        self.expression.eval(env)

    def __repr__(self):
        return 'Region(name({0}), {1})'.format(self.external_name, self.expression)


class Link(BaseBox):
    def __init__(self, var1, var2, cap1, cap2, link_type):
        super(Link, self).__init__()

        self.left_var = var1
        self.right_var = var2
        self.left_cap = cap1
        self.right_cap = cap2
        self.link_type = link_type

    def eval(self, env):
        if self.left_var not in env.variables:
            raise NameError("{0} is not yet declared".format(self.left_var))

        if self.right_var not in env.variables:
            raise NameError("{0} is not yet declared".format(self.right_var))

    def __repr__(self):
        return "{0}(variables({1}, {2}), capabilities({3}, {4})".format(self.link_type, self.left_var, self.right_var, self.left_cap, self.right_cap)


class UnidirectionalLink(Link):
    def __init__(self, left, right, left_cap, right_cap):
        super(UnidirectionalLink, self).__init__(left, right, left_cap, right_cap, "UnidirectionalLink")


class BidirectionalLink(Link):
    def __init__(self, left, right, left_cap, right_cap):
        super(BidirectionalLink, self).__init__(left, right, left_cap, right_cap, 'BidirectionalLink')
