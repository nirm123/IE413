import os

# Number of trial
num = 50

# File path for batch file
trial = "28"
path = trial + "/HW5.exp"

# Open file
file_object = open(path, "w")

# Print line for individual trials
for i in range(num):
    file_object.write(trial + "_run_" + str(i) + ".csv   n   " + str(i+1) + "  1000  1\n")

# Close file
file_object.close()
