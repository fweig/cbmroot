import os


commandXterm1 = ('xterm -hold -e python3 run_one.py {} {}&').format(1, "4gev")
os.system(commandXterm1)
   
commandXterm2 = ('xterm -hold -e python3 run_one.py {} {}&').format(2, "8gev")
os.system(commandXterm2)

commandXterm3 = ('xterm -hold -e python3 run_one.py {} {}&').format(3, "10gev")
os.system(commandXterm3)

commandXterm4 = ('xterm -hold -e python3 run_one.py {} {}&').format(4, "12gev")
os.system(commandXterm4)
