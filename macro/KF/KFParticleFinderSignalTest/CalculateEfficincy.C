// #include "$VMCWORKDIR/KF/KFParticleParformance/KFPartEfficiencies.h"

void CalculateEfficincy(TString workdir="data") {

  TStopwatch timer;
  timer.Start();

  KFPartEfficiencies fEfficiency;
  
  const int nDirs = 126;
  
  workdir += "/Signal";
  for(int i=0; i<nDirs; i++)
  {
    TString fileName = workdir;
    fileName += i;
    fileName += "/Efficiency_KFParticleFinder.txt";
    std::fstream file(fileName.Data(),fstream::in);
    if(!(file.is_open()))
    {
      std::cout << "File does not exist: " << std::endl;
      std::cout << "    " << fileName << std::endl;
      continue;
    }
    KFPartEfficiencies eff;
    eff.AddFromFile(fileName.Data());
    fEfficiency += eff;
  }

  fEfficiency.CalcEff();
  fEfficiency.PrintEff();
}
