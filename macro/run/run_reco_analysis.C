/** @file run_reco_analysis.C
 ** @author Tim Fauerbach
 ** @since Jun 2019
 **/

// ---------------- Foward Declarations und struct Declarations

TClonesArray* ConnectBranchIfExist(TFile* f, TTree* t, TString branch, TString dataClass);
TDirectoryFile* GetResidual(TFile* f, std::string& component);

struct triple {
  Int_t entries;
  double mem;
  double time;
};

struct perf_component {
  TClonesArray* rec_branch;
  std::string histName;
  std::vector<triple> data;
  std::vector<TH1F*> monHists;
  std::vector<TH1*> hists;
};
/** Run Analysis of Reconstruction
 ** @param filename       Name of files to read from
 ** @param componentsInp  Vector Objects Containing 3 Strings Declaring in Order: ClassName, BranchName, TaskName
 ** @param residualInp    Vector of pairs {{component, { PullStdDevAllowed, ResidualXMeanAllowed, ResidualYMeanAllowed, ResidualTMeanAllowed}}} i.e. { {"sts", { .2, 12, 3, 4}}}
 ** @param maxMemoryUsed  Max Memeory used allowed for reconstruction in MB
 ** @param uploadFit      Create and Upload JPGS of Fitted Histograms
 **/
