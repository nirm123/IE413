import pandas
import numpy as np

total_customers = 0
five_customers = 0
twenty_customers = 0
total_wait_time = 0
five_wait_time = 0
twenty_wait_time = 0

for i in range(5):
    file_name = 'HW3_NO3_RUN' + str(i+1) + '.csv'
    df = pandas.read_csv(file_name)
    df = df.drop(df.index[len(df)-1])
    df = df.drop(df.columns[df.columns.str.contains('unnamed',case=False)],axis=1)

    num_customers = int(df['IN'].max())
    total_customers += num_customers

    wait_time = np.zeros(num_customers)
    service_time = np.zeros(num_customers)
    
    i = 0
    for index, row in df.iterrows():
        if row['Event'] == 'END':
            wait_time[i] = row['W[IN]']
            service_time[i] = row['S[IN]']
            i += 1
    
    for i in range(num_customers):
        if int(service_time[i]) != 5 and int(service_time[i]) != 20:
            if (wait_time[i] > 20):
                service_time[i] = 20
            else:
                service_time[i] = 5

        if int(service_time[i]) == 5:
            five_wait_time += wait_time[i]
            five_customers += 1
        else:
            twenty_wait_time += wait_time[i]
            twenty_customers += 1

    total_wait_time += np.sum(wait_time)

print('Average Number of Customers: ' + str(total_customers/5))
print('Average Wait Time: ' + str(total_wait_time/total_customers))
print('Average Wait Time (Short): ' + str(five_wait_time/five_customers))
print('Average Wait Time (Long): ' + str(twenty_wait_time/twenty_customers))

