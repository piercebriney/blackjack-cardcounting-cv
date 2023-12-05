import matplotlib.pyplot as plt
import numpy
import csv


eps = []
profit = []
with open('../simulator/epstest.csv', mode='r') as file:
    csvFile = csv.reader(file)
    for line in csvFile:
        eps.append(line[0])
        profit.append(line[1])

plt.plot(eps, profit, '--bo')
plt.show()