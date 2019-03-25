import pandas
import numpy as np

E = [283, 203, 146, 104, 75, 189] 

for i in range(5):
    O = [0] * 6

    file_name = 'RUN' + str(i+1) + '.csv'
    df = pandas.read_csv(file_name)
    df = df.drop(df.columns[df.columns.str.contains('unnamed',case=False)],axis=1)
    mean = 0
    
    for j in range(1000):
        interval = df['Time'][j+1] - df['Time'][j]
        mean += float(interval)
    mean = float(mean)/1000

    var = 0
    for j in range(1000):
        interval = df['Time'][j+1] - df['Time'][j]
        var += float((interval - mean)**2)
    var = float(var)/999

    print("RUN" + str(i + 1))
    print("Mean: " + str(mean))
    print("Variance: " + str(var) + "\n")
         

