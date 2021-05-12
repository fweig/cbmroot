# Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

for i in a b c d e f g h i j k
do
#  echo === trd_v17${i}_1e.digi.par
#  tail -6 trd_v17${i}_1e.digi.par
  echo root -l Create_TRD_Geometry_v17${i}.C
  root -l Create_TRD_Geometry_v17${i}.C
  echo
  echo
done
