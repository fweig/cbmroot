#ifndef CBMSTSDIGITIZEQAREPORT_H_
#define CBMSTSDIGITIZEQAREPORT_H_

#include "CbmSimulationReport.h"

class CbmStsParAsic;
class CbmStsParSim;
class CbmStsSetup;


class CbmStsDigitizeQaReport : public CbmSimulationReport {
public:
  CbmStsDigitizeQaReport(CbmStsSetup* setup, const CbmStsParSim* settings, const CbmStsParAsic* asicPar);
  virtual ~CbmStsDigitizeQaReport();


private:
  CbmStsSetup* fSetup           = nullptr;
  const CbmStsParSim* fSettings = nullptr;
  const CbmStsParAsic* fAsicPar = nullptr;
  virtual void Create();
  virtual void Draw();
  void DrawNofObjectsHistograms();
  void DrawLogHistograms();
  void DrawHistograms();
  void Draw2dHistograms();
  void ScaleHistograms();

  CbmStsDigitizeQaReport(const CbmStsDigitizeQaReport&);
  CbmStsDigitizeQaReport& operator=(const CbmStsDigitizeQaReport&);

  ClassDef(CbmStsDigitizeQaReport, 1)
};

#endif
