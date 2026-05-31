def _let_0():
    def factorial(n):
        return (1 if (n == 0) else (n * factorial((n - 1))))
    return factorial(5)

_let_0()
