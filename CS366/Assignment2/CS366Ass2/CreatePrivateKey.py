p = 0xF7E75FDC469067FFDC4E847C51F452DF
q = 0xE85CED54AF57E53E092113E62F436F4F
e = 0x0D88C3
n = p * q

pn = (p - 1) * (q - 1)
d = pow(e, -1, pn)
print("d =", hex(d))