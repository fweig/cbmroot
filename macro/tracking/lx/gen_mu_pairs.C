#include "TRandom3.h"
#include <fstream>
#include "TVector3.h"
#include "TMath.h"
#include "TRotation.h"
#include "TDatabasePDG.h"
#include <iostream>

TRandom3 gRandomGen;
Double_t M;
Double_t gMuonMass;
Double_t minTheta;
Double_t maxTheta;
Double_t thetaRange;
Double_t phi1;
Double_t theta1;
Double_t E;
Int_t firstPid;
int evNumber;
ofstream muonPairsFile;
TVector3 t1;
TVector3 yAxis1;

using namespace std;

double GetTheta()
{   
   return minTheta + thetaRange * gRandomGen.Rndm();
}

double GetPhi()
{
   return 2 * TMath::Pi() * gRandomGen.Rndm();
}

bool TryE1()
{
   Double_t E1;
   
   do
   {
      E1 = gRandomGen.Gaus(4.3, 1.3435);
   }
   while (E1 < 3.0 || E1 > E - 3.0);
   
   Double_t p1 = TMath::Sqrt(E1 * E1 - gMuonMass * gMuonMass);
   TVector3 pV1(t1);
   pV1.SetMag(p1);
   
   
   Double_t E2 = E - E1;
   Double_t p2 = TMath::Sqrt(E2 * E2 - gMuonMass * gMuonMass);
   Double_t cosTheta2L = (E1 * E2 + gMuonMass * gMuonMass - M * M / 2) / (p1 * p2);
   Double_t theta2L = TMath::ACos(cosTheta2L);
   
   for (int i = 0; i < 100; ++i)
   {
      Double_t phi2L = GetPhi();
      TVector3 pV2(t1);
      TRotation r;
      r.Rotate(phi2L, t1);
      TVector3 y2(yAxis1);
      y2.Transform(r);
      r.Rotate(theta2L, y2);
      pV2.Transform(r);
      pV2.SetMag(p2);   
      Double_t cosTheta2 = pV2.z() / pV2.Mag();
      Double_t theta2 = TMath::ACos(cosTheta2);
      
      if (theta2 > minTheta && theta2 < maxTheta)
      {
         muonPairsFile << 2 << " " << evNumber << " " << 0 << " " << 0 << " " << 0 << endl;
         muonPairsFile << firstPid << " " << pV1.x() << " " << pV1.y() << " " << pV1.z() << endl;
         muonPairsFile << -firstPid << " " << pV2.x() << " " << pV2.y() << " " << pV2.z() << endl;
         return true;
      }
   }
   
   return false;
}

bool TryE()
{
   do
   {
      E = gRandomGen.Gaus(8.6, 1.9);
   } while (E < 6.0 || E > 20.0);
   
   for (int i = 0; i < 100; ++i)
   {
      if (TryE1())
         return true;
   }
   
   return false;
}

bool TryTheta1()
{
   theta1 = GetTheta();
   TRotation r;
   r.RotateZ(phi1);
   TVector3 y(0, 1, 0);
   y.Transform(r);
   r.Rotate(theta1, y);
   t1 = TVector3(0, 0, 1);
   t1.Transform(r);
   yAxis1 = y;
   
   for (int i = 0; i < 100; ++i)
   {
      if (TryE())
         return true;
   }
   
   return false;
}

bool CreateMuonPair()
{
   phi1 = GetPhi();
   firstPid = (gRandomGen.Rndm() >= 0.5) ? -13 : 13;
   
   for (int i = 0; i < 100; ++i)
   {
      if (TryTheta1())
         return true;
   }
   
   return false;
}

void gen_mu_pairs(Int_t index)
{
   Double_t m = 0.1 + 0.01 * index;
   gMuonMass = TDatabasePDG::Instance()->GetParticle(13)->Mass();
   M = m;
   minTheta = TMath::ATan(0.1);
   maxTheta = 25 * TMath::Pi() / 180;
   thetaRange = maxTheta - minTheta;
   time_t initTime;
   gRandomGen.SetSeed(time(&initTime));
   
   ofstream invMEffFile("inv_m.txt", ios_base::out | ios_base::trunc);
   invMEffFile << m;
   
   int nofBadEvents = 0;
   muonPairsFile.open("muon_pairs.txt", ios_base::out | ios_base::trunc);
   
   for (evNumber = 1; evNumber <= 1000; ++evNumber)
   {
      bool result = CreateMuonPair();
      cout << "Event number: " << evNumber << " - " << (result ? "Good" : "Bad") << endl; 
   }
   
   cout << "Bad events: " << nofBadEvents << endl;
}
