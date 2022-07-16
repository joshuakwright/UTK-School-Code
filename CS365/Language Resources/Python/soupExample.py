from bs4 import BeautifulSoup
import matplotlib.pyplot as plt
import pylab ,matplotlib.ticker, requests,re, operator
from collections import Counter

r = requests.get("https://www.gutenberg.org/files/47960/47960-h/47960-h.htm")
soup = BeautifulSoup(r.text, 'html.parser')

lowerText = soup.get_text().lower()
wordsInText = re.split('\s+',lowerText)

for i in range(len(wordsInText)):
	wordsInText[i] = re.sub('[,".&|:@,<>()*$?!\\/;=]', "", wordsInText[i])
	wordsInText[i] = re.sub('^[0-9\.]*$', "", wordsInText[i])
wordDictionary = Counter(wordsInText)

stopwords = ['ut', '\'re','.', ',', '--', '\'s','cf', '?', ')', '(', ':',
 '\'','\"', '-', '}', '{', '&', '|', u'\u2014', '' ]
for stopword in stopwords:
 	wordDictionary.pop(stopword,None)
sortedWordFreqency = sorted(wordDictionary.items(), key = operator.itemgetter(1), reverse=True)
top25Words = sortedWordFreqency[0:25]

fig, ax = plt.subplots() 
ax.barh([ x [0] for x in top25Words ],[ x [1] for x in top25Words ])
plt.show()


