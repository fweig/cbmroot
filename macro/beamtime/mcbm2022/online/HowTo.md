
# Submit scripts starting the slurm jobs

4 scripts are provided, 3 to start topologies and 1 to stop topologies:
- start_topology.sh
- start_topology_array.sh
- start_topology_servers.sh
- stop_topology.sh

All of these scripts assume that we have 4 processing nodes available in slurm, named `en[13-16]`.
Each processing ndoe is connected to a tsclient publisher with 1/2 of the TS of a TS builder node, resulting in a full
processing with equal sharing in case of 2 builder nodes and in a 2/3 processing with equal sharing in case of 3 builder
nodes.
For the case with 4 TS builder nodes, new scripts should be developed, either connecting one processing node to each or
running the processing topology directly on the builder node (with a strong attention to memory consumption).

## start_topology.sh

This starts a job based on `mq_processing_node.sbatch` on each of the 4 processing nodes.

It expects 4 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.

Each process in the topology is started in the background, so in order to avoid the full job being killed when reaching
the end of the startup phase, an infinite loop has to be started which is exited only when the number of process in the
current session goes under a predefined threshold.

## start_topology_array.sh

This starts a job based on `mq_processing_node_array.sbatch` on each of the 4 processing nodes.

It expects 4 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.

The difference with the previous scripts is that these sbatch jobs try to make use of the array functionality of SLURM
to start the topology processes instead of starting them in the background.
This would have simplified the process management as they then each appear as a sub-job in the SLURM interface.
This however cannot be used on the mFLES for the time being as the SLURM server there does not have ressource allocation
and management enabled.

## start_topology_servers.sh

In addition to the 4 processing nodes, this script assumes that we have a 5th node available for running the common
parts of the topologies:
- parameter server
- histogram server
It then starts the main processes of the topology on the processing nodes with one job for each level (source, sink,
unpackers, event builders), making use of the `oversubscribe` sbatch option which allows to run up to 4 jobs per node
without checking the available ressources.

It also expects 4 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
Internally two more parameters are provided to each of the SLURM jobs in order to share with them the address on which
the common services are available.

## stop_topology.

This starts a job based on `mq_shutdown.sbatch` on each of the 4 processing nodes.
This *sbatch* script will send a SIGINT signal to the processes started by a topology in the following order, trying
thus to achieve a clean shutdown:
1. RepReqTsSampler;
1. Unpackers
1. Event Builders
1. Event Sink
1. Histogram server (if any)
1. Parameter server (if any)

In each case, it will wait until all process matching the expected name for a given level are gone before sending the
next signal.

It expects a single parameter:
- the `<Run Id>`, as reported by flesctl

This script is meant to be run after the the `start_topology.sh` one.
It will also work in the case of the `start_topology_array.sh` and `start_topology_servers.sh`, but in these case using
the `scancel` command of SLURM is a cleaner solution.

# SBATCH scripts

In total 10 SBATCH scripts are used for these various methods of starting the smae topology:
- create_log_folder.sbatch
- mq_processing_node.sbatch
- mq_processing_node_array.sbatch
- mq_shutdown.sbatch
- mq_parserv.sbatch
- mq_histoserv.sbatch
- mq_source.sbatch
- mq_sink.sbatch
- mq_unpackers.sbatch
- mq_builders.sbatch

For all parameter server devices, the set of parameter files and setup files is picked based on the provided `<Run Id>`
(see lists of parameters).

## create_log_folder.sbatch

This script is used to prepare all necessary log folders on the `/local` disk in case not already done, irrespective of
the target disk for the selected data and before starting the topology itself.
It is used in all of the startup scripts

It expects a single parameter:
- the `<Run Id>`, as reported by flesctl

## mq_processing_node.sbatch

This is the only topology script in the `start_topology.sh` case.

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

In addition, 2 optional parameters can be provided:
- the `<histogram server host>`, which would allow to use a common server
- the `<parameter server host>` (only if the histo server is provided), which would allow to use a common server
These two servers will be started by this script only if not overwritten by the user parameters.

The script will start the following processes in the background in this order:
- histogram server device, if no hostname provided
- source device
- parameter server device, if no hostname provided
- sink device
- `N` pairs of unpacker and event builder devices

It then enters an infinite loop until the number of processes in the attached sessions goes down to less than `6`.
The check is done every `5 seconds` and to monitor this the list of processes and total count is written to a file in
the log folder called `still_running.txt`.

## mq_processing_node_array.sbatch

This is the only topology script in the `start_topology_array.sh` case.

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

Depending on the sub-job index provided by SLURM, it will start either the histogram server (index 1), sampler (index
2), parameter server (index 3), event sink (index 4)

## mq_shutdown.sbatch

It does not expect any parameters.

