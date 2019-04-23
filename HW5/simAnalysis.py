import csv
import math
from scipy import stats

# Number of trials
num = 50

# Which sigma model
version = "28"

# Complete filepath/filename
dir_file = version + "/"
file_nam = version + "_run_"

# Variable for confidence interval
student = stats.t.ppf(1 - 0.025, (num - 1))

grand_group_size = []
grand_total_wait = []
grand_size_wait = []
grand_num_size = []
grand_num_tour = []
grand_num_no_tour = []
grand_empty_spot = []

# Iterate through all trials
for i in range(num):
    # Open csv file
    data = open(dir_file + file_nam + str(i) + ".csv")
    
    # Set up csv reader
    csv_reader = csv.reader(data, delimiter='\t')
    
    # Variable to ignore first row
    line_count = 0

    group_enter = 0
    group_exit = 0
    num_enter = 0
    num_exit = 0
    num_empty = 0
    total_wait = 0
    size_wait = [0] * 12
    num_size = [0] * 12

    # Iterate through each customer for stats
    for row in csv_reader:
        # Final row
        if row[0][0] == 'S':
            break

        # Ignore the first row
        if line_count == 0:
            line_count += 1
            continue

        # 1 - ENTER/EXIT/LEAVE
        # 3 - Size of group entering system 
        # 4 - Size of group starting tour 
        # 5 - Wait time 
        # 6 - Number of empty tour spots
        
        if row[1] == "  ENTER":
            group_enter += 1
            num_enter += int(row[3])
        if row[1] == "   EXIT":
            group_exit += 1
            num_exit += int(row[4])
            total_wait += float(row[5])
            num_size[int(row[4])] += 1
            size_wait[int(row[4])] += float(row[5])
        if row[1] == "  LEAVE":
            num_empty += int(row[6])

    grand_group_size.append(num_enter/group_enter)
    grand_total_wait.append(total_wait/group_exit)
    for i in range(12):
        if num_size[i] == 0:
            continue
        size_wait[i] = size_wait[i]/num_size[i]
    grand_size_wait.append(size_wait)
    grand_num_tour.append(num_exit)
    grand_num_no_tour.append(num_enter - num_exit)
    grand_empty_spot.append(num_empty/10)

# Open file
file_object = open(version + "_stats.txt", "w")

avg_group_size = sum(grand_group_size)/50
file_object.write("Average group size: " + str(avg_group_size) + "\n")

var_group_size = 0
for i in grand_group_size:
    var_group_size += (i - avg_group_size)**2
var_group_size /= 49
file_object.write("Variance group size: " + str(var_group_size) + "\n")

student_delta = student*(math.sqrt(var_group_size)/math.sqrt(num))
file_object.write("CI: [" + str(avg_group_size - student_delta) + ", " + str(avg_group_size + student_delta) + "]\n\n")

avg_wait_time = sum(grand_total_wait)/50
file_object.write("Average wait time: " + str(avg_wait_time) + "\n")

var_wait_time = 0
for i in grand_total_wait:
    var_wait_time += (i - avg_wait_time)**2
var_wait_time /= 49
file_object.write("Variance wait time: " + str(var_wait_time) + "\n")

student_delta = student*(math.sqrt(var_wait_time)/math.sqrt(num))
file_object.write("CI: [" + str(avg_wait_time - student_delta) + ", " + str(avg_wait_time + student_delta) + "]\n\n")

avg_size_wait = [0] * 12
total_size = [0] * 12
for i in range(12):
    for j in grand_size_wait:
        if j[i] > 0:
            avg_size_wait[i] += j[i]
            total_size[i] += 1
    if total_size[i] != 0:
        avg_size_wait[i] /= total_size[i]
    file_object.write("Average wait time " + str(i) + ": " + str(avg_size_wait[i]) + "\n")
    var = 0
    for j in grand_size_wait:
        if j[i] != 0:
            var += (j[i] - avg_size_wait[i])**2
    if total_size[i] != 1:
        var /= (total_size[i] - 1)
    else:
        file_object.write("Only 1 sample\n\n")
        continue
    file_object.write("Variance wait time " + str(i) + ": " + str(var) + "\n")
    student_delta = student*(math.sqrt(var)/math.sqrt(num))
    file_object.write("CI: [" + str(avg_size_wait[i] - student_delta) + ", " + str(avg_size_wait[i] + student_delta) + "]\n\n")


avg_num_tour = sum(grand_num_tour)/50
file_object.write("Number who take the tour: " + str(avg_num_tour) + "\n")

var_num_tour = 0
for i in grand_num_tour:
    var_num_tour += (i - avg_num_tour)**2
var_num_tour /= 49
file_object.write("Variance number who take the tour: " + str(var_num_tour) + "\n")

student_delta = student*(math.sqrt(var_num_tour)/math.sqrt(num))
file_object.write("CI: [" + str(avg_num_tour - student_delta) + ", " + str(avg_num_tour + student_delta) + "]\n\n")

avg_num_no_tour = sum(grand_num_no_tour)/50
file_object.write("Number who can't take the tour: " + str(avg_num_no_tour) + "\n")

var_num_no_tour = 0
for i in grand_num_no_tour:
    var_num_no_tour += (i - avg_num_no_tour)**2
var_num_no_tour /= 49
file_object.write("Variance number who can't take the tour: " + str(var_num_no_tour) + "\n")

student_delta = student*(math.sqrt(var_num_no_tour)/math.sqrt(num))
file_object.write("CI: [" + str(avg_num_no_tour - student_delta) + ", " + str(avg_num_no_tour + student_delta) + "]\n\n")

avg_empty_spot = sum(grand_empty_spot)/50
file_object.write("Number of empty spots: " + str(avg_empty_spot) + "\n")

var_empty_spot = 0
for i in grand_empty_spot:
    var_empty_spot += (i - avg_empty_spot)**2
var_empty_spot /= 49
file_object.write("Variance empty tour spots: " + str(var_empty_spot) + "\n")

student_delta = student*(math.sqrt(var_empty_spot)/math.sqrt(num))
file_object.write("CI: [" + str(avg_empty_spot - student_delta) + ", " + str(avg_empty_spot + student_delta) + "]\n\n")

# Close file
file_object.close()
