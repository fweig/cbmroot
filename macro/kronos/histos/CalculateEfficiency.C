void CalculateEfficiency(TString inputDir) {

  TStopwatch timer;
  timer.Start();

  std::cout << "Start" << std::endl;

  KFPartEfficiencies fEfficiency;

  int nFiles = 5000;
  for(int i=0; i<nFiles; i++)
  {
    cout << "File " << i << endl;
    
    TString num = "";
    num += i;
    
    TString fileName = inputDir + TString("/") + num;
    
    fileName += "/Efficiency_KFParticleFinder.txt";
    cout << fileName << std::endl;
    
    std::fstream file(fileName.Data(),fstream::in);
    if(!file.is_open()) continue;
    KFPartEfficiencies eff;
    eff.AddFromFile(fileName.Data());
    fEfficiency += eff;
  }

  fEfficiency.CalcEff();
  fEfficiency.PrintEff();
  
  // std::fstream outFile("Efficiency.txt",fstream::out);
  // outFile << fEfficiency;
  // outFile.close();
}
