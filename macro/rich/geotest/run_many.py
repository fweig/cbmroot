import os

for taskId in range(7,8):
	commandXterm = ('xterm -hold -e python3 run_one.py {}&').format(taskId)
	print(commandXterm + " " + str(taskId))
	os.system(commandXterm)
   
