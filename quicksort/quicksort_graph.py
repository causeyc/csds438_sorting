# -*- coding: utf-8 -*-
"""
Created on Sun Nov 15 02:34:50 2020

@author: Jason Richards (jdr145)
"""
import matplotlib.pyplot as plt
import csv

threads_1 = []
threads_2 = []
threads_4 = []
threads_8 = []
threads_16 = []
x = []

with open('project.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        try:
            a = float(row[0])
        except ValueError:
            continue
        if float(row[0]) == 1:
            threads_1.append(float(row[2]))
        elif float(row[0]) == 2:
            threads_2.append(float(row[2]))
        elif float(row[0]) == 4:
            threads_4.append(float(row[2]))
        elif float(row[0]) == 8:
            threads_8.append(float(row[2]))
        elif float(row[0]) == 16:
            threads_16.append(float(row[2]))
        if float(row[1]) not in x:
            x.append(float(row[1]))

plt.plot(x, threads_1, label='1 thread', marker='o')
plt.plot(x, threads_2, label="2 threads", marker='o')
plt.plot(x, threads_4, label="4 threads", marker='o')
plt.plot(x, threads_8, label="8 threads", marker='o')
plt.plot(x, threads_16, label="16 threads", marker='o')
plt.xlabel('number elements')
plt.ylabel('time (s)')
plt.title('Parellel Quicksort using OpenMP')
plt.legend()
plt.show()