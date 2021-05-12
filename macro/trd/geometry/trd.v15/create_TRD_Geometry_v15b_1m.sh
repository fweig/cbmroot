# Copyright (C) 2015 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

#
# create TRD geometry from patch file
#
# diff -c Create_TRD_Geometry_v15b_3e.C Create_TRD_Geometry_v15b_1m.C > b3etob1m.patch
#
# cp Create_TRD_Geometry_v13p4.C Create_TRD_Geometry_v13p1.C
# patch Create_TRD_Geometry_v13p1.C p4top1.patch
# root -l Create_TRD_Geometry_v13p1.C
#
SOURCE=Create_TRD_Geometry_v15b_3e.C
TARGET=Create_TRD_Geometry_v15b_1m.C
PATCH=b3etob1m.patch
#
echo 
echo cp $SOURCE $TARGET
echo patch $TARGET $PATCH
echo root -l $TARGET
echo 
#
cp $SOURCE $TARGET
patch $TARGET $PATCH
root -l $TARGET


