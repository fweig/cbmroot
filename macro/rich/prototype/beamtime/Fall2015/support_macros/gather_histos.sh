#!/bin/bash

OUTPUT_FOLDER=output_save/

# Block 3 - WLS on 100

PREFIX="/store/cbm_rich_data_2014/WLS/WLS_on/nonstretched/"
SUFFIX="offset00100/"
FOLDERS[1]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringH2/"$SUFFIX

command="hadd res_on_100.root "

for i in {1..3}
do
	for IN_FILE in ${FOLDERS[$i]}*
	do
		FILENAME=`basename ${IN_FILE}`
		HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
		command+=$HISTO2_FILE" "
	done
done

#echo $command
$command
# here execute command

# Block 4 - WLS on 250

PREFIX="/store/cbm_rich_data_2014/WLS/WLS_on/nonstretched/"
SUFFIX="offset00250/"
FOLDERS[1]=$PREFIX"ringD1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringDG1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[4]=$PREFIX"ringGH1/"$SUFFIX
FOLDERS[5]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[6]=$PREFIX"ringH2/"$SUFFIX

command="hadd res_on_250.root "

for i in {1..6}
do
	for IN_FILE in ${FOLDERS[$i]}*
	do
		FILENAME=`basename ${IN_FILE}`
		HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
		command+=$HISTO2_FILE" "
	done
done

#echo $command
$command
# here execute command

# Block 1 - WLS off 100

PREFIX="/store/cbm_rich_data_2014/WLS/WLS_off/nonstretched/"
SUFFIX="offset00100/"
FOLDERS[1]=$PREFIX"ringD1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringDG1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringDG2/"$SUFFIX
FOLDERS[4]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[5]=$PREFIX"ringGH1/"$SUFFIX
FOLDERS[6]=$PREFIX"ringGH2/"$SUFFIX
FOLDERS[7]=$PREFIX"ringGH_redo/"$SUFFIX
FOLDERS[8]=$PREFIX"ringG_redo/"$SUFFIX
FOLDERS[9]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[10]=$PREFIX"ringH2/"$SUFFIX
FOLDERS[11]=$PREFIX"ringH_redo/"$SUFFIX

command="hadd res_off_100.root "

for i in {1..11}
do
	for IN_FILE in ${FOLDERS[$i]}*
	do
		FILENAME=`basename ${IN_FILE}`
		HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
		command+=$HISTO2_FILE" "
	done
done

#echo $command
$command
# here execute command

#exit

# Block 2 - WLS off 250

PREFIX="/store/cbm_rich_data_2014/WLS/WLS_off/nonstretched/"
SUFFIX="offset00250/"
FOLDERS[1]=$PREFIX"ringD1/"$SUFFIX
FOLDERS[2]=$PREFIX"ringDG1/"$SUFFIX
FOLDERS[3]=$PREFIX"ringG1/"$SUFFIX
FOLDERS[4]=$PREFIX"ringGH1/"$SUFFIX
FOLDERS[5]=$PREFIX"ringH1/"$SUFFIX
FOLDERS[6]=$PREFIX"ringH2/"$SUFFIX

command="hadd res_off_250.root "

for i in {1..6}
do
	for IN_FILE in ${FOLDERS[$i]}*
	do
		FILENAME=`basename ${IN_FILE}`
		HISTO2_FILE=${OUTPUT_FOLDER}histo2_${FILENAME}.root
		command+=$HISTO2_FILE" "
	done
done

#echo $command
$command
# here execute command
