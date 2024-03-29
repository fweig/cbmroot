# Copyright (C) 2016 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

rm all.*.par out/*.out.root out/*.params.root  out/*.hst.root

root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_26_18_21")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_26_22_22")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_27_11_42")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_27_16_45")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_27_16_47")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_08_07")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_09_01__0001")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_09_01__0002")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_09_01__0003")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_09_01__0004")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_28_09_01__0005")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_29_19_26__0006")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_29_19_26__0007")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_29_19_26__0008")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_29_19_26__0009")'
root -b -q 'rates_unpacking.C("/hera/cbm/users/tofGsiApr14/cern-nov15_mbs_rates/mbs/", "rates_2015_11_29_19_26__0010")'
