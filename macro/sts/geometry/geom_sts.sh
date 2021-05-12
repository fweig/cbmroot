# Copyright (C) 2016 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann

for STA in 1 2 3 4 5 6 7 8 
do
  root -l -q geom_sts.C\($STA\)
done
