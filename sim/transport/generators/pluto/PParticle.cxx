////////////////////////////////////////////////////////
//  Particle Class implementation file
//
//  A PParticle is a TLorentzVector with id and weight.
//  Static particle properties from the permanent data
//  base in PStaticData are used. Additional external particles
//  and decay modes may be loaded. See also PData.
//
//                    Author:  M.A. Kagarlis
//                    Written: 15.11.98
//                    Revised: 15.12.2000  R. Holzmann
//                    Revised: 22.03.2005  R. Holzmann
//                    Revised: 23.07.2007  IF (new framework)
//
////////////////////////////////////////////////////////

#include "PParticle.h"
#include "TF1.h"
#include "TMath.h"

void PParticle::defaults(void) {

  SetVertex(0., 0., 0., 0.);
  pParticle             = nullptr;
  qParticle1            = nullptr;
  qParticle2            = nullptr;
  index                 = -1;
  parentIndex           = -2;
  decayModeIndex        = -1;
  destroyDecayModeIndex = 0;
  decayTime             = 0.;
  debug                 = "";
  values                = nullptr;
  make_new_qParticle    = 1;
  SetSourceId(-1);
  SetParentId(-1);
  SetSiblingIndex(-1);
  SetDaughterIndex(-1);
  SetParentIndex(-1);
  SetSibling(nullptr);
  mult      = 1.;
  spectator = 0;
  genwt = invgenwt = 1.;

  ResetDaughters();
}

PParticle::PParticle(int /*id*/, Double_t /*T*/, Double_t /*w*/)
  : TLorentzVector() {
  defaults();
}

/*
PParticle::PParticle(const char * id, Double_t T, Double_t w):
    TLorentzVector(0,0,sqrt(T*T+2*T*makeStaticData()->GetParticleMass(id)),
		   T+makeStaticData()->GetParticleMass(makeStaticData()->IsParticleValid(id))),
    pid( makeStaticData()->IsParticleValid(id) ), wt( (pid)?w:0.), active(kTRUE) {
    // name, lab kinetic energy (GeV), weight
 defaults();
}


PParticle::PParticle(int id, Double_t px, Double_t py, Double_t pz, Double_t m, Double_t w):
    TLorentzVector(px,py,pz,sqrt(px*px+py*py+pz*pz+
				 ( (m>0.)?m*m:makeStaticData()->GetParticleMass(id)*
				   makeStaticData()->GetParticleMass(id) ) )),
    pid( makeStaticData()->IsParticleValid(id) ), wt( (pid)?w:0.), active(kTRUE) {
    // id, Px, Py, Pz (GeV/c), mass (GeV/c**2) overrides default, weight
 defaults();
}


PParticle::PParticle(const char * id, Double_t px, Double_t py, Double_t pz, Double_t m, Double_t w):
    TLorentzVector(px,py,pz,sqrt(px*px+py*py+pz*pz+
				 ( (m>0.)?m*m:makeStaticData()->GetParticleMass(id)*
				   makeStaticData()->GetParticleMass(id) ) )),
    pid( makeStaticData()->IsParticleValid(id) ), wt( (pid)?w:0.), active(kTRUE) {
    // name, Px, Py, Pz (GeV/c), mass (GeV/c**2) overrides default, weight
 defaults();
}


PParticle::PParticle(int id, const TVector3 & p, Double_t m, Double_t w):
    TLorentzVector( p, sqrt(p.Mag2()+( (m>0.)?m*m:makeStaticData()->GetParticleMass(id)*
				       makeStaticData()->GetParticleMass(id) ))),
    pid(makeStaticData()->IsParticleValid(id) ), wt( (pid)?w:0.), active(kTRUE) {
    // id, 3-momentum vector (GeV/c), mass (GeV/c**2) overrides default, weight
 defaults();
}


PParticle::PParticle(int id, Double_t * pt, Double_t w):
    TLorentzVector( pt ), pid( id ), wt( w ), active(kTRUE) {
    // id, pointer to 4-dim array (Px, Py, Pz, E) (GeV/c, GeV), weight
 defaults();
}

  
PParticle::PParticle(int id, float * pt, Double_t w):
    TLorentzVector( pt ), pid( id ), wt( w ), active(kTRUE) {
    // id, pointer to 4-dim array (Px, Py, Pz, E) (GeV/c, GeV), weight
 defaults();
}


PParticle::PParticle(const PParticle & p):
    TLorentzVector( p.Vect4() ), pid(p.ID() ),
    sourceId( p.GetSourceId() ),
    parentId( p.GetParentId() ),
    parentIndex( p.GetParentIndex() ),
    daughterIndex( p.GetDaughterIndex() ),
    siblingIndex( p.GetSiblingIndex() ),
    decayTime( p.GetProperTime() ), 
    wt( p.W() ), active( p.IsActive() ) {
    // copy constructor
    SetVertex(p.X(),p.Y(),p.Z(),p.T());
    pParticle = nullptr;
    qParticle1= nullptr;
    qParticle2= nullptr;

    make_new_qParticle=p.make_new_qParticle;
    if (p.make_new_qParticle) {
	qParticle1 = (p.qParticle1 ? 
		      new PParticle(p.qParticle1) : nullptr); //Copy, because it will destroy in dtor
    } 

    if (p.make_new_qParticle) {
	qParticle2 = (p.qParticle2 ?
		      new PParticle(p.qParticle2) : nullptr);
    }

    sParticle  = p.sParticle;
    //      if (p.debug) 
// 	  debug=p.debug;
//       else
    debug="";
    values=nullptr;
    destroyDecayModeIndex= p.destroyDecayModeIndex;
    decayModeIndex= p.decayModeIndex;

    if (p.values) values=new PValues(*(p.values));
    mult=p.mult;
    spectator=p.spectator;
    genwt=p.genwt;
    invgenwt=p.invgenwt;
}


PParticle::PParticle(const PParticle * p):
    TLorentzVector( p->Vect4() ), pid( p->ID() ), 
    sourceId( p->GetSourceId() ),
    parentId( p->GetParentId() ),
    parentIndex( p->GetParentIndex() ),
    daughterIndex( p->GetDaughterIndex() ),
    siblingIndex( p->GetSiblingIndex() ),
    decayTime( p->GetProperTime() ), 
    wt( p->W() ), active( p->IsActive() ) {
    // copy constructor
    SetVertex(p->X(),p->Y(),p->Z(),p->T());
    pParticle = nullptr;
    qParticle1= nullptr;
    qParticle2= nullptr;

    make_new_qParticle=p->make_new_qParticle;
    if (p->make_new_qParticle) {
	qParticle1 = (p->qParticle1 ?
		      new PParticle(p->qParticle1): nullptr); //Copy, because it will destroy in dtor
    } 

    if (p->make_new_qParticle) {
	qParticle2 = (p->qParticle2 ?
		      new PParticle(p->qParticle2): nullptr);
    }

    sParticle  = p->sParticle;
 
    //      if (p->debug) debug=p->debug;
//       else 
    debug="";
    values=nullptr;
    destroyDecayModeIndex= p->destroyDecayModeIndex;
    decayModeIndex= p->decayModeIndex;

    if (p->values) values=new PValues(*(p->values));

    mult=p->mult;
    spectator=p->spectator;
    genwt=p->genwt;
    invgenwt=p->invgenwt;
}

*/


ClassImp(PParticle)
