import cryptography.x509
from cryptography.hazmat.primitives.asymmetric import rsa
import os
import warnings

directory = 'C:/Users/mrjos/Desktop/Certs/CompCerts'  # directory with all the certs to scan

fileTypes = {}  # dict to keep track of file types
firstIssue = None  # variable to store earliest issuance date
lastIssue = None  # variable to store latest issuance date
firstExp = None  # variable to store earliest expiration
lastExp = None  # variable to store latest expiration date
algCount = {}  # variable to store the count of each public-key algorithm
eCount = {}  # variable to store the count of each e value
RSALengthTooShortCount = 0  # variable to store how many certificates are using RSA keys of insecure length
selfSignedCount = 0  # variable to store how many certificates are self-signed
leafCount = {}  # variable to store the count of each issuer's leaf certificates
publicKeyCount = {}  # variable to store the public keys and the count of each

progress = 0  # tracks progress of going through files
prevProgress = 0  # tracks last outputted percentage to know when to print the next one

for filename in os.scandir(directory):  # opens certification directory and loops through all of the files
    cert = open(filename, 'r')  # opens the certificate file
    certStr = cert.read()  # reads in the certificate file
    certByte = certStr.encode()  # encodes the file contents using UTF-8

    startChar = certStr[0:26]  # gets beginning of file
    pemChar = "-----BEGIN CERTIFICATE----"  # if this is at the top of a file, it is a PEM file
    if startChar.strip() == pemChar:  # if beginning of file is in this format, file is pem
        fileType = "PEM"  # set filetype to pem
    else:  # if beginning of file is not in the format above, its DER
        fileType = "DER"  # set filetype to der

    if fileType in fileTypes.keys():  # if the filetype has already been seen, increment its count
        fileTypes[fileType] += 1  # increment the file count
    else:  # if the filetype hasn't been seen, add to dict and set its count to 1
        fileTypes[fileType] = 1  # filetype inserted and count set to 1

    try:  # try except block to catch parsing errors due to negative serial number
        with warnings.catch_warnings():  # negative serial numbers cause warnings so need to use this to catch that
            # and raise exception
            warnings.simplefilter("error")  # filter the warnings for errors
            if fileType == "PEM":  # parse file if pem type with pem decoder
                x509 = cryptography.x509.load_pem_x509_certificate(certByte)  # turns the pem encoding into a x509
                # object
            else:  # parse file if der with der decoder
                x509 = cryptography.x509.load_der_x509_certificate(certByte)  # turns the der encoding into a x509
                # object
    except Exception as ex:  # print file name and error
        print(ex)  # print error
        print(filename)  # print file name

    if firstIssue == None:  # used to determine if tracking variables need to be initialized
        firstIssue = (filename, x509.not_valid_before)  # initializes firstIssue variable with first file issue date
        lastIssue = (filename, x509.not_valid_before)  # initializes lastIssue variable with the first file issue date
        firstExp = (filename, x509.not_valid_after)  # initializes firstExp variable with the first file expiration date
        lastExp = (filename, x509.not_valid_after)  # initializes lastExp variable with the first file expiration date

    if firstIssue[1] > x509.not_valid_before:  # Updates earliest issue date as new ones are found
        firstIssue = (filename, x509.not_valid_before)  # sets new earliest issue date
    if lastIssue[1] < x509.not_valid_before:  # updates latest issue date as new ones are found
        lastIssue = (filename, x509.not_valid_before)  # sets new latest issue date
    if firstExp[1] > x509.not_valid_after:  # updates earliest expiration date as new ones are found
        firstExp = (filename, x509.not_valid_after)  # sets new earliest expiration date
    if lastExp[1] < x509.not_valid_after:  # updates latest expiration date as new ones are found
        lastExp = (filename, x509.not_valid_after)  # sets new latest expiration date

    try:  # a try except block was implemented because there were some certificates with incorrect formatting,
        # resulting in the program quitting. This allows it to continue through all the certificates
        if x509.signature_hash_algorithm.name in algCount.keys():  # checks if the algorithm is already in the dict
            algCount[x509.signature_hash_algorithm.name] += 1  # if it is, it increments the count by 1
        else:
            algCount[x509.signature_hash_algorithm.name] = 1  # if it is not, it adds it and sets the count to 1
    except:
        continue  # skips ill formatted certificates

    if isinstance(x509.public_key(), rsa.RSAPublicKey):  # checks if the key is RSA since those are the only ones
        # with an e value
        if x509.public_key().public_numbers().e in eCount.keys():  # checks if the e value is in the dict
            eCount[x509.public_key().public_numbers().e] += 1  # if the value is, it increments its count by 1
        else:
            eCount[x509.public_key().public_numbers().e] = 1  # if it's not, it adds it and sets the count to 1

        if x509.public_key().key_size < 2048:  # checks if the key is of insecure length
            RSALengthTooShortCount += 1  # if key is too short, it increments the insecure key length count by 1

    try:  # a try except block was implemented because there were some certificates with incorrect formatting,
        # resulting in the program quitting. This allows it to continue through all the certificates
        if x509.issuer.public_bytes() == x509.subject.public_bytes():  # checks to see if the issuer is the same as
            # the subject to tell if the certificate is self signed
            selfSignedCount += 1  # if the ticket is self signed, it increments the self signed ticket count by 1
        else:  # only looking at certificates that are not self signed (since there are no root ca tickets in this
            # collection) to count the issuer with the most leaf certificates
            if x509.issuer in leafCount.keys():  # checks if the issuer is in the dict
                leafCount[x509.issuer] += 1  # if the issuer is in the dict, increment leaf count by 1
            else:
                leafCount[x509.issuer] = 1  # if the issuer is not in the dict, add it and set count to 1
    except:
        continue  # skips the ill formatted tickets

    if x509.public_key() in publicKeyCount.keys():  # checks to see if the public key has been seen yet
        publicKeyCount[x509.public_key()] += 1  # if it has, increment its count by 1
    else:
        publicKeyCount[x509.public_key()] = 1  # if it hasn't, add it to the dict and set count to 1

    percentProgress = int((progress / 205938) * 100)  # Calculates the percent completion of the program
    if percentProgress == prevProgress + 1:  # Only prints out percentage every time it goes up by 1
        print(percentProgress, "%")  # prints progress
        prevProgress = percentProgress  # updates prevProgress to be used in judging when to print the next percentage
    progress += 1  # increments the count of files read

