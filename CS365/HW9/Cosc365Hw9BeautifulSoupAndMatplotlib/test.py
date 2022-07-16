import csv
import requests, re
from bs4 import BeautifulSoup

#myDict = {'a': -1, 'b': 2}
#myDict2 = {'a': 10, 'b': 8, 'c': 20}
hello = "9$&0“7;h\")*52,/[8e—:‘l\\5?>.”<(]l1,|0o4,!=6"
other = "by J. M. Barrie's [James Matthew Barrie]"
title = "kljhdzzflaksjdhfaskjdasjkj\rdfas das;l\rdfj asdfas dfasjdf fasdf\n asdf\n  asglakjskdfaslghafasidas*** START OF THE PROJECT GUTENBERG EBOOK PETER PAN *** as;lkhkej as;ejfasdf/a sdfasdfa sdfalsjd faskd\n alsdkj fasdfa sdfasdf as\n asdf ja           s;ldfkjsdlfgs\n sdfg sdfgsd"
end = "*** END OF THE PROJECT GUTENBERG EBOOK PETER PAN ***asfiqwejfq wfq;lwkejfl;ksdjf jjk ;lkjdgasjefas;lkdjas;ldfjk as;ldfjk"

bookHtml = re.sub("[\n\r]", "", title)
bookHtml = re.sub('.*\*\*\* START OF THE PROJECT GUTENBERG EBOOK .* \*\*\*', "", bookHtml)

#bookHtml = re.sub('(\*\*\* END OF THE PROJECT GUTENBERG EBOOK .* \*\*\*.*)$', "", end, re.DOTALL)

wordsInHtmlWithoutDash = re.sub("—", " ", other)
wordsInHtmlWithouts = re.sub("'s", "", wordsInHtmlWithoutDash)
wordsInHtml = re.split('\s+', wordsInHtmlWithouts)

for i in range(len(wordsInHtml)):
    wordsInHtml[i] = re.sub('[,"\.&\|:<>()*$?!\\\\/;=”“‘\[\]]', "", wordsInHtml[i])
    wordsInHtml[i] = re.sub('[0-9]', '', wordsInHtml[i])

print(bookHtml)

#key = "hello my name is josh"
#key = re.sub("\s+", "", key)
#print(key)


#bookHtml = "*** END OF THE PROJECT GUTENBERG EBOOK asjkdf a;sldj;laskdjf l;asdj fl;kasjd fljas d34523456 ***"
#bookHtml = re.sub('\*\*\* END OF THE PROJECT GUTENBERG EBOOK .* \*\*\*', "", bookHtml)
#print(bookHtml)
#myDict2.update(myDict)

#print(myDict2)

#for key in myDict2:
#    if key in myDict:
 #       myDict.pop(key)

#print(myDict)
