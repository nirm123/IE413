import csv
import math
from scipy import stats

# Number of trials
num = 1000

# Which sigma model
version = "3_1_Sim"

# Complete filepath/filename
dir_file = "../" + str(version) + "/" + str(num) + "/"
file_nam = version + "_run_"

# Alpha/Mean for variance reduction with control variates 
alpha = -4.4
alpha2 = -5.2
service = 1.68333333334

# Averages of sojourn time for each trial
grand_average = []

# Averages of sojourn time for each trial
grand_wait = []

# Ratio of customers who wait more than 15 minutes
grand_cust15 = []

# Ratio of time queue 0 is longer than 4 for each trial
grand_ratio4 = []

# Ratio of time queue 0 is longet than 8 for each trial
grand_ratio8 = []

# Calculated variance for average soujourn time and ratios 
average_var = 0
ratio_var4 = 0
ratio_var8 = 0
cust_var = 0
wait_var = 0

# Variable for confidence interval
student = stats.t.ppf(1 - 0.025, (num - 1))

# Iterate through all trials
for i in range(num):
    # Variable for finishing time of trial
    max_time = 0

    # Accumulator for average
    average = 0

    # Accumulator for customers waiting more than 15
    cust = 0
    
    # Variable for ratio 4
    ratio4 = 0

    # Variable for ratio 8
    ratio8 = 0

    # Open csv file
    data = open(dir_file + file_nam + str(i) + ".csv")
    
    # Set up csv reader
    csv_reader = csv.reader(data, delimiter='\t')
    
    # Variable to ignor first row
    line_count = 0

    # Variable to keep count of the number of customers
    count = 0

    # Variable to keep coun of wait time
    wait = 0

    # Iterate through each customer for stats
    for row in csv_reader:
        # Final row
        if row[0][0] == 'S':
            break

        # Ignore the first row
        if line_count == 0:
            line_count += 1
            continue

        # Ignore customers with sojourn time longer than 35 (error in simulation)
        if float(row[4]) > 40:
            continue

        # Running sum for average sojourn
        average += float(row[4]) + alpha * (float(row[3]) - service)

        # Running sum for average wait
        wait += float(row[7]) + alpha2 * (float(row[3]) - service)
        
        # Update max_time
        max_time = float(row[0])
        
        # Update ratio8 time
        ratio8 = float(row[5])
        
        # Update ratio4 time
        ratio4 = float(row[6])

        # Keep count of number of customers
        count += 1

        # Keep count of customers spending more than 15 minutes in system
        if float(row[4]) > 15:
            cust += 1

    # Calculate average sojourn
    average /= count
    
    # Calculate average wait
    wait /= count

    # Calculate average customers
    cust /= count
    
    # Append average, ratio4, cust15, wait, and ratio8
    grand_average.append(average)
    grand_ratio4.append(ratio4/max_time)
    grand_ratio8.append(ratio8/max_time)
    grand_cust15.append(cust)
    grand_wait.append(wait)

# Open file
file_object = open("../" + version + "/" + str(num) + "_stats.txt", "w")

# Calculated average sojourn time
calculated_average = (sum(grand_average)/num)
file_object.write("Average sojourn time of customer:\n")
file_object.write("AVG: " + str(calculated_average) + "\n")

# Calculate variance of sojourn time
for i in grand_average:
    average_var += (i - calculated_average)**2
average_var /= (num - 1)
file_object.write("VAR: " + str(average_var) + "\n")

# CI for sojourn
student_delta = student*(math.sqrt(average_var)/math.sqrt(num))
file_object.write("CI: [" + str(calculated_average - student_delta) + ", " + str(calculated_average + student_delta) + "]\n\n")

# Calculated average number of customers spending more than 15 minutes in a system
calculated_15 = (sum(grand_cust15)/num)
file_object.write("Percentage of customers spending more than 15 minutes in the system:\n")
file_object.write("AVG: " + str(calculated_15) + "\n")

# Calculate variance for customer spending more than 15 minutes in the system
for i in grand_cust15:
    cust_var += (i - calculated_15)**2
cust_var /= (num - 1)
file_object.write("VAR: " + str(cust_var) + "\n")

# CI for customers spending more than 15 minutes
student_delta = student*(math.sqrt(cust_var)/math.sqrt(num))
file_object.write("CI: [" + str(calculated_15 - student_delta) + ", " + str(calculated_15 + student_delta) + "]\n\n")

# Calculated ratio of time with line greater than 4
calculated_ratio4 = sum(grand_ratio4)/num
file_object.write("Percentage of time the initial queue is longer than 4:\n")
file_object.write("AVG: " + str(calculated_ratio4) + "\n")

# Calculate variance of time with line greater than 4
for i in grand_ratio4:
    ratio_var4 += (i - calculated_ratio4)**2
ratio_var4 /= (num - 1)
file_object.write("VAR: " + str(ratio_var4) + "\n")

# CI for time line greater than 4
student_delta = student*(math.sqrt(ratio_var4)/math.sqrt(num))
file_object.write("CI: [" + str(calculated_ratio4 - student_delta) + ", " + str(calculated_ratio4 + student_delta) + "]\n\n")

# Calculated ratio of time with line greater than 8
calculated_ratio8 = sum(grand_ratio8)/num
file_object.write("Percentage of time the initial queue is longer than 8:\n")
file_object.write("AVG: " + str(calculated_ratio8) + "\n")

# Calculate variance of time with line greater than 8
for i in grand_ratio8:
    ratio_var8 += (i - calculated_ratio8)**2
ratio_var8 /= (num - 1)
file_object.write("VAR: " + str(ratio_var8) + "\n")

# CI for time line greater than 8
student_delta = student*(math.sqrt(ratio_var8)/math.sqrt(num))
file_object.write("CI: [" + str(calculated_ratio8 - student_delta) + ", " + str(calculated_ratio8 + student_delta) + "]\n\n")

# Calculated average sojourn time
calculated_wait = (sum(grand_wait)/num)
file_object.write("Average wait time of customer:\n")
file_object.write("AVG: " + str(calculated_wait) + "\n")

# Calculate variance of sojourn time
for i in grand_wait:
    wait_var += (i - calculated_wait)**2
wait_var /= (num - 1)
file_object.write("VAR: " + str(wait_var) + "\n")

# CI for sojourn
student_delta = student*(math.sqrt(wait_var)/math.sqrt(num))
file_object.write("CI: [" + str(calculated_wait - student_delta) + ", " + str(calculated_wait + student_delta) + "]")

# Close file
file_object.close()
