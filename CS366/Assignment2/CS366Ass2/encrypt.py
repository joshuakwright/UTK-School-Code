p = 0xF7E75FDC469067FFDC4E847C51F452DF
q = 0xE85CED54AF57E53E092113E62F436F4F
e = 0x0D88C3
n = p * q

def encrypt(message):
    return pow(message, e, n)

msg = "A top secret!"
numMsg = msg.encode()
intMsg = int.from_bytes(numMsg, "big")
encryptedMsg = encrypt(intMsg)
print("'", msg, "'", 'encrypted and in hex =', hex(encryptedMsg))
