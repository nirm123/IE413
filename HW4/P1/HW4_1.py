import pandas
import numpy as np

E = [283, 203, 146, 104, 75, 189] 

for i in range(5):
    O = [0] * 6

    file_name = 'RUN' + str(i+1) + '.csv'
    df = pandas.read_csv(file_name)
    df = df.drop(df.columns[df.columns.str.contains('unnamed',case=False)],axis=1)
    for j in range(1000):
        interval = df['Time'][j+1] - df['Time'][j]
        if interval < 1:
            O[0] += 1
        elif interval < 2:
            O[1] += 1
        elif interval < 3:
            O[2] += 1
        elif interval < 4:
            O[3] += 1
        elif interval < 5:
            O[4] += 1
        else:
            O[5] += 1

    print("RUN" + str(i+1))
    print("[0,1): " + str(O[0]))
    print("[1,2): " + str(O[1]))
    print("[2,3): " + str(O[2]))
    print("[3,4): " + str(O[3]))
    print("[4,5): " + str(O[4]))
    print("[5,INF): " + str(O[5]))

    summation = 0
    for j in range(6):
        temp = float((O[j] - E[j])**2)/E[j]
        summation += temp
    print("X^2: " + str(summation) + "\n")
 

