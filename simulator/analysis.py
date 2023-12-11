import matplotlib.pyplot as plt
import numpy as np
import csv
from matplotlib import cm
import ruptures as rpt
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from pathlib import Path
from collections import defaultdict
from subprocess import run

def getProfits(distance, angle):
    lines = Path('matrix/' + distance + "_" + angle +".txt.output").read_text().splitlines()
    profits = np.array([float(i) for i in lines])
    return profits.mean(), profits

def showHeight(height, interp):
    fig, ax = plt.subplots()
    angle = []
    profit = []
    X = []
    Y = []
    for a in range(15, 90, 3):
        if a <= 45 or a % 15 == 0:
            p, ps = getProfits(height, str(a))
            if p > 0 and profit[-1] < 0:
                X.append(profit[-1])
                X.append(p)
                Y.append(angle[-1])
                Y.append(a)
            angle.append(a)
            profit.append(p)
    angle = np.array(angle)
    profit = np.array(profit)
    if interp:
        x = [np.interp(0, X, Y)]
        y = [0]
        ax.plot(x, y, marker = 'x')
        ax.annotate('{0:.2f} Degrees'.format(x[0]), xy = (x[0], y[0]), xytext=(5,-10), xycoords='data', textcoords='offset points')
    ax.plot(angle, profit, linestyle='--', marker='o', color='b')
    ax.set_xlabel("Angle (degrees)")
    ax.set_ylabel("Average Profit over 10000 trials ($)")
    ax.set_title("Average Profit at Distance " + height + " ft")
    ax.axhline(y=0, color='k')

def showAngle(angle):
    fig, ax = plt.subplots()
    dist = np.linspace(1, 5, 9)
    profit = []
    for d in dist:
        p, ps = getProfits("{:.1f}".format(d), angle)
        profit.append(p)
    dist = np.array(dist)
    profit = np.array(profit)
    ax.plot(dist, profit, linestyle='--', marker='o', color='b')
    ax.set_xlabel("Distance (ft)")
    ax.set_ylabel("Average Profit over 10000 trials ($)")
    ax.set_title("Average Profit at Angle " + angle + "Degrees")
    ax.axhline(y=0, color='k')


def plotSurface():
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    H = np.arange(1.0, 5.0, 0.5)
    angle = []
    height = []
    profit = []
    for h in H:
        for a in range(15, 90, 3):
            if a <= 45 or a % 15 == 0:
                height.append(h)
                angle.append(a)
                p, ps = getProfits("{:.1f}".format(h), str(a))
                profit.append(p)
    surf = ax.plot_trisurf(np.array(angle), np.array(height), np.array(profit), cmap=cm.coolwarm, linewidth=0, antialiased=False)
    ax.set_xlabel('Angle (degrees)')
    ax.set_zlabel('Average Profit over 10000 trials ($)')
    ax.set_ylabel('Distance (feet)')
    ax.set_title('Average Profit')
    ax.set_ylim(1,5)
    fig.colorbar(surf, shrink=0.5, aspect=5)

def showFreq(distance, angle):
    p, xs = getProfits(distance, angle)
    mu = xs.mean()
    std = xs.std()
    upper = mu + std
    lower = mu - std
    s = 0
    s = sum(1 for x in xs if x > 0)
    print("prob", s / len(xs))
    _, _, bars = plt.hist(xs, bins=500)
    for bar in bars:
        color = "palegreen" if bar.get_x() > 0 else "lightcoral"
        bar.set_facecolor(color)

    plt.xlabel("Profit ($)")
    plt.ylabel("Frequency")
    plt.title("Card Counting for 800 Rounds over 10K Trials")
    plt.axvline(xs.mean(), color='k', linestyle='dashed', linewidth=1)
    min_ylim, max_ylim = plt.ylim()
    # plt.axvline(upper, color='b', linestyle='dashed', linewidth=1)
    # plt.axvline(lower, color='b', linestyle='dashed', linewidth=1)
    plt.text(mu * 1.1, max_ylim * 0.9, "Mean: {mu:.2f}")
        
def showAllHeights():
    d = defaultdict(list)
    lines = Path("build/out.txt").read_text().splitlines()
    for i in range(0, len(lines), 5):
        name = lines[i].split()[1].split("/")[-1]
        distance, angle = name.split("_")
        distance = float(distance)
        angle = int(angle[:-4])
        mean = float(lines[i + 1].split()[1])
        stdev = float(lines[i + 2].split()[2])
        d[distance].append((angle, mean))

    for k, v in d.items():
        angles, means = list(zip(*v))
        angles = list(angles)
        means = list(means)
        plt.plot(angles, means, linestyle='--', marker='o', label=f"{k} ft.")
    plt.xlabel("Angle (degrees)")
    plt.ylabel("Profit ($)")
    plt.title("Profit vs. Angle over 10K Trials")
    plt.legend()

# for i in $(seq 0 0.1 1.0); do ./simulator ../matrix/1.0_15.txt --perfectness $i; done | grep Mean | cut -d' ' -f2- > eps.test
def epsilonGraph(dist, angle, n):
    mat = "matrix/{d:.1f}_{a}.txt".format(d = dist, a = angle)
    xs = np.linspace(0, 1, num=n)
    ys = []
    # lines = Path("build/eps.test").read_text().splitlines()
    # ys = [float(i) for i in lines]
    for x in xs:
        cmd = f"./build/simulator {mat} --perfectness {x}"
        out = run(cmd.split(), capture_output=True)
        lines = out.stdout.decode().splitlines()
        mean = float(next(line for line in lines if "Mean" in line).split()[1])
        ys.append(mean)
    plt.plot(xs, ys, linestyle='--', marker='o')
    plt.xlabel("Interpolation Value")
    plt.ylabel("Profit ($)")
    plt.title("Profit vs. Interpolation Value for 10K Trials at {d:.1f} ft. and {a} Degrees".format(d = dist, a = angle))

def showAllAngles():
    d = defaultdict(list)
    lines = Path("build/out.txt").read_text().splitlines()
    for i in range(0, len(lines), 5):
        name = lines[i].split()[1].split("/")[-1]
        distance, angle = name.split("_")
        distance = float(distance)
        angle = int(angle[:-4])
        mean = float(lines[i + 1].split()[1])
        stdev = float(lines[i + 2].split()[2])
        d[angle].append((distance, mean))

    for k, v in d.items():
        distances, means = list(zip(*v))
        distances = list(distances)
        means = list(means)
        plt.plot(distances, means, linestyle='--', marker='o', label=f"{k} Degrees")
    plt.xlabel("Distance (feet)")
    plt.ylabel("Profit ($)")
    plt.title("Profit vs. Distance over 10K Trials")
    plt.legend()
    plt.show()
# plotSurface()
# print(getProfits("1.0", "15"))
showHeight("1.0", True)
# showAngle("90", True)
# epsilonGraph(1.0, 90, 10)
# showAngle()
# showAllHeights()
plt.show()