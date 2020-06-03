
#ifndef CBMANAJPSIREPORT_H
#define CBMANAJPSIREPORT_H

#include "CbmSimulationReport.h"
#include "CbmAnaJpsiHist.h"

#include "TSystem.h"

#include <string>
#include <vector>

class CbmAnaJpsiReport : public CbmSimulationReport
{
public:
   /**
    * \brief Constructor.
    */
	CbmAnaJpsiReport();

   /**
    * \brief Destructor.
    */
   virtual ~CbmAnaJpsiReport();

protected:
   /**
    * \brief Inherited from CbmSimulationReport.
    */
   virtual void Create();

   /**
    * \brief Inherited from CbmSimulationReport.
    */
   virtual void Draw();

   void DrawAnalysisStepsH2(
         const std::string& hName,
		 bool DoDrawEfficiency);

   void DrawAnalysisStepsH1(
            const std::string& hName,
            bool doScale,
			double min = -1.,
			double max = -1.);

   void DrawSourceTypesH1(
         const std::string& hName,
         bool doScale = false);

   void DrawCutH1(
         const std::string& hName,
         double cutValue,
		  bool doScale = false);

   void Draw2DCut(
         const std::string& hist);

   void DrawCutDistributions();

   void DrawSignalMinv();

   void DrawMinvMismatches(
   		int step);

   void DrawMinvMismatchesAll();

   void DrawEfficiency(const std::string& h,
		   const std::string& hMC);

   void DrawPtYEfficiency(
			int step);

   void DrawPtYEfficiencyAll();

   void SetAnalysisStepLabels(
         TH1* h);

   double SignalOverBg(
		   int step);

   void SignalOverBgAllSteps();

   void DrawMinvSAndBg(
   		int step);

   void DrawMinvSAndBgAllSteps();

   void DrawMomEffAllSteps();

   void DrawMomMcVsRec();

   void DrawPairSourceAnaSteps(
		int step);

   void DrawPairSource();

   void DrawBgSource2D(
         const std::string& histName,
         const std::vector<std::string>& yLabels,
         const std::string& zTitle);


	ClassDef(CbmAnaJpsiReport, 1);
};

#endif /* CBMANAJPSIREPORT_H */

