void print_medialist(TString fileName)
{

  cout << "Open file " << fileName << endl;
  TFile* infile = TFile::Open(fileName);
  if (!infile->IsOpen()) {
    std::cout << "print_medialist: input file " << fileName << " is not accessible!" << std::endl;
    return;
  }

  CbmMediaList* matListPtr {};
  infile->GetObject("CbmMediaList", matListPtr);
  if (nullptr != matListPtr) {
    const std::vector<std::pair<TString, TString>>& matlist = matListPtr->GetVector();

    for (auto& info : matlist) {
      std::cout << info.first << ", " << info.second << std::endl;
    }
  }
  else {
    std::cout << "print_medialist: could not read needed data from file" << std::endl;
  }
  return;
}