void run_reco_analysis(const char* filename = "test", std::vector<std::array<std::string, 3>> componentsInp = {},
                       std::vector<std::pair<std::string, std::array<float, 4>>> residualsInp = {},
                       int maxMemoryUsed = 5000, bool uploadFit = false)
{

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----------------- Open Files and Trees
  TString filename_data(filename);
  TString recFile  = filename_data + ".tb.rec.root";
  TString monFile  = filename_data + ".tb.rec.monitor.root";
  TString qaFile   = filename_data + ".qa.hists.root";
  TString fairFile = filename_data.Insert(filename_data.Last('/') + 1, "FairRunInfo_") + ".par.root";

  std::unique_ptr<TFile> f_rec(new TFile(recFile, "READ"));
  if (f_rec == nullptr) {
    std::cerr << "Could not open input file " << recFile << std::endl;
    exit(1);
  }

  std::unique_ptr<TTree> t_rec(static_cast<TTree*>(f_rec->Get("cbmsim")));
  if (t_rec == nullptr) {
    std::cerr << "Could not open cbmsimtree in input file  " << std::endl;
    exit(1);
  }

  std::unique_ptr<TFile> f_mon(new TFile(monFile, "READ"));
  if (f_mon == nullptr) {
    std::cerr << "Could not open input file " << monFile << std::endl;
    exit(1);
  }

  std::unique_ptr<TDirectory> t_mon(static_cast<TDirectory*>(f_mon->Get("MonitorResults")));
  if (t_mon == nullptr) {
    std::cerr << "Could not open MonitorResults" << std::endl;
    exit(1);
  }

  std::unique_ptr<TFile> f_l1(new TFile(qaFile, "READ"));
  if (f_l1 == nullptr) { std::cerr << "Could not open QA file" << std::endl; }

  std::unique_ptr<TFile> f_fri(new TFile(fairFile, "READ"));
  if (f_fri == nullptr) { std::cerr << "Could not open FairRunInfo file." << std::endl; }

  // -------------------------------------------------
  // Performance
  //--------------------------------------------------

  // ---- Declare and Fill Structs
  Int_t treeentries = t_rec->GetEntriesFast();

  std::vector<perf_component> components;
  for (int i = 0; i < componentsInp.size(); i++) {
    std::cout << "Connecting " << componentsInp[i][0];
    std::string tmpstring = componentsInp[i][2];
    std::replace(tmpstring.begin(), tmpstring.end(), '_', ' ');
    perf_component tmp = {ConnectBranchIfExist(f_rec.get(), t_rec.get(), componentsInp[i][0], componentsInp[i][1]),
                          tmpstring, std::vector<triple>(treeentries), std::vector<TH1F*>(2), std::vector<TH1*>(3)};
    if (tmp.rec_branch) {
      components.push_back(tmp);
      std::cout << " Branch Address: " << tmp.rec_branch << std::endl;
    }
    else {
      std::cout << " ... not found in Tree!" << std::endl;
    }
  };

  for (int i = 0; i < components.size(); i++) {
    TString name = components[i].histName;
    components[i].hists[0] =
      static_cast<TH1*>(new TH2F((name + "_Time"), "Time / Entries", treeentries, 0, 1, treeentries, 0, .1));
    components[i].hists[0]->SetCanExtend(TH1::kAllAxes);
    components[i].hists[1] =
      static_cast<TH1*>(new TH2F(name + "_Memory", "Memory / Entries", treeentries, 0, 1, treeentries, 0, 1));
    components[i].hists[1]->SetCanExtend(TH1::kAllAxes);
    components[i].hists[2] = static_cast<TH1*>(new TH1F(name + "_Entries", "Entries", treeentries, 0, 1));
    components[i].hists[2]->SetCanExtend(TH1::kAllAxes);
  }

  // ---- Get Time and Memory Usage Histograms
  TList* hist_list = t_mon->GetListOfKeys();
  TString tmp_str;

  for (int i = 0; i < components.size(); i++) {
    TObjLink* lnk = hist_list->FirstLink();
    while (lnk) {
      TObject* tmp = lnk->GetObject();
      tmp_str      = tmp->GetName();
      if (tmp_str.Contains(components[i].histName)) {
        if (tmp_str.Contains("_TIM")) components[i].monHists[0] = static_cast<TH1F*>(t_mon->Get(tmp_str));
        if (tmp_str.Contains("_MEM")) components[i].monHists[1] = static_cast<TH1F*>(t_mon->Get(tmp_str));
      }
      lnk = lnk->Next();
    }
  }
  // ---- Connect Number of Entries with Time and Memeory Usage in Data Struct
  std::cout << "Number of entries: " << treeentries << std::endl;
  for (int j = 0; j < treeentries; j++) {
    t_rec->GetEntry(j);
    std::cout << "TimeSlice: " << j << std::endl;
    for (int i = 0; i < components.size(); i++) {
      if (components[i].rec_branch->GetEntriesFast() > 0) {
        Int_t currententries = components[i].rec_branch->GetEntriesFast();
        std::cout << "\t\t" << components[i].rec_branch->GetName()
                  << " Entries: " << components[i].rec_branch->GetEntriesFast() << std::endl;
        double curr_time           = components[i].monHists[0]->GetBinContent(j + 1);
        double curr_mem            = components[i].monHists[1]->GetBinContent(j + 1);
        components[i].data[j]      = {components[i].rec_branch->GetEntriesFast(), 0, 0};
        components[i].data[j].time = curr_time;
        components[i].data[j].mem  = curr_mem;
        components[i].hists[0]->Fill(currententries, curr_time);
        components[i].hists[1]->Fill(currententries, curr_mem);
        components[i].hists[2]->Fill(currententries);
      }
    }
  }

  // -------------------------------------------------
  // CDash Output
  //--------------------------------------------------

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  char cwdbuff[PATH_MAX + 1];
  getcwd(cwdbuff, sizeof(cwdbuff));

  cout << endl << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;

  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemoryAnalysis\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;
  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;

  if (f_fri) {
    try {
      TH1F* tmphist = static_cast<TH1F*>(f_fri->Get("ResidentMemoryVsEvent"));
      double tmpmem = tmphist->GetMaximum();
      std::cout << "<DartMeasurement name=\"MaxMemoryReconstruction\" "
                   "type=\"numeric/double\">";
      std::cout << tmpmem;
      std::cout << "</DartMeasurement>" << std::endl;
      if (tmpmem > maxMemoryUsed) {
        std::cout << "Strong Deviation" << std::endl;
        std::cout << "Max Memory Used > " << maxMemoryUsed << std::endl;
      }
    }
    catch (std::exception& e) {
      std::cout << "Could not find ResidentMemoryVsEvent" << std::endl;
    }
  }

  // ---- Submit Parameters of Fitted Functions as HTML Table

  std::string htmlString = "<DartMeasurement name=\"PerformanceTable\" type=\"text/html\"><![CDATA[";
  htmlString += "<table><tr><th>Component</th><th>Histogramm</"
                "th><th>Parameter</th><th>Value</th><th>Error</th></tr>";
  for (int i = 0; i < components.size(); i++) {
    std::string tmpname = components[i].histName;
    tmpname.erase(std::remove_if(tmpname.begin(), tmpname.end(), [](unsigned char x) { return std::isspace(x); }),
                  tmpname.end());
    std::cout << "<DartMeasurement name=\"" << tmpname << "SumTime\" type=\"numeric/double\">"
              << components[i].monHists[0]->GetSum() << "</DartMeasurement>" << std::endl;
    std::cout << "<DartMeasurement name=\"" << tmpname << "TimePerEntry\" type=\"numeric/double\">"
              << components[i].monHists[0]->GetSum() / treeentries << "</DartMeasurement>" << std::endl;
    std::cout << "<DartMeasurement name=\"" << tmpname << "MaxMemory\" type=\"numeric/double\">"
              << components[i].monHists[1]->GetMaximum() << "</DartMeasurement>" << std::endl;
    for (int j = 0; j < components[i].hists.size() - 2; j++) {
      components[i].hists[j]->Fit("pol3", "Q");
      TF1* fit = components[i].hists[j]->GetFunction("pol3");
      htmlString +=
        "<tr><td>" + components[i].histName + "</td>" + "<td>" + components[i].hists[j]->GetName() + "</td>";
      htmlString += "<td>p0</td><td>" + std::to_string(fit->GetParameter(0)) + "</td><td>"
                    + std::to_string(fit->GetParError(0)) + "</td></tr>";
      htmlString += "<td></td><td></td><td>p1</td><td>" + std::to_string(fit->GetParameter(1)) + "</td><td>"
                    + std::to_string(fit->GetParError(1)) + "</td></tr>";
      htmlString += "<td></td><td></td><td>p2</td><td>" + std::to_string(fit->GetParameter(2)) + "</td><td>"
                    + std::to_string(fit->GetParError(2)) + "</td></tr>";
      if (uploadFit) {
        std::string tmpstring = filename;
        tmpstring.append(components[i].hists[j]->GetName());
        tmpstring.append(".jpg");
        components[i].hists[j]->SetMarkerStyle(20);
        components[i].hists[j]->Draw();
        gPad->Print(tmpstring.c_str());
        std::cout << "<DartMeasurementFile name=\"" << tmpstring << "\" type=\"image/jpg\">" << cwdbuff << "/"
                  << tmpstring << "</DartMeasurementFile>" << std::endl;
      }
    }
    TH1F* currHist = static_cast<TH1F*>(components[i].hists[2]);
    htmlString += "<tr><td>" + components[i].histName + "</td>" + "<td>" + currHist->GetName() + "</td>";
    htmlString += "<td>Mean</td><td>" + std::to_string(currHist->GetMean()) + "</td><td>"
                  + std::to_string(currHist->GetMeanError()) + "</td></tr>";
    htmlString += "<tr><td></td><td></td><td>StdDev</td><td>" + std::to_string(currHist->GetStdDev()) + "</td><td>"
                  + std::to_string(currHist->GetStdDevError()) + "</td></tr>";
  }

  htmlString += "</table>]]></DartMeasurement>";
  //  std::cout << htmlString << std::endl;

  // -------------------------------------------------
  // Residuals
  //--------------------------------------------------
  if (f_l1) {

    for (int i = 0; i < residualsInp.size(); i++) {
      TDirectoryFile* residualDir   = GetResidual(f_l1.get(), residualsInp[i].first);
      std::vector<std::string> dims = {"x", "y", "t"};
      for (int k = 0; k < dims.size(); k++) {
        std::string tmp       = dims[k];
        std::string tmpstring = "P" + tmp + "_" + residualsInp[i].first;
        std::cout << "Finding " << tmpstring << std::endl;
        TH1F* tmphist        = static_cast<TH1F*>(residualDir->Get(tmpstring.c_str()));
        float pullDevAllowed = residualsInp[i].second[0];
        if (tmphist && pullDevAllowed != -1) {
          float tmp_stdDev = tmphist->GetStdDev();
          if (((tmp_stdDev > 1 + pullDevAllowed) || (tmp_stdDev < 1 - pullDevAllowed))
              && (tmp_stdDev != 0 && tmphist->GetEntries() != 0)) {
            TString tmpfilename       = "_P" + tmp + residualsInp[i].first;
            TString tmpfilenamesuffix = filename_data + tmpfilename + ".jpg";
            tmphist->Draw();
            gPad->Print(tmpfilenamesuffix);
            std::cout << "<DartMeasurementFile name=\"";
            std::cout << tmpfilename;
            std::cout << "\" type=\"image/jpg\">";
            std::cout << cwdbuff << "/" << filename_data << tmpfilename << ".jpg";
            std::cout << "</DartMeasurementFile>" << std::endl;
            std::cout << "Strong Deviation" << std::endl;
            std::cout << "Pull " << tmpfilename << std::endl;
          }
        }
        else {
          std::cout << "Could not find Histogram " + tmpstring << std::endl;
        }
        tmpstring = tmp + "_" + residualsInp[i].first;
        std::cout << "Finding " << tmpstring << std::endl;
        TH1F* tmphist_2      = static_cast<TH1F*>(residualDir->Get(tmpstring.c_str()));
        float resMeanAllowed = residualsInp[i].second[1 + k];
        if (tmphist_2 && resMeanAllowed != -1) {
          float tmp_mean = tmphist->GetMean();
          if ((tmp_mean < 0 - resMeanAllowed || tmp_mean > 0 + resMeanAllowed) && tmphist_2->GetEntries() != 0) {
            TString tmpfilename       = "_" + tmp + residualsInp[i].first;
            TString tmpfilenamesuffix = filename_data + tmpfilename + ".jpg";
            tmphist_2->Draw();
            gPad->Print(tmpfilenamesuffix);
            std::cout << "<DartMeasurementFile name=\"";
            std::cout << tmpfilename;
            std::cout << "\" type=\"image/jpg\">";
            std::cout << cwdbuff << "/" << filename_data << tmpfilename << ".jpg";
            std::cout << "</DartMeasurementFile>" << std::endl;
            std::cout << "Strong Deviation" << std::endl;
            std::cout << "Residual " << tmpfilename << std::endl;
          }
        }
        else {
          std::cout << "Could not find Histogram " + tmpstring << std::endl;
        }
      }
    }
  }
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}

TClonesArray* ConnectBranchIfExist(TFile* f, TTree* t, TString branch, TString dataClass)
{
  TClonesArray* tcl {nullptr};
  TList* list = dynamic_cast<TList*>(f->Get("BranchList"));
  if (list) {
    for (Int_t i = 0; i < list->GetEntries(); i++) {
      TObjString* Obj = dynamic_cast<TObjString*>(list->At(i));
      if (Obj) {
        TString ObjName = Obj->GetString();
        if (ObjName.EqualTo(branch)) {
          tcl = new TClonesArray(dataClass);
          t->SetBranchAddress(branch, &tcl);
          break;
        }
      }
    }
  }
  return tcl;
}

TDirectoryFile* GetResidual(TFile* f, std::string& component)
{
  TString tmp            = component;
  TDirectoryFile* dirptr = static_cast<TDirectoryFile*>(f->Get(tmp));
  return dirptr;
}
