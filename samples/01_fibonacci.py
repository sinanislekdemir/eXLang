count = 0
n1, n2 = 0, 1
nterms = 40

while count < nterms:
    print(n1)
    nth = n1 + n2
    # update values
    n1 = n2
    n2 = nth
    count += 1
