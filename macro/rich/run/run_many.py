import os

for taskId in range(1,4):
	commandXterm = ('xterm -hold -e python3 run_one.py {}&').format(taskId)
	os.system(commandXterm)
   
