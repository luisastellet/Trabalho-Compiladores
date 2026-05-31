def _let_0():
    def factorial(n):
        return (1 if (n == 0) else (n * factorial((n - 1))))
    return print(factorial(5), end='')

_let_0()
