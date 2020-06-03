/** @file CbmRecoQa.h
 ** @author Tim Fauerbach
 ** @since Jun 2019
 **/

#ifndef CBMRECOQA
#define CBMRECOQA 1

#include <string>
#include "TFile.h"
#include "TH1.h"
#include "FairTask.h"
#include "FairRootManager.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"

#include <vector>
#include <string>
#include <array>
#include <utility>

class CbmRecoQa : public FairTask
{
  private:
    void record(std::string decName, int i); 
    TFile* pullresfile;
    int verbosity;
    std::vector<std::pair<std::string, std::array<int,4>>> detectors;
    std::vector<std::vector<TH1F*>> hists;
    CbmMCEventList* eventList;
    FairRootManager* fManager;
    CbmMCDataManager* mcManager;
    std::string outname;
  public:  
    CbmRecoQa(std::vector<std::pair<std::string, std::array<int,4>>> decNames, std::string outName = "test", int verbose_l = 0);
    ~CbmRecoQa();
    static CbmRecoQa* instance;
    virtual InitStatus ReInit();
    virtual InitStatus Init();
    virtual void FinishEvent();
    virtual void FinishTask();

    ClassDef(CbmRecoQa,1);
};

#endif
