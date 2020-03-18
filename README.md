# covid-19
simulates the spread of covid-19

## covid.c
covid.c is used for regression on real data. it can be compiled with `gcc -o covid covid.c -lm` and run with `./covid`

for the purpose of simulating, covid.c is functionally identical to covid.py. 

the difference is a basic regression algorithm that simply runs simulations, continually adjusting the parameters until the results match the targets within a margin.

## covid.py
covid.py is used for simulations of covid-19. it can be run with `python3 covid.py` with python 3.5+

### methodology
the simulation assumes that those who are infected or recovered are no longer susceptible to covid-19, using a susceptible count which is equal to the world population subtracting the infected and recovered count. for the purposes of this simulation, those who have died from covid-19 are also considered immune and no longer susceptible to covid-19, and are added the recovered count.

the `spread()` function increases the infected count with logistic growth basing susceptible count as a maximum.

the `recover()` function decreases the infected count by approximating how many were infected `days` ago and adding them to the recovered count.

the result is a simulation that uses logistic growth of covid-19, while accounting for herd immunity by taking those who are immune out of the susceptible count.

### parameters of simulation 
* `infect` - the number of people currently infected in thousands

* `recovd` - the number of people who have recovered or died in thousands

* `populn` - world population in thousands 

* `factor` - the growth factor of covid-19

* `days` - average period of time a person has covid-19

* `length` - how long the simulation will run for in days

### output
the simulation results are by default plotted using pyplot with days on the x-axis, and total cases in thousands on the y-axis. this plot is saved to the file `aggr.png`

alternatively, the results can be printed day by day by uncommenting the print line in the for loop near the end

