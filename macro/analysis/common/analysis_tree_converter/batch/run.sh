LOGDIR=/lustre/cbm/users/$USER/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition main -D "$LOGDIR" --export=ALL batch_run.sh
