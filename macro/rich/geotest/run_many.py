import os


commandXterm1 = ('xterm -hold -e python3 run_one.py {} {}&').format(1, "urqmdtest")
os.system(commandXterm1)

commandXterm11 = ('xterm -hold -e python3 run_one.py {} {}&').format(1, "geotest")
os.system(commandXterm11)

commandXterm1 = ('xterm -hold -e python3 run_one.py {} {}&').format(2, "urqmdtest")
os.system(commandXterm1)

commandXterm11 = ('xterm -hold -e python3 run_one.py {} {}&').format(2, "geotest")
os.system(commandXterm11)

commandXterm1 = ('xterm -hold -e python3 run_one.py {} {}&').format(3, "urqmdtest")
os.system(commandXterm1)

commandXterm11 = ('xterm -hold -e python3 run_one.py {} {}&').format(3, "geotest")
os.system(commandXterm11)

