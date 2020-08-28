Howto calibrate run HD_cosmic_2020-08-21
========================================

nohup ./calib_batch.sh 900921911 901 HD_cosmic_2020-08-21 .50.3.0.0 &> Calib_HD_cosmic_2020-08-21.50.3.0.0.out &

nohup ./iter_tracks.sh HD_cosmic_2020-08-21.50.3.0.0 11 "900921911_901" &> IterTracks_HD_cosmic_2020-08-21.50.3.0.0_11.out &

nohup ./trk_cal_digi.sh HD_cosmic_2020-08-21.50.3.0.0 900921911 901 50 HD_cosmic_2020-08-21.50.3.0.0 2 &> TrkCal_HD_cosmic_2020-08-21.50.3.0.0_2.out &

nohup ./eval_tracks.sh HD_cosmic_2020-08-21.50.3.0.0 600 921 911 21 900921911_901 &> Eval_HD_cosmic_2020-08-21.50.3.0.0_600_21.out &
