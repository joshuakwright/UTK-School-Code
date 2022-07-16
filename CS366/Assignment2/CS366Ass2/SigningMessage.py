p = 0xF7E75FDC469067FFDC4E847C51F452DF
q = 0xE85CED54AF57E53E092113E62F436F4F
d = 0x3587a24598e5f2a21db007d89d18cc50aba5075ba19a33890fe7c28a9b496aeb
n = p * q

def sign(message):
    return pow(message, d, n)

msg = "I owe you $2000"
numMsg = msg.encode()
intMsg = int.from_bytes(numMsg, "big")
signedMsg = sign(intMsg)
print("'", msg, "'", 'signed and in hex = ', hex(signedMsg))