# #!
# run simulation and tracks reconstruction

MAINDIR=`pwd`

if [ -z $1 ]
then
  NEVENTS=1000
else
  NEVENTS=$1
fi

if [ -z $2 ]
then
  WORKDIR=$MAINDIR"/data"
else
  WORKDIR=$2
fi
cd $WORKDIR

rm -rf *

NNN=35
TASKS=1
ONE=1
NOL=0

PID=""
for i in {0..125}
do
  CURDIR="Signal"$i
  mkdir $CURDIR
  cd $CURDIR
  echo `pwd`

  cp -rf $MAINDIR/Signal/* .
  
  bash runsim.sh $i $NEVENTS >& runsim.log &
  PID=$PID" "$!
  
  sleep 0.2
  
  if [ $TASKS -eq $NNN ]
  then
    wait $PID
    PID=""
    TASKS=0
  fi
  
  TASKS=`expr $TASKS + $ONE`
  
  cd ../
done
wait $PID

cd $MAINDIR

echo -e "\007"

