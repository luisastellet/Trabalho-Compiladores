grade = 85
classification = "A" if (grade >= 90) else ("B" if (grade >= 80) else ("C" if (grade >= 70) else ("D" if (grade >= 60) else ("F"))))
print(classification, end='')
