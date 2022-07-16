import cryptography.x509
import ssl

certFile = 'utk.pem'
host = 'eecs.utk.edu'

f = open(certFile, 'w')  # open file for writing
certd = ssl.get_server_certificate((host, 443))  # getting google's certificate on port 443
f.write(certd)  # writing google's cert to the file
f.close()

cert = open(certFile, 'r')  # opens the certificate file
certStr = cert.read()  # reads in the certificate file
certByte = certStr.encode()  # encodes the file contents using UTF-8
x509 = cryptography.x509.load_pem_x509_certificate(certByte)  # turns the pem encoding into a x509

print("Expiration Date: ", x509.not_valid_after)
print("Subject: ", x509.subject)
