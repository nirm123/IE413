import pandas as pd
import datetime
import time
import scipy.stats as ss

format = '%M:%S:%f'
sheet = pd.read_csv("Blaze_stats.csv")

col = ["Interarrival (B)", "Interarrival (I)", "Toppings", "Cooking", "Finishing"]
data = [[None for _ in range(30)] for _ in range(5)]
stat = [[None for _ in range(4)] for _ in range(5)]
count = 0
for i in col:
    cur_min = sheet[i].min()
    cur_max = sheet[i].max()
    for j in range(30):
        x = time.strptime(sheet[i][j], format)
        data[count][j] = (datetime.timedelta(hours=x.tm_hour,minutes=x.tm_min,seconds=x.tm_sec).total_seconds()/60)
        if cur_min == sheet[i][j]:
            stat[count][0] = data[count][j]
        if cur_max == sheet[i][j]:
            stat[count][1] = data[count][j]
    stat[count][2] = stat[count][1] - stat[count][0]
    count += 1

exp_set = [0, 1, 4];
for i in exp_set:
    stat[i][3] = (sum(data[i])/30)
    buc5 = 3*stat[i][3]
    buc1 = buc5/5
    buc2 = buc1 * 2
    buc3 = buc1 * 3
    buc4 = buc1 * 4
    e1 = ss.expon.cdf(buc1, 0, stat[i][3])
    e2 = ss.expon.cdf(buc2, 0, stat[i][3])
    e3 = ss.expon.cdf(buc3, 0, stat[i][3])
    e4 = ss.expon.cdf(buc4, 0, stat[i][3])
    e5 = ss.expon.cdf(buc5, 0, stat[i][3])
    e6 = ((1 - e5) * 30)
    e5 = ((e5 - e4) * 30)
    e4 = ((e4 - e3) * 30)
    e3 = ((e3 - e2) * 30)
    e2 = ((e2 - e1) * 30)
    e1 = (e1 * 30)
    c1 = 0
    c2 = 0
    c3 = 0
    c4 = 0
    c5 = 0
    c6 = 0
    X = 0
    for j in range(30):
        if data[i][j] < buc1:
            c1 += 1
        elif data[i][j] < buc2:
            c2 += 1
        elif data[i][j] < buc3:
            c3 += 1
        elif data[i][j] < buc4:
            c4 += 1
        elif data[i][j] < buc5:
            c5 += 1
        else:
            c6 += 1
    X += (c1 - e1)**2/(e1)
    X += (c2 - e2)**2/(e2)
    X += (c3 - e3)**2/(e3)
    X += (c4 - e4)**2/(e4)
    X += (c5 - e5)**2/(e5)
    X += (c6 - e6)**2/(e6)
    if i == 0:
        print("X^2 (5 dof) of Interarrival (B) being exp(" + str(stat[i][3]) + "): " + str(X))
    elif i == 4:
        print("X^2 (5 dof) of Finishing being exp(" + str(stat[i][3]) + "): " + str(X))
    else:
        print("X^2 (5 dof) of Interarrival (I) being exp(" + str(stat[i][3]) + "): " + str(X))

for i in range(2, 4):
    expected = 5
    buc1 = stat[i][0] + stat[i][2]/6
    buc2 = stat[i][0] + 2*stat[i][2]/6
    buc3 = stat[i][0] + 3*stat[i][2]/6
    buc4 = stat[i][0] + 4*stat[i][2]/6
    buc5 = stat[i][0] + 5*stat[i][2]/6
    c1 = 0
    c2 = 0
    c3 = 0
    c4 = 0
    c5 = 0
    c6 = 0
    X = 0
    for j in range(30):
        if data[i][j] < buc1:
            c1 += 1
        elif data[i][j] < buc2:
            c2 += 1
        elif data[i][j] < buc3:
            c3 += 1
        elif data[i][j] < buc4:
            c4 += 1
        elif data[i][j] < buc5:
            c5 += 1
        else:
            c6 += 1
    X += (c1-expected)**2/expected
    X += (c2-expected)**2/expected
    X += (c3-expected)**2/expected
    X += (c4-expected)**2/expected
    X += (c5-expected)**2/expected
    X += (c6-expected)**2/expected
    if i == 2:
        print("X^2 (5 dof) of Toppings being U(" + str(stat[i][0]) + ", " + str(stat[i][1]) + "): " + str(X))
    else:
        print("X^2 (5 dof) of Cooking being U(" + str(stat[i][0]) + ", " + str(stat[i][1]) + "): " + str(X))
    
