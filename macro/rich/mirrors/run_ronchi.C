void run_ronchi() {
  //gSystem->Load("libCbmRichMirrors");
  //gSystem->Load("libtiff");

  std::string dir       = "/dir/";
  std::string fileName1 = "horizontal_191111.tif";
  std::string fileName2 = "vertical_191111.tif";

  CbmRichRonchiAna* ronchiAna = new CbmRichRonchiAna();
  ronchiAna->SetTiffFileNameH(fileName1);
  ronchiAna->SetTiffFileNameV(fileName2);
  ronchiAna->Run();
}
