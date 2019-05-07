# restaurant
restaurant simulation in c++

https://www.cs.bgu.ac.il/~spl191/wiki.files/Assignment%20Specifications.pdf

config1 text:
#number of tables
6
#tables description
6,4,4,14,10,10
#Menu
Beer,ALC,50
Salad,VEG,40
Water,BVG,10
Wine,ALC,60
Chili Con Carne,SPC,200

Below is a running example, according to the configuration file “config1.txt”. Program

output is marked in yellow.
Restaurant is now open!
open 1 Alice,chp Bob,alc
order 1
Alice ordered Water
Bob ordered Beer
order 1
Bob ordered Wine
open 2 Idan,veg Elad,alc
order 2
Idan ordered Salad
Idan ordered Water
Elad ordered Beer
order 2
Idan ordered Salad
Idan ordered Water
Elad ordered Wine
move 1 2 1
status 1
Table 1 status: open
Customers:
0 Alice
Orders:
Water 10NIS 0
Current Bill: 10NIS
status 2
Table 2 status: open
Customers:
2 Idan
3 Elad
1 Bob
Orders:
Salad 40NIS 2
Water 10NIS 2
Beer 50NIS 3
Salad 40NIS 2
Water 10NIS 2
Wine 60NIS 3
Beer 50NIS 1
Wine 60NIS 1
Current Bill: 320NIS
open 2 Jon,spc Sam,spc
Error: Table does not exist or is already open
log
open 1 Alice,chp Bob,alc Completed
order 1 Completed
order 1 Completed
open 2 Idan,veg Elad,alc Completed
order 2 Completed
order 2 Completed
move 1 2 1 Completed
status 1 Completed
status 2 Completed
open 2 Jon,spc Sam,spc Error: Table does not exist or is already open
closeall
Table 1 was closed. Bill 10NIS
Table 2 was closed. Bill 320NIS
