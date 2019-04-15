class Context:
    def __init__(self):
        self.variables = {}

    def __repr__(self):
        return "context: \n\tvariables: {0}".format(self.variables)