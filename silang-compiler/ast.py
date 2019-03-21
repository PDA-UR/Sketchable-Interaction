class Statement:
    def __init__(self, left, right, op, end):
        self.left = left
        self.right = right
        self.op = op
        self.end = end

    def eval(self):
        print(self.left)
        print(self.op)
        print(self.right)
        print(self.end)

    def __repr__(self):
        return "Expression: {0}, {1}, {2}".format(self.left, self.op, self.right)


class Expression:
    def __init__(self, left, rigth, op):
        self.left = left
        self.right = rigth
        self.op = op

    def eval(self):
        print(self.left, self.right, self.op)

    def __repr__(self):
        return "Expression: {0}, {1}, {2}".format(self.left, self.op, self.right)


class Operator:
    def __init__(self, value):
       self.value = value

    def eval(self):
        print(self.value)

    def __repr__(self):
        return "Operator: " + str(self.value)


class Point:
    def __init__(self):
        self.value = 1



if __name__ == '__main__':
    pass
