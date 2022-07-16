#Joshua Wright jwrig117

import csv
import requests, re, operator
from bs4 import BeautifulSoup
from collections import Counter
import matplotlib.pyplot as plt

masterDict = {}
allBooks = {}

with open("sitesToScrap.csv") as booksFile:
    links = csv.reader(booksFile)
    for row in links:
        contents = requests.get(row[0])
        html = BeautifulSoup(contents.text, 'html.parser')
        book = re.sub('[\n\r]', " ", html.get_text())
        bookHtml = re.sub('^(.*?)\*\*\* START OF THE PROJECT GUTENBERG EBOOK .*? \*\*\*', "", book, re.DOTALL)
        bookHtml = re.sub('(\*\*\* END OF THE PROJECT GUTENBERG EBOOK .*? \*\*\*)(.*?)$', "", bookHtml)
        lowerCaseHtml = bookHtml.lower()
        wordsInHtmlWithoutDash = re.sub("—", " ", lowerCaseHtml)
        wordsInHtml = re.split('\s+', wordsInHtmlWithoutDash)

        for i in range(len(wordsInHtml)):
            wordsInHtml[i] = re.sub('[,"\.&\|:<>()*$?!\\\\/;=”“‘\[\]]', "", wordsInHtml[i])
            wordsInHtml[i] = re.sub('^[0-9\.]*$', "", wordsInHtml[i])
        wordToCount = Counter(wordsInHtml)

        stopwords = ['ut', '\'re', '.', ',', '--', '\'s', 'cf', '?', ')', '(', ':', '\'', '\"', '-', '}', 'â', '£', '{',
                     '&', '|', u'\u2014', '', ']']
        for stopword in stopwords:
            wordToCount.pop(stopword, None)

        allBooks[row[1]] = wordToCount

        for key in wordToCount.keys():
            if key in masterDict.keys():
                masterDict[key] += 1
            else:
                masterDict[key] = 1
booksFile.close()

masterNonUnique = [key for key in masterDict if masterDict[key] != 1]
for key in masterNonUnique: del masterDict[key]

ThePrinceOriginal = sorted(allBooks["The Prince"].items(), key=operator.itemgetter(1), reverse=True)[0:25]
ThePrinceUnique = None

for key in allBooks.keys():
    nonUnique = [bookKey for bookKey in allBooks[key].keys() if bookKey not in masterDict.keys()]
    for nonUniqueKey in nonUnique: del allBooks[key][nonUniqueKey]
    top25Sorted = sorted(allBooks[key].items(), key = operator.itemgetter(1), reverse=True)[0:25]
    if (key == "The Prince"):
        ThePrinceUnique = top25Sorted
    bookFileName = re.sub("\s+", "", key) + ".txt"
    bookFile = open(bookFileName, "w")
    for element in top25Sorted:
        print(element, file=bookFile)
    bookFile.close()

fig, axis = plt.subplots(1, 2)
axis[0].barh([ x [0] for x in ThePrinceOriginal ],[ x [1] for x in ThePrinceOriginal ], color='r')
axis[0].set_xlabel("Frequency")
axis[0].set_ylabel("Words")
axis[0].set_title("The Prince Top 25 Words")

axis[1].barh([ x [0] for x in ThePrinceUnique ],[ x [1] for x in ThePrinceUnique ], color='b')
axis[1].set_xlabel("Frequency")
axis[1].set_ylabel("Words")
axis[1].set_title("The Prince Top 25 Unique Words")
plt.show()