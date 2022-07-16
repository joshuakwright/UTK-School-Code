import ssl

f = open('utk.pem', 'w')  # open file for writing
cert = ssl.get_server_certificate(('eecs.utk.edu', 443))  # getting google's certificate on port 443
f.write(cert)  # writing google's cert to the file