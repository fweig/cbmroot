# Geometry related helper scripts and macros

In this directory a collection of ROOT macros and shell scripts is available
which helps to create or check information which is stored in binary ROOT
files which contain a TGeoManager.

## Scan Geometry (scan_geometry.C)

Scan geometry travels through the root geometry hierachy of a typical root geometry binary and outputs standard information regarding the node to the screen. There are several options, some of which are commented out in the file, which allows the easy comparison of root binaries. A GRAPHVIZ file named geo.gv is also output to the working directory.

### Examing Text

A typical use may be

```
root -l -q scan_geometry.C("pipe_v16b_1e.geo.root") | less
```

to look at output on screen. To save this output to a file, something like

```
root -l -q '~/cbmroot/macro/geometry/scan_geometry.C("tof_v20a_1h.geo.root")' 2>&1 1>tof_v20a_1h
```

Outputing two files may be compared with standard unix diff such as

```
diff -y -W 200 tof_v20a_1h tof_v20b_1h | less
```

### Examining Graphical Represetation

Sometimes, especially if the output is very large, it can be more insighted, to get the overall picture to look at a graph tree of the volumes in a geometry instead. This can be done for a single detector or indeed the whole experimental setup, e.g. test.geo.root To make a pdf graph, a basic graph may be made with a command like

```
sed -n '2,$p' geo.gv | head -n -1 | sort | uniq -c | awk 'BEGIN{print "digraph {"}{printf "%s -> %s [label=\"%d\"]\n", $2, $4, $1}END{print "}"}' > geo2.gv
dot -Tpdf geo2.gv > geo.pdf
```

---

## check_media.C

The macro compares the media information at execution time with the media
information assigned to the nodes when building the detector system
geometry. The macro is executed automatically in the CbmRoot test suite for
all tested setups.
If needed it can be executed also manually as decribed in the following.
If the CbmRoot run time environment is set and a geometry file
for a CBM setup is available the macro is used  in the following way

root -l $VMCWORKDIR/macro/geometry/check_media.C'("setup_name")'

The setup_name is the name of the setup used during the simulation to create
the geometry file.

This macro to check the sanity odf the full geometry is/was needed due to
the way we build the complete CBM geometry from independent geometries of the
several detector systems.
When building the complete geometry it happened that the media information
of some nodes was overridden by wrong media information. This problem results
in wrong physical results. The error was fixed but to catch the problem if
it reappears the macro is executed in the test suite for all tested setups.

---

## create_medialist.C

Macro to save the media information when a detector geometry is generated.
The assumption is that the media information is correct at the time when the
detector geometry is created. At the end of this process the macro is
executed and a binary file with the media information for this specific
detector geometry is created. The created root file has to be added to the
input repository in the directory geometry_check.
If needed the macro can also be executed manually with the following procedure.
The macro can be executed only if there is exactly on volume in the geometry file.
To achieve this some preparatory work is needed.

1. Setup the CbmRoot run time environment

   source $BUILDIR/config.sh (if using from build directory)\
   source $INSTALLDIR/bin/CbmRootConfig.sh (if using from installation directory)

2. Create a new test setup

   1. Copy one of the existing setups in geometry/setup
   2. Comment all lines `setup->SetModule(****);` using  single line comments `//` only except the line with the
      detector geometry you want to use

3. Modify the simulation macro

   Remove the target from macro/run/run_tra_file.C by commenting the line `run.SetTarget(****);`

4. Execute the simulation macro to create a geometry file

   root -l $VMCWORKIRDIR/macro/run/run_tra_file.C\(\"$VMCWORKDIR/input/urqmd.auau.10gev.centr.root\",
                           1,\"data/${setup}_test\", \"${setup}\"\)

   The variable setup has to be name of the setup created in 2.

5. Execute the macro create_medialist.C

   root -l $VMCWORKDIR/macro/geometry/create_medialist.C\(\"data/${setup}_test.geo.root\"\)

---

## print_medialist.C

The macro can be used to print the information which is stored in the *geometrycheck.root
files. The output is a list of all stored nodes with the corresponing media
name. To use the macro do

root -l $VMCWORKDIR/macro/geometry/print_medialist.C\(\"$VMCWORKDIR/input/geometry_check/sts_v19a_geometrycheck.root\"\)

---

## create_positionlist.C

Macro to store the global positions of the center of each nodes of a detector system in a
file. The usage is equal to the first four steps described for
create_medialist.C Only the last step is slightly different and creates the
output file example.txt

   root -l $VMCWORKDIR/macro/geometry/create_positionlist.C\(\"data/${setup}_test.geo.root\"\)

---

## costum_setup.sh

---

## switch_defaults.sh

This script switches between official and trial versions of the CBMROOT geometries. This
is intended for use by a expert user, who will remember to swith back to the official
geometry relase (currently APR21) once the specific use case has ended. Current options
include This script switches between official and trial versions of the CBMROOT geometries. This
is intended for use by a knowledgable user, who will remember to swith back to the official
geometry relase (currently APR21) once the specific use case has ended. Current options
include

* APR20 - (previous 2020 default geometries. Run old defaults with the new CBMROOT software.)
* APR21 - (current 2021 default geometries. This is the official release geometries.
* TEST  - (Geoemtries shift such that the center of the magnet is the origin of the CBM exp.)

