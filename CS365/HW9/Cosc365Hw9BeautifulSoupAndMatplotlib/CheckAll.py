import os
import re

titles = ['Around the World in Eighty Days', 'Pride and Prejudice', "Alice's Adventures in Wonderland", 'The Picture of Dorian Gray', "A Hermit's Wild Friends", "A Doll's House", 'The Prince', 'The Importance of Being Earnest', 'Great Expectations', 'In the Dead of Night', 'Metamorphosis', 'The Yellow Wallpaper', 'The Strange Case Of Dr. Jekyll And Mr. Hyde', "Grimms' Fairy Tales", 'The Prophet', 'A Modest Proposal', 'The King James Bible', 'Winnie-the-Pooh', 'The Wonderful Wizard of Oz', 'Leviathan', 'Peter Pan', 'The Scarlet Letter', "Gulliver's Travels", 'The War of the Worlds', 'Old Granny Fox']

for i in titles:
    file = re.sub("\s+", "", i)
    os.system('python HW9CheckerFinal-2All.py {}.txt'.format(file))