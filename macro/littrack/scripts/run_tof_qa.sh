# Copyright (C) 2013 GSI/JINR-LIT
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Andrey Lebedev

xterm -hold -e ". ./tof_qa.sh sts_tof_v13a tof/tof_v13a.geo.root ${VMCWORKDIR}/parameters/tof/tof_v13a.digi.par 8gev"&
#xterm -hold -e ". ./tof_qa.sh sts_tof_v13b tof/tof_v13b.geo.root ${VMCWORKDIR}/parameters/tof/tof_v13b.digi.par 8gev"&
#xterm -hold -e ". ./tof_qa.sh sts_tof_v07a tof/tof_v07a.geo nofile 8gev"&
