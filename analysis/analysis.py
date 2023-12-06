import matplotlib.pyplot as plt
import numpy as np
import csv
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter


def showHeight(height, interp):
    fig, ax = plt.subplots()
    angle = []
    profit = []
    X = []
    Y = []
    with open('../simulator/results/angles_at_height_' + height +'.csv', mode='r') as file:
        csvFile = csv.reader(file)
        for line in csvFile:
            an = float(line[0])
            p = float(line[1])
            if p > 0 and profit[-1] < 0:
                X.append(profit[-1])
                X.append(p)
                Y.append(angle[-1])
                Y.append(an)
            angle.append(an)
            profit.append(p)
    if interp:
        x = [np.interp(0, X, Y)]
        y = [0]
        ax.plot(x, y, marker = 'o')
        ax.annotate('{0:.2f} Degrees'.format(x[0]), xy = (x[0], y[0]), xytext=(5,5), xycoords='data', textcoords='offset points')
        
    
    ax.plot(angle, profit, linestyle='--', marker='x', color='b')
    ax.set_xlabel("Angle (degrees)")
    ax.set_ylabel("Average Profit over 10000 trials ($)")
    ax.set_title("Average Profit at Distance " + height + " ft")
    ax.axhline(y=0, color='k')

def plotSurface():
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    H = ["1.0", "1.5", "2.0", "2.5", "3.0", "3.5", "4.0", "4.5", "5.0"]
    angle = []
    height = []
    profit = []
    for h in H:
        with open('../simulator/results/angles_at_height_' + h +'.csv', mode='r') as file:
            csvFile = csv.reader(file)
            for line in csvFile:
                height.append(float(h))
                angle.append(float(line[0]))
                profit.append(float(line[1]))

    surf = ax.plot_trisurf(np.array(angle), np.array(height), np.array(profit), cmap=cm.coolwarm, linewidth=0, antialiased=False)
    ax.set_xlabel('Angle (degrees)')
    ax.set_zlabel('Average Profit over 10000 trials ($)')
    ax.set_ylabel('Distance (feet)')
    ax.set_title('Average Profit')
    ax.set_ylim(1,5)

    # Add a color bar which maps values to colors.
    fig.colorbar(surf, shrink=0.5, aspect=5)

# plotSurface()
plotSurface()
plt.show()

