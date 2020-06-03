LOGDIR=/lustre/nyx/cbm/users/$USER/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

export MAINDIR=`pwd`

sbatch -D "/lustre/nyx/cbm/users/$USER/log" --export=ALL batch_run.sh
