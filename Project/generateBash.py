import os

trial = "trial3"

file_object = open(trial + ".ebp", "w")
for i in range(1000):
    file_object.write(trial + "_run" + str(i) + ".xls   n   " + str(i) + "  240  1\n")

file_object.close()