This script will follow this sequence:
1. Send SIGINT to all processes named `RepReqTsSampler` (Source device)
1. Wait until all such processes are gone (check every `1 second`)
1. Send SIGINT to all processes named `MqUnpack` (Unpackers)
1. Wait until all such processes are gone (check every `1 second`)
1. Send SIGINT to all processes named `BuildDig` (Event Builders)
1. Wait until all such processes are gone (check every `1 second`)
1. Send SIGINT to all processes named `DigiEventSink` (Event Sink)
1. Wait until all such processes are gone (check every `1 second`)
1. Send SIGINT to all processes named `MqHistoServer` , if any (Histogram server)
1. Wait until all such processes are gone (check every `1 second`)
1. Send SIGINT to all processes named `parmq-server` , if any (Parameter server)
1. Wait until all such processes are gone (check every `1 second`)

## mq_histoserv.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

The full list of parameters is just for compatibility reasons, in the end only the `<Run Id>` is used to go in the right
log folder and the `<Trigger set>` to name the log file.


The process could be started in the foreground, therefore blocking the SLURM job until it returns.
But in order to be as close as possible to the "all-in-one" version, it is started in the background with an infinite
check loop behind identical to the latter.

## mq_parserv.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

The full list of parameters is just for compatibility reasons, in the end only the `<Run Id>` is used in this script to
select the parameter files and setup and to go in the right log folder and the `<Trigger set>` to name the log file.

The process could be started in the foreground, therefore blocking the SLURM job until it returns.
But in order to be as close as possible to the "all-in-one" version, it is started in the background with an infinite
check loop behind identical to the latter.

## mq_source.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

In addition, 2 optional parameters can be provided:
- the `<histogram server host>`, which would allow to use a common server
- the `<parameter server host>` (only if the histo server is provided), which would allow to use a common server
If not used, the script will expect both servers to be running on the localhost interface at `127.0.0.1`

The parameters for `<Number of branches>` and `<Disk index>` are not used in this script.

The process could be started in the foreground, therefore blocking the SLURM job until it returns.
But in order to be as close as possible to the "all-in-one" version, it is started in the background with an infinite
check loop behind identical to the latter.

## mq_sink.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

In addition, 2 optional parameters can be provided:
- the `<histogram server host>`, which would allow to use a common server
- the `<parameter server host>` (only if the histo server is provided), which would allow to use a common server
If not used, the script will expect both servers to be running on the localhost interface at `127.0.0.1`

The parameter for `<Nb branches>` is used to set the limit for the size of the ZMQ buffer of processed timeslices (1 per
branch).

The parameter for `<source hostname>` is not used in this script.

The process could be started in the foreground, therefore blocking the SLURM job until it returns.
But in order to be as close as possible to the "all-in-one" version, it is started in the background with an infinite
check loop behind identical to the latter.

## mq_unpackers.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

In addition, 2 optional parameters can be provided:
- the `<histogram server host>`, which would allow to use a common server
- the `<parameter server host>` (only if the histo server is provided), which would allow to use a common server
If not used, the script will expect both servers to be running on the localhost interface at `127.0.0.1`

The parameters for `<source hostname>` and `<Disk index>` are not used in this script.

The limit for the size of the ZMQ buffer of processed timeslices at the output of each branch is set to 2.

The processes cannot be started in the foreground, as multiple need to be created (1 per branch.
So in order to be as close as possible to the "all-in-one" version, they are started in the background with an infinite
check loop behind identical to the latter.

## mq_builders.sbatch

It expects 5 parameters in the following order:
- the `<Run Id>`, as reported by flesctl
- the `<Number of branches to be started per node>`, leading to a total parallel capability of `4 x n` timeslices
- the `<Trigger set>` in the range `[0-14]`, with `[0-6]` corresponding to the trigger settings tested by N. Herrmann
  and `[7-14]` those used for development by P.-A. Loizeau
- the `<Disk index>` in the range `[0-8]`, with `0` indicating the `/local/mcbm2022` disk-folder pair and `[1-8]`
  indicating the `/storage/<n>/` disks.
- the `<TS source full hostname>` which should be a combination `hostname:port`. In order to avoid overloading the
  standard network, it is critical here to target an `ibX`, e.g. `node8ib2:5561`

In addition, 2 optional parameters can be provided:
- the `<histogram server host>`, which would allow to use a common server
- the `<parameter server host>` (only if the histo server is provided), which would allow to use a common server
If not used, the script will expect both servers to be running on the localhost interface at `127.0.0.1`

The parameters for `<source hostname>` and `<Disk index>` are not used in this script.

The limit for the size of the ZMQ buffer of processed timeslices at both the input and the output of each branch is set
to 2.

The processes cannot be started in the foreground, as multiple need to be created (1 per branch.
So in order to be as close as possible to the "all-in-one" version, they are started in the background with an infinite
check loop behind identical to the latter.

# Known problems

1. Some memory leak in the Sink leads to a final memory usage of `~12 GB` even after the events of all TS are pushed to
   disk
1. Something fishy is happening with the ZMQ buffering, as even without re-ordering and missing TS insertion, the memory
   usage of the sink increase up to `180 GB`, which is far more than expected with the HWM of 2 messages at input
1. The plots generated by the sink for the buffer monitoring and processed TS/Event counting have messed up scales
