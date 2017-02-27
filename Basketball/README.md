#############################################################################################

  NBAFinalsOutcomes.C

Data taken from https://www.ticketcity.com/nba/nba-finals-tickets/nba-finals-champions.html


This project explores whether the NBA Finals outcomes are consistent with two 
equally matched teams.  The metric is the "number of games won by the loser". 

First, it calculates the "ideal" N_games_won_by_loser under the assumption that
each team had a 50% chance to win each game.  Incidentally, this "ideal" distribution 
can also be calculated analytically ! 

Second, it runs toys to determine a compatibility distribution (in this case, chi2)
when compared to the "ideal".  

Third, it calculates the actaul chi2 of the data compared to the "ideal". 

Finally, it calculates the p-value of getting the actual historical results. 

We compare the chi2 distribution to an ideal chisq distribution with the appropriate
degrees of freedom.  Moreover, the p-value is ~0.8, indicating that the NBA Finals outcomes 
(since 1947) are consistent with the teams being equally matched (ie. each team always has 
a 50% chance to win).  We should note, however, that 70 years worth of data is actually 
pretty low statistics.  In our N_games_won_by_loser distribution, there are 4 bins [0,1,2,3] 
and each bin has between 8 and 26 counts.  The uncertainties are therefore quite large. 


run macro with ROOT by: 
$ root -l NBAFinalsOutcomes.C

#############################################################################################


