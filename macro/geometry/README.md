Tools in the Geometry Macro Folder
==================================

Scan Geometry (scan_geometry.C)
===============================

Scan geometry travels through the root geometry hierachy of a typical root geometry binary and outputs standard information regarding the node to the screen. There are several options, some of which are commented out in the file, which allows the easy comparison of root binaries. A GRAPHVIZ file named geo.gv is also output to the working directory.

Examing Text
------------
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

Examining Graphical Represetation
---------------------------------
Sometimes, especially if the output is very large, it can be more insighted, to get the overall picture to look at a graph tree of the volumes in a geometry instead. This can be done for a single detector or indeed the whole experimental setup, e.g. test.geo.root To make a pdf graph, a basic graph may be made with a command like
```
sed -n '2,$p' geo.gv | head -n -1 | sort | uniq -c | awk 'BEGIN{print "digraph {"}{printf "%s -> %s [label=\"%d\"]\n", $2, $4, $1}END{print "}"}' > geo2.gv
dot -Tpdf geo2.gv > geo.pdf
```


