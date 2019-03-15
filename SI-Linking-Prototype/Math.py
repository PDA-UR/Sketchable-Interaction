import math


class Math:
    @staticmethod
    def dot(u, v):
        return sum((a * b) for a, b in zip(u, v))

    @staticmethod
    def vector_norm(v):
        return math.sqrt(Math.dot(v, v))

    @staticmethod
    def normalize_vector(v):
        n = float(Math.vector_norm(v))

        if n != 0:
            return [float(v[i]) / n for i in range(len(v))]
        else:
            return [-1 for i in range(len(v))]
