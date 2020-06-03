#ifndef CBMTRDUTILS_H
#define CBMTRDUTILS_H 1

#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t
#include <TString.h>     // for TString

#include <map>           // for map
#include <vector>        // for vector

class CbmTrdParModDigi;
class CbmTrdParSetDigi;
class TH1;
class TH2;
class TH3;
class TLegend;
class TPolyLine;
class TProfile;

class CbmTrdUtils
{
 public:

  CbmTrdUtils();
  virtual ~CbmTrdUtils();
  void InitColorVector(Bool_t logScale, Double_t min, Double_t max);
  Int_t GetColorCode(Double_t value);
  void Statusbar(Int_t i, Int_t n);
  Int_t GetModuleType(Int_t moduleAddress, CbmTrdParModDigi *fModuleInfo, CbmTrdParSetDigi *fDigiPar);
  void NiceLegend(TLegend *l);
  void NiceTProfile(TProfile *h, Int_t color, Int_t mStyle, Int_t mSize, TString xTitle, TString yTitle) ;
  void NiceTH3(TH3 *h, Int_t color, Int_t mStyle, Int_t mSize, TString xTitle, TString yTitle, TString zTitle);
  void NiceTH2(TH2 *h, Int_t color, Int_t mStyle, Int_t mSize, TString xTitle, TString yTitle, TString zTitle);
  void NiceTH1(TH1 *h, Int_t color, Int_t mStyle, Int_t mSize, TString xTitle, TString yTitle);
  void CreateLayerView(std::map<Int_t/*moduleAddress*/, TH1*>& Map, CbmTrdParModDigi *fModuleInfo, CbmTrdParSetDigi *fDigiPar, const TString folder, const TString pics, const TString zAxisTitle, const Double_t fmax, const Double_t fmin, const Bool_t logScale);
  TPolyLine *CreateTriangularPad(Int_t column, Int_t row, Double_t value, Double_t min_range, Double_t max_range, Bool_t logScale);
  TPolyLine *CreateRectangularPad(Int_t column, Int_t row, Double_t value, Double_t min_range, Double_t max_range, Bool_t logScale);
  static TString GetGeantName(Int_t GeantId) {
    if (GeantId == 1)
      return (TString)"#gamma";
    else if (GeantId == 2)
      return (TString)"e^{+}";
    else if (GeantId == 3)
      return (TString)"e^{-}";
    else if (GeantId == 4)
      return (TString)"#nu_{(e;#mu;#tau)}";
    else if (GeantId == 5)
      return (TString)"#mu^{+}";
    else if (GeantId == 6)
      return (TString)"#mu^{-}";
    else if (GeantId == 7)
      return (TString)"#pi^{0}";
    else if (GeantId == 8)
      return (TString)"#pi^{+}";
    else if (GeantId == 9)
      return (TString)"#pi^{-}";
    else if (GeantId ==10)
      return (TString)"K^{0}_{L}";
    else if (GeantId ==11)
      return (TString)"K^{+}";
    else if (GeantId ==12)
      return (TString)"K^{-}";
    else if (GeantId ==13)
      return (TString)"n";
    else if (GeantId ==14)
      return (TString)"p";
    else if (GeantId ==15)
      return (TString)"#bar{p}";
    else if (GeantId ==16)
      return (TString)"K^{0}_{S}";
    else if (GeantId ==17)
      return (TString)"#eta";
    else if (GeantId ==18)
      return (TString)"#Lambda";
    else if (GeantId ==19)
      return (TString)"#Sigma^{+}";
    else if (GeantId ==20)
      return (TString)"#Sigma^{0}";
    else if (GeantId ==21)
      return (TString)"#Sigma^{-}";
    else if (GeantId ==22)
      return (TString)"#Xi^{0}";
    else if (GeantId ==23)
      return (TString)"#Xi^{-}";
    else if (GeantId ==24)
      return (TString)"#Omega^{-}";
    else if (GeantId ==25)
      return (TString)"#bar{n}";
    else if (GeantId ==26)
      return (TString)"#bar{#Lambda}";
    else if (GeantId ==27)
      return (TString)"#bar{#Sigma}^{-}";
    else if (GeantId ==28)
      return (TString)"#bar{#Sigma}^{0}";
    else if (GeantId ==29)
      return (TString)"#bar{#Sigma}^{+}";
    else if (GeantId ==30)
      return (TString)"#bar{#Xi}^{0}";
    else if (GeantId ==31)
      return (TString)"#bar{#Xi}^{+}";
    else if (GeantId ==32)
      return (TString)"#bar{#Omega}^{+}";
    else if (GeantId ==33)
      return (TString)"#tau^{+}";
    else if (GeantId ==34)
      return (TString)"#tau^{-}";
    else if (GeantId ==35)
      return (TString)"D^{+}";
    else if (GeantId ==36)
      return (TString)"D^{-}";
    else if (GeantId ==37)
      return (TString)"D^{0}";
    else if (GeantId ==38)
      return (TString)"#bar{D}^{+}";
    else if (GeantId ==39)
      return (TString)"D_{S}^{+}";
    else if (GeantId ==40)
      return (TString)"#bar{D_{S}}^{-}";
    else if (GeantId ==41)
      return (TString)"#Lambda_{C}^{+}";
    else if (GeantId ==42)
      return (TString)"W^{+}";
    else if (GeantId ==43)
      return (TString)"W^{-}";
    else if (GeantId ==44)
      return (TString)"Z^{0}";
    else if (GeantId ==45)
      return (TString)"d";
    else if (GeantId ==46)
      return (TString)"t";
    else if (GeantId ==47)
      return (TString)"He";
    else if (GeantId ==48)
      return (TString)"#gamma_{RICH}";
    else if (GeantId ==49)
      return (TString)"Primary";
    else
      return (TString)"not known";
  };
  static Int_t PdgToGeant(Int_t PdgCode);
 private:
  std::vector<Int_t> fColors;
  std::vector<Double_t> fZLevel;

};
#endif
