#!/bin/sh

MAINDIR=`pwd`
rm -rf KFParticleFinder.root

if [ -z $1 ]
then
  WORKDIR=$MAINDIR"/data"
else
  WORKDIR=$1
fi

START=0

NDIRS=125
for (( i=$START; i<=$NDIRS; i++ ))
do
  TMPHISTOSNAMES[i]=""
  TMPHISTOSNAMES2[i]=""
  TMPFILENAME[i]=""
  TMPFILENAME2[i]=""
done

ONE=1
NTMPFILES=0
NDIRSINTMPFILE=0
MAXDIRSINTMPFILE=7

for (( i=$START; i<=$NDIRS; i++ ))
do
  NEWFILE=$WORKDIR"/Signal"$i"/KFParticleFinder.root"
  if [ -f $NEWFILE ]; then

    TMPHISTOSNAMES[NTMPFILES]=${TMPHISTOSNAMES[NTMPFILES]}" "$NEWFILE
    
    NDIRSINTMPFILE=`expr $NDIRSINTMPFILE + $ONE`
    if [ $NDIRSINTMPFILE -eq $MAXDIRSINTMPFILE ]
    then
      if [ $i -ne $NDIRS ]
      then
        echo ${TMPHISTOSNAMES[NTMPFILES]}

        NTMPFILES=`expr $NTMPFILES + $ONE`
        NDIRSINTMPFILE=0
      fi
    fi
  fi
done

PID=""
for (( i=$START; i<=$NTMPFILES; i++ ))
do
  TMPFILENAME[i]="KFParticleFinderTmp"$i".root"
  hadd ${TMPFILENAME[i]} ${TMPHISTOSNAMES[i]} &> collog$i.log &
  PID=$PID" "$!
done
wait $PID


NTMPFILES2=0
NDIRSINTMPFILE2=0
for (( i=$START; i<=$NTMPFILES; i++ ))
do
  TMPHISTOSNAMES2[NTMPFILES2]=${TMPHISTOSNAMES2[NTMPFILES2]}" "${TMPFILENAME[i]}
  
  NDIRSINTMPFILE2=`expr $NDIRSINTMPFILE2 + $ONE`
  if [ $NDIRSINTMPFILE2 -eq $MAXDIRSINTMPFILE ]
  then
    if [ $i -ne $NTMPFILES ]
    then
      NTMPFILES2=`expr $NTMPFILES2 + $ONE`
      NDIRSINTMPFILE2=0
    fi
  fi
done

PID=""
for (( i=$START; i<=$NTMPFILES2; i++ ))
do
  TMPFILENAME2[i]="KFParticleFinderTmp_2"$i".root"
  hadd ${TMPFILENAME2[i]} ${TMPHISTOSNAMES2[i]} &> collog$i.log &
  PID=$PID" "$!
done
wait $PID

HISTOSNAMES=""
for (( i=$START; i<=$NTMPFILES2; i++ ))
do
  HISTOSNAMES=$HISTOSNAMES" "${TMPFILENAME2[i]}
done

echo $HISTOSNAMES
hadd KFParticleFinder.root $HISTOSNAMES &> collog.log &
PID=$!
wait $PID

rm -rf KFParticleFinderTmp*
rm -rf collog*
