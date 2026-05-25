square = lambda x: (x * x)
apply_op = lambda op, a, b: (a * b) if (op == 3) else ((a - b) if (op == 2) else ((a + b) if (op == 1) else (0)))
combined_result = n1 = 10
n2 = 20
(square(n1) + apply_op(1, n1, n2))
