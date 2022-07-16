p = 0xF7E75FDC469067FFDC4E847C51F452DF
q = 0xE85CED54AF57E53E092113E62F436F4F
e = 0x0D88C3
d = 0x3587a24598e5f2a21db007d89d18cc50aba5075ba19a33890fe7c28a9b496aeb
n = p * q
C = 0x39cf8ba4e56530ae0a9d9d6072464fb253d55d4d5fdbd3789d28996a3f877879

def decrypt(num):
    return pow(C, d, n)

decNum = decrypt(C)
hexMsg = str(hex(decNum))[2:]
bytesMsg = bytes.fromhex(hexMsg)
asciiMsg = bytesMsg.decode("ASCII")
print(asciiMsg)