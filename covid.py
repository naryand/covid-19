# list of countries, country has object containing, name, susceptible pop., infected pop., immune pop.
# simulate spread (travel btwn countries?)
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

# 
class country():
    def __init__(self, country, suscept, infect, immune):
        self.name = country
        self.suscept = suscept
        self.infect = infect
        self.immune = immune

# 96k current cases, 80k recoveries, 7k deaths worldwide
# assume the dead are immune
infect = 96
recovd = 87
populn = 7800000
# per day multiplication of cases
factor = 1.189060
# infection period in days
days = 14.451526
# simulation length in days
length = 10

# initialize world
world = country("world", populn-infect-recovd, infect, recovd)

def spread(factor, country):
    if world.suscept == 0 or world.infect == 0: # none left to infect or eradicated
        return
    
    # number of new cases with logistic growth
    infected = (factor-1)*world.infect*(1-((world.infect+world.immune)/world.suscept))
    world.suscept -= infected # subtract new cases from suscept

    if world.suscept < 0: # prevent suscept from going negative
        world.infect += world.suscept
        world.suscept = 0
        return
    
    world.infect += infected # add new cases to infected

def recover(factor, country, days):
    if world.infect == 0: # if eradicated
        return
    
    # number of recoveries
    recovered = world.infect/(factor**days) # approximation of cases that were in progress days ago
    world.infect -= recovered # subtract recovered from infected

    if world.infect < 0: # prevent infect from going negative
        world.immune += world.infect
        world.infect = 0
        return

    world.immune += recovered # add recovered to immune

# aggregate infections list
aggrlist = [None] * (length)

# simulate over the length
for i in range(1, length+1):
    spread(factor, world)
    recover(factor, world, days)
    aggrlist[i-1] = world.infect+world.immune # add cumulative to list
    # print simulation results by day
    #print("Day: {:d} Susceptible: {:.3f} Infected: {:.3f} Immune: {:.3f} Cumulative: {:.3f} ".format(i, world.suscept, world.infect, world.immune, world.infect+world.immune))

# plot simulation results
#plt.yticks(ticks=range(0,1000000,100000))
plt.grid(b=True)
plt.plot(range(1,length+1), aggrlist)
plt.savefig("aggr.png")
