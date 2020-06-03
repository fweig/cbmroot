char HistText[256];

TFile *ParFile;
TFile *SimFile;
TFile *RecFile;

TH1F *H_Radius;
TH1F *H_aAxis;
TH1F *H_bAxis;
TH1F *H_boa;
TH1F *H_dR;
TH1F *H_nbHits;

TTree *cbmsim;
TTree *cbmrec;

TClonesArray *tracks;
TClonesArray *points;
TClonesArray *ReflPoints;
TClonesArray *hits;
TClonesArray *rings;
TClonesArray *ringmatch;

// ------------------------------------------------------------------- //
