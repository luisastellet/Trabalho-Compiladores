def _lambda_0(x):
    return (x * x)

square = _lambda_0
def _lambda_1(op, a, b):
    return (a + b) if (op == 1) else ((a - b) if (op == 2) else ((a * b) if (op == 3) else (0)))

apply_op = _lambda_1
def _let_2():
    n1 = 10
    n2 = 20
    return (square(n1) + apply_op(1, n1, n2))

combined_result = _let_2()
print(combined_result, end='')
