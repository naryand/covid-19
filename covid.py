# list of countries, country has object containing, name, susceptible pop., infected pop., immune pop.
# simulate spread (travel btwn countries?)
import math
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

# 
class country():
    def __init__(self, country, suscept, infect, immune):
        self.name = country
        self.suscept = suscept
        self.infect = infect
        self.immune = immune

# 85k current cases, 83.5k recoveries, 6.5k deaths countrywide as of march 15
# assume the dead are immune
infect = 2
recovd = 0
populn = 7800000
# per day multiplication of cases
factor = 1.10999
# infection period in days
days = 43
# simulation length in days
length = 66

# initialize country
world = country("world", populn-infect-recovd, infect, recovd)

fortnight = [-1] * round(days)

def spread(factor, country, days, list):
    if country.suscept == 0 or country.infect == 0: # none left to infect or eradicated
        return
    
    # number of new cases with logistic growth
    infected = (factor-1)*country.infect*(1-((country.infect+country.immune)/country.suscept))
    country.suscept -= infected # subtract new cases from suscept

    if country.suscept < 0: # prevent suscept from going negative
        country.infect += country.suscept
        country.suscept = 0
        return
    
    country.infect += infected # add new cases to infected

    for i in range(1, round(days)):
        list[round(days)-i] = list[round(days)-i-1]
        list[0] = country.infect

def recover(factor, country, days, list):
    if country.infect == 0: # if eradicated
        return
    
    # number of recoveries
    if list[round(days)-1] == -1:
        recovered = country.infect/(math.pow(factor, days)) # approximation of cases that were in progress days ago # subtract recovered from infected
    else: 
        recovered = list[round(days)-1]
    
    country.infect -= recovered

    if country.infect < 0: # prevent infect from going negative
        country.immune += country.infect
        country.infect = 0
        return

    country.immune += recovered # add recovered to immune

# aggregate infections list
aggrlist = [None] * (length)

# simulate over the length
for i in range(1, length+1):
    spread(factor, world, days, fortnight)
    recover(factor, world, days, fortnight)
    aggrlist[i-1] = world.infect+world.immune # add cumulative to list
    # print simulation results by day
    print("Day: {:d} Susceptible: {:.3f} Infected: {:.3f} Immune: {:.3f} Cumulative: {:.3f} ".format(i, world.suscept, world.infect, world.immune, world.infect+world.immune))

# plot simulation results
#plt.yticks(ticks=range(0,1000000,100000))
plt.grid(b=True)
plt.plot(range(1,length+1), aggrlist)
plt.savefig("aggr.png")
