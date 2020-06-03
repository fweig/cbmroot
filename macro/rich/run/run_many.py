import os

for taskId in range(1,5):
	commandXterm = ('xterm -hold -e python3 run_one.py {}&').format(taskId)
	print(commandXterm + " " + str(taskId))
	os.system(commandXterm)
   
