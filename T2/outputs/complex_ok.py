square = lambda x: (x * x)
apply_op = lambda op, a, b: (a * b) if (op == 3) else ((a - b) if (op == 2) else ((a + b) if (op == 1) else (0)))
combined_result = (lambda n1, n2: (square(n1) + apply_op(1, n1, n2)))(10, 20)
