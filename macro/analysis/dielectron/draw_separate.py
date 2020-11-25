import os
import sys
import shutil

def main()
   particle = sys.argv[1]
   histDir =  "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm"
   histDirPart = histDir + "/sepHists/" + particle
   os.makedirs(histDirPart)
   os.chdir(histDirPart)
   os.system(('root -l -b -q {}/draw_sep_ana.C\(\\"{}\\}').format(histDirPart, particle))
   os.system(('root -l -b -q {}/draw_sep_litqa.C\(\\"{}\\}').format(histDirPart, particle))



if __name__ == '__main__':
    main()
