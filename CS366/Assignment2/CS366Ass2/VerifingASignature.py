e = 0x010001
n = 0xAE1CD4DC432798D933779FBD46C6E1247F0CF1233595113AA51B450F18116115
C = 0x643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6802F
corruptedC = 0x643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6803F

def decrypt(num):
    return pow(C, e, n)

decNum = decrypt(C)
hexMsg = str(hex(decNum))[2:]
bytesMsg = bytes.fromhex(hexMsg)
asciiMsg = bytesMsg.decode("ASCII")
print(asciiMsg)
print('Signature Verified\n')

decNum = decrypt(corruptedC)
hexMsg = str(hex(decNum))[2:]
bytesMsg = bytes.fromhex(hexMsg)
corruptedAsciiMsg = bytesMsg.decode("ASCII")
print(corruptedAsciiMsg)
print("This signature should be corrupted, but...")

hash = 'Launch a missile.'
if hash == corruptedAsciiMsg:
    print('Signature Match')
