// Author: Marios A. Kagarlis
// Written: 15.11.98
// Revised: 20.07.99
// Revised: 30.08.00  R. Holzmann
// Revised: 07.09.00  J. Ritman (added parent-,siblings-,daughter- index
//                    and decayTime)
// Revised: 15.12.00  R. Holzmann  (creation time added)
// Revised: 22.03.05  R. Holzmann  (get/setParent() added)
// Revised: 23.07.07  IF (new framework) 

// PParticle Class Header
 
#ifndef _PPARTICLE_H_
#define _PPARTICLE_H_

#define MAX_DAUGHTERS 7

#include "TLorentzVector.h"
#include "TMath.h"
#include "PValues.h"

class PParticle: public TLorentzVector {
  
 public:
  PParticle(Int_t id=0, Double_t T=0., Double_t w=1.);
  // id, lab kinetic energy (GeV), weight

/*
  PParticle(const char *, Double_t T=0., Double_t w=1.);
  // name, lab kinetic energy (GeV), weight

  PParticle(Int_t, Double_t, Double_t, Double_t, Double_t m=0., Double_t w=1.);
  // id, Px, Py, Pz (GeV/c), mass (GeV/c**2) overrides default, weight

  PParticle(const char *, Double_t, Double_t, Double_t,Double_t m=0., Double_t w=1.);
  // name, Px, Py, Pz (GeV/c), mass (GeV/c**2) overrides default, weight

  PParticle(Int_t, const TVector3 &, Double_t m=0., Double_t w=1.);
  // id, 3-momentum vector (GeV/c), mass (GeV/c**2) overrides default, weight

  PParticle(Int_t, Double_t *, Double_t w=1.);
  // id, pointer to 4-dim array (Px, Py, Pz, E) (GeV/c, GeV), weight
  
  PParticle(Int_t, float *, Double_t w=1.);
  // id, pointer to 4-dim array (Px, Py, Pz, E) (GeV/c, GeV), weight

  PParticle(const PParticle &);
  // copy constructor

  PParticle(const PParticle *);
  // copy constructor
*/

  virtual ~PParticle() {
      if (qParticle1) delete qParticle1;
      if (qParticle2) delete qParticle2;
      if (values)  delete values;
  }

  Int_t ID() const { return pid; }

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }

  void SetVertex(Double_t x, Double_t y, Double_t z, Double_t t)
                                                 {fV.SetXYZ(x,y,z); fT=t;}

  inline Int_t GetParentIndex() const {return parentIndex;}
  inline Int_t GetDaughterIndex() const {return daughterIndex;}

  inline TVector3& GetVertex() {return fV;}
  inline TVector3& getVertex() {return fV;}

  inline void SetParentId(Int_t pId) {parentId = pId;}
  inline void SetSourceId(Int_t sId) {sourceId = sId;}
  inline void SetDaughterIndex(Int_t dInd) {daughterIndex = dInd;}
  inline void SetSiblingIndex(Int_t sInd) {siblingIndex = sInd;}
  inline void  SetParentIndex(Int_t pInd) {parentIndex = pInd;}
  inline void SetSibling(PParticle *p) {sParticle = p;}

  inline void ResetDaughters(void) {
    for (int i=0; i<(MAX_DAUGHTERS+1); i++)
      daughters[i]=nullptr;
  };

 protected:

  void defaults(void);

  Int_t pid;           //  particle code  (partially conforms with Geant 3)
  Int_t sourceId;      //  Source ID
  Int_t parentId;      //  parent ID
  Int_t parentIndex;   //  parent index in particle array
  Int_t decayModeIndex;//  decay Mode index (for decayAll option)
  Int_t destroyDecayModeIndex;// save only for data file
  Int_t daughterIndex; //  daughter index
  Int_t siblingIndex;  //  sibling index
  Int_t spectator;     //! flag that forces particle to be treated as spectator

  Double_t decayTime;  //  proper time until decay  (in mm/c)
  Double_t wt;         //  weight
  Double_t genwt;      //! generator weight
  Double_t invgenwt;   //! inverted generator weight
  Double_t mult;       //! multiplicity

  TVector3 fV;         //  creation vertex (in mm)
  Double_t fT;         //  creation time (in mm/c)
  Bool_t active;       //! internal activity flag
  Int_t index;         //! index in particle array
  PParticle* pParticle;//! pointer to particle object
  PParticle* qParticle1;//!
  PParticle* qParticle2;//!
  PParticle* sParticle; //! pointer to particle object
  PParticle* daughters[MAX_DAUGHTERS+1]; //!pointer to daughter array 
  TString debug;        //! debug string

  PValues * values;   //!pointer to value container
  Int_t status;       //! status of parent particle in PChannel::Decay

  Bool_t make_new_qParticle; //! Workaround

  ClassDef(PParticle,4)  // Pluto Particle Class
};
#endif // _PPARTICLE_H_













