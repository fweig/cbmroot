#$ -wd /hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
#$ -j y
# Number of subtasks to start
#$ -t 1-91
##$ -t 31-47

X=$((${SGE_TASK_ID} - 1))
XXX=$(printf "%03d" "$X")

#X=91
RunList='
27Nov2115 28Nov0001 28Nov0041 28Nov0126 28Nov0220 28Nov0258 28Nov0330 28Nov0404 28Nov0449 28Nov0528 28Nov0606 28Nov0645 28Nov0728 28Nov0829 28Nov0946 28Nov1026 28Nov1129 28Nov1231 28Nov1234 28Nov1256 28Nov1300 28Nov1328 28Nov1445 28Nov1527 28Nov1637 28Nov1851 28Nov1936 28Nov1957 28Nov2030 28Nov2141 28Nov2220 28Nov2322 29Nov0009 29Nov0043 29Nov0120 29Nov0206 29Nov0248 29Nov0319 29Nov0351 29Nov0425 29Nov0525 29Nov0600 29Nov0625 29Nov0626 29Nov0652 29Nov0718 29Nov0750 29Nov0821 29Nov0932 29Nov0952 29Nov1202 29Nov1257 29Nov1609 29Nov1803 29Nov1903 29Nov1958 29Nov2041 29Nov2103 30Nov0123 30Nov0234 30Nov0312 30Nov0404 30Nov0904 30Nov1027 30Nov1134 30Nov1222 30Nov1317 30Nov1355 30Nov1506 30Nov1541 30Nov1619 30Nov1637 30Nov1740 30Nov1816 30Nov1851 30Nov1933 30Nov2003 30Nov2031 30Nov2134 01Dec0045 01Dec0109 01Dec0128 01Dec0146 01Dec0206 01Dec0224 01Dec0225 01Dec0241 01Dec0258 01Dec0317 01Dec0351 01Dec0427
'

I=0
RunId=""
for RunId in $RunList
do
#echo check $RunId
if (($I == $X)); then
echo found RunId $RunId
break
fi

((I=$I+1))
done

if [[ ${Dut} = "" ]]; then
Dut="$2"
fi

if [[ ${Mref} = "" ]]; then
Mref="$3"
fi

if [[ ${Sel2} = "" ]]; then
Sel2="$4"
fi

if [[ ${InSet} = "" ]]; then
InSet="$5"
fi

echo Processing anahits_all_batch at I=$I for Run $RunId, Dut $Dut, Mref $Mref, Sel2 $Sel2, InSet $InSet

wdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15
outdir=/hera/cbm/users/nh/CBM/cbmroot/trunk/macro/tof/beamtime/nov15/hst

if((1)); then

source /hera/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 

cd  ${wdir}
source ./iter_hits.sh CbmTofSps_${RunId} ${Dut} ${Mref} ${Sel2} ${InSet}
fi

mv -v ${SGE_STDOUT_PATH} ${outdir}/anahit_${RunId}_${Dut}${Mref}${Sel2}_${InSet}.out