for key in fileTypes:  # loops through dictionary of file types (only two entries, PEM and DER)
    print("FileType: ", key, " Count: ", fileTypes[key])  # Prints file types and their quantities

print("Earliest Issue Date: ", firstIssue)  # prints earliest issue date
print("Latest Issue Date: ", lastIssue)  # prints latest issue date
print("Earliest Expiration Date: ", firstExp)  # prints earliest expiration date
print("Latest Expiration Date: ", lastExp)  # prints latest expiration date

mostUsedAlg = None  # variable to store the most used algorithm in
mostUsedAlgCount = 0  # variable to store the count of the most used algorithm
for key in algCount:  # loops through every algorithm in the algCount dict to find the most used algorithm
    if mostUsedAlg == None:  # initializes the mostUsedAlg variable with the first algorithm if in the first loop
        mostUsedAlg = key  # sets the mostUsedAlg to the first alg
        mostUsedAlgCount = algCount[key]  # sets mostUsedAlgCount to the count of the first algorithm
    elif algCount[key] > mostUsedAlgCount:  # if the current algorithm has a greater count than the one stored in
        # mostUsedAlgCount, then the most used alg is updated
        mostUsedAlg = key  # mostUsedAlg is updated to current alg
        mostUsedAlgCount = algCount[key]  # mostUsedAlgCount is updated to current alg count
print("Most used algorithm: ", mostUsedAlg)  # prints the most used algorithm

mostUsedE = None  # variable to store the most used e value
mostUsedECount = 0  # variable to store the count of the most stored e value
for key in eCount:  # loops through all of the e values to find the most used e value
    if mostUsedE == None:  # initializes the mostUsedE variable with the first e value if in the first loop
        mostUsedE = key  # sets the mostUsedE to the e value of the first e value in eCount
        mostUsedECount = eCount[key]  # sets the mostUsedECount to the count of the first e value
    elif eCount[key] > mostUsedECount:  # if the current e value has a greater count than the one stored in
        # mostUsedECount, the the most used e value is updated
        mostUsedE = key  # mostUsedE is updated to the current e value
        mostUsedECount = eCount[key]  # mostUsedECount is updated to the count of the current e value
print("Most common e: ", mostUsedE)  # prints the most used e value

print(RSALengthTooShortCount, "certificates using RSA keys of insecure length")  # prints the count of the RSA keys
# that are of insecure length
print(selfSignedCount, "certificates are self-signed")  # prints the count of self signed certificates

issuerWithMostLeafs = None  # variable to store the issuer with the most leafs
mostLeafCount = 0  # variable to store the count of the most leafs stored by an issuer
for key in leafCount:  # loops through all of the issuers to find the one with the most leafs
    if issuerWithMostLeafs == None:  # initializes the issuerWithMostLeafs variable with the first issuer value if in
        # the first loop
        issuerWithMostLeafs = key  # sets the issuerWithMostLeafs to the issuer of the first issuer in leafCount
        mostLeafCount = leafCount[key]  # sets the mostLeafCount to the count of the first issuers leafs
    elif leafCount[key] > mostLeafCount:  # if the current leaf count is greater than the one stored in
        # mostLeafCount, the issuerWithMostLeafs is updated
        issuerWithMostLeafs = key  # issuerWithMostLeafs is updated to the current issuer
        mostLeafCount = leafCount[key]  # mostLeafCount is updated to the leaf count of the current issuer

issuer = issuerWithMostLeafs.rfc4514_string()
print("Issuer with the most leafs: ", issuer)  # prints the issuer with the most leafs

samePublicKeyCount = 0  # variable to hold the count of public keys with the same public key
for key in publicKeyCount:  # loops through list of all public keys and their use count
    if publicKeyCount[key] > 1:  # if the count of a public key is greater than 1, that means more than one
        # certificate used that public key, and therefore use the same public key
        samePublicKeyCount += publicKeyCount[key]  # update the amount of certificates that used the same public key
        # for a given public key
print("Count of certificates that contain the same public key: ", samePublicKeyCount)  # prints the count of
# certificates that use the same public key
