#include "CbmTrdModuleRecT.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdFASP.h"
#include "CbmTrdHit.h"
#include "CbmTrdParModDigi.h"

#include <FairLogger.h>

#include <TGeoPhysicalNode.h>

#include <TClonesArray.h>
#include <TF1.h>
#include <TGraphErrors.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

Double_t CbmTrdModuleRecT::fgDT[]      = {4.181e-6, 1586, 24};
TGraphErrors* CbmTrdModuleRecT::fgEdep = NULL;
TGraphErrors* CbmTrdModuleRecT::fgT    = NULL;
TF1* CbmTrdModuleRecT::fgPRF           = NULL;
//_______________________________________________________________________________
CbmTrdModuleRecT::CbmTrdModuleRecT()
  : CbmTrdModuleRec()
  , fConfigMap(0)
  , fT0(0)
  , fBuffer()
  , vs()
  , vse()
  , vt()
  , vx()
  , vxe() {}

//_______________________________________________________________________________
CbmTrdModuleRecT::CbmTrdModuleRecT(Int_t mod, Int_t ly, Int_t rot)
  : CbmTrdModuleRec(mod, ly, rot)
  , fConfigMap(0)
  , fT0(0)
  , fBuffer()
  , vs()
  , vse()
  , vt()
  , vx()
  , vxe() {
  //printf("AddModuleT @ %d\n", mod); Config(1,0);
  SetNameTitle(Form("TrdRecT%d", mod),
               "Reconstructor for triangular read-out.");
}

//_______________________________________________________________________________
CbmTrdModuleRecT::~CbmTrdModuleRecT() {}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRecT::AddDigi(const CbmTrdDigi* d, Int_t id) {
  /* Add digi to cluster fragments. At first clusters are ordered on pad rows and time. 
 * No channel ordering is assumed. The time condition for a digi to enter a cluster 
 * chunk is to have abs(dt)<5 wrt cluster t0 
 */

  if (CWRITE()) cout << "add @" << id << " " << d->ToString();

  Int_t ch = d->GetAddressChannel(), col, row = GetPadRowCol(ch, col), dtime;
  Double_t t, r = d->GetCharge(t, dtime);
  Int_t tm = d->GetTimeDAQ() - fT0, terminator(0);
  if (dtime < 0)
    tm += dtime;  // correct for the difference between tilt and rect
  if (r < 1)
    terminator = 1;
  else if (t < 1)
    terminator = -1;

  if (CWRITE())
    printf(
      "row[%2d] col[%2d] tm[%2d] terminator[%d]\n", row, col, tm, terminator);
  CbmTrdCluster* cl(NULL);

  // get the link to saved clusters
  std::map<Int_t, std::list<CbmTrdCluster*>>::iterator it = fBuffer.find(row);
  // check for saved
  if (it != fBuffer.end()) {
    Bool_t kINSERT(kFALSE);
    std::list<CbmTrdCluster*>::iterator itc = fBuffer[row].begin();
    for (; itc != fBuffer[row].end(); itc++) {
      if (CWRITE()) cout << (*itc)->ToString();

      UShort_t tc = (*itc)->GetStartTime();
      Int_t dt    = tc - tm;

      if (dt < -5)
        continue;
      else if (dt < 5) {
        if (CWRITE()) printf("match time dt=%d\n", dt);
        if ((*itc)->IsChannelInRange(ch) == 0) {
          if (!(*itc)->AddDigi(id, ch, terminator, dt)) break;
          kINSERT = kTRUE;
          if (CWRITE()) cout << "          => Cl (Ad) " << (*itc)->ToString();
          break;
        }
      } else {
        if (CWRITE()) printf("break for time dt=%d\n", dt);
        break;
      }
    }

    if (!kINSERT) {
      if (itc != fBuffer[row].end() && itc != fBuffer[row].begin()) {
        itc--;
        fBuffer[row].insert(
          itc, cl = new CbmTrdCluster(fModAddress, id, ch, row, tm));
        if (CWRITE()) cout << "          => Cl (I) " << cl->ToString();
      } else {
        fBuffer[row].push_back(
          cl = new CbmTrdCluster(fModAddress, id, ch, row, tm));
        if (CWRITE()) cout << "          => Cl (Pb) " << cl->ToString();
      }
      cl->SetTrianglePads();
      if (terminator < 0)
        cl->SetProfileStart();
      else if (terminator > 0)
        cl->SetProfileStop();
    }
  } else {
    fBuffer[row].push_back(cl =
                             new CbmTrdCluster(fModAddress, id, ch, row, tm));
    cl->SetTrianglePads();
    if (terminator < 0)
      cl->SetProfileStart();
    else if (terminator > 0)
      cl->SetProfileStop();
    if (CWRITE()) cout << "          => Cl (Nw) " << cl->ToString();
  }

  return kTRUE;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRecT::GetOverThreshold() const {
  Int_t nch(0);
  for (std::map<Int_t, std::list<CbmTrdCluster*>>::const_iterator ir =
         fBuffer.cbegin();
       ir != fBuffer.cend();
       ir++) {
    for (std::list<CbmTrdCluster*>::const_iterator itc = (*ir).second.cbegin();
         itc != (*ir).second.cend();
         itc++)
      nch += (*itc)->GetNCols();
  }
  return nch;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRecT::FindClusters() {
  CbmTrdCluster* cl(NULL);

  // get the link to saved clusters
  Int_t ncl(0);
  std::list<CbmTrdCluster*>::iterator itc0, itc1;
  for (std::map<Int_t, std::list<CbmTrdCluster*>>::iterator ir =
         fBuffer.begin();
       ir != fBuffer.end();
       ir++) {
    itc0 = (*ir).second.begin();
    while ((*ir).second.size() > 1
           && itc0 != (*ir).second.end()) {  // try merge clusters
      itc1 = itc0;
      itc1++;

      Bool_t kMERGE = kFALSE;
      while (itc1 != (*ir).second.end()) {
        if (CWRITE())
          cout << "  base cl[0] : " << (*itc0)->ToString()
               << "     + cl[1] : " << (*itc1)->ToString();
        if ((*itc0)->Merge((*itc1))) {
          if (CWRITE()) cout << "  SUM        : " << (*itc0)->ToString();
          kMERGE = kTRUE;
          delete (*itc1);
          itc1 = (*ir).second.erase(itc1);
          if (itc1 == (*ir).second.end()) break;
        } else
          itc1++;
      }
      if (!kMERGE) itc0++;
    }

    for (itc0 = (*ir).second.begin(); itc0 != (*ir).second.end(); itc0++) {
      cl = (*itc0);
      cl = new ((*fClusters)[ncl++]) CbmTrdCluster(*cl);
      cl->SetTrianglePads();
      delete (*itc0);
    }
  }
  fBuffer.clear();

  //printf("fClusters[%p] nCl[%d]\n", (void*)fClusters, fClusters->GetEntriesFast());
  //LOG(info) << GetName() << "::FindClusters : " << ncl;
  return ncl;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRecT::MakeHits() { return kTRUE; }

//_______________________________________________________________________________
Bool_t CbmTrdModuleRecT::Finalize() {
  /*  Steering routine for classifying hits and apply further analysis
 * -> hit deconvolution (see Deconvolute)
 * -> hit merging for row-cross (see RowCross)
 */

  //   Int_t nhits=fHits->GetEntriesFast();
  //   //if(CWRITE())
  //     LOG(info) << "\n"<<GetName()<<"::Finalize("<<nhits<<")";
  //   CbmTrdHit *hit(NULL), *hitp(NULL);
  //   for(Int_t ih(0); ih<nhits; ih++){
  //     hit = (CbmTrdHit*)((*fHits)[ih]);
  //     for(Int_t jh(ih+1); jh<nhits; jh++){
  //       hitp = (CbmTrdHit*)((*fHits)[jh]);
  //       //if(CWRITE())
  //       cout<<ih<<" "<<hit->ToString();
  //       cout<<"-> "<<jh<<"  "<<hitp->ToString();
  //     }
  //   }
  return kTRUE;
}

#include <TCanvas.h>
#include <TH1.h>
#define NANODE 9
//_______________________________________________________________________________
CbmTrdHit* CbmTrdModuleRecT::MakeHit(Int_t ic,
                                     const CbmTrdCluster* cl,
                                     std::vector<const CbmTrdDigi*>* digis) {
  if (!fgEdep) {  // first use initialization of PRF helppers
    LOG(info) << GetName() << "::MakeHit: Init static helpers. ";
    fgEdep = new TGraphErrors;
    fgEdep->SetLineColor(kBlue);
    fgEdep->SetLineWidth(2);
    fgT = new TGraphErrors;
    fgT->SetLineColor(kBlue);
    fgT->SetLineWidth(2);
    fgPRF = new TF1("prf", "[0]*exp(-0.5*((x-[1])/[2])**2)", -10, 10);
    fgPRF->SetLineColor(kRed);
    fgPRF->SetParNames("E", "x", "prf");
  }
  TH1* hf(NULL);
  TCanvas* cvs(NULL);
  if (CDRAW()) {
    cvs = new TCanvas("c", "TRD Anode Hypothesis", 10, 600, 1000, 500);
    cvs->Divide(2, 1, 1.e-5, 1.e-5);
    TVirtualPad* vp = cvs->cd(1);
    vp->SetLeftMargin(0.06904908);
    vp->SetRightMargin(0.009969325);
    vp->SetTopMargin(0.01402806);
    vp = cvs->cd(2);
    vp->SetLeftMargin(0.06904908);
    vp->SetRightMargin(0.009969325);
    vp->SetTopMargin(0.01402806);
    hf = new TH1I("hf", ";x [pw];s [ADC chs]", 100, -3, 3);
    hf->GetYaxis()->SetRangeUser(-50, 4500);
    hf->Draw("p");
  }

  if (CWRITE()) cout << cl->ToString();

  // check cluster integrity and do digi merging if needed
  vector<Bool_t> vmask(digis->size(), 0);  // masks in case of merged Digis
  vector<CbmTrdDigi*> vdgM;
  if (cl->GetNCols() != digis->size() && !MergeDigis(digis, &vdgM, &vmask)) {
    cout << cl->ToString();
    for (vector<const CbmTrdDigi*>::iterator i = digis->begin();
         i != digis->end();
         i++)
      cout << (*i)->ToString();
    return NULL;
  }

  // Read in all digis information()
  ULong64_t t0 = fT0 + cl->GetStartTime();  // absolute hit time (prompt signal)
  Int_t n0(0), ovf(0), cM;
  if (!(n0 = LoadDigis(digis, &vdgM, &vmask, t0, cM))) {
    cout << cl->ToString();
    for (vector<const CbmTrdDigi*>::iterator i = digis->begin();
         i != digis->end();
         i++)
      cout << (*i)->ToString();
    return NULL;
  }
  if (n0 < 0) {
    ovf = 1;
    n0 *= -1;
  }

  // analyse digis topology; no of signal types, maximum, etc
  Bool_t tM(kTRUE);  // maximum type tilt=1; rect=0
  Int_t nL(0);       // signal index for the max signal
  Int_t col, row = GetPadRowCol(cl->GetStartCh(), col);
  Double_t max(0.),  // maximum signal
    LS(0.),          // left side sum of signals
    S(0.);           // sum of signals
  Int_t nr(0), nt(0);
  for (Int_t is(1); is <= n0; is++) {
    if (vxe[is] > 0) {  // select tilted coupling
      nt++;
      S += vs[is];
      if (vs[is] > max) {
        max = vs[is];
        tM  = kTRUE;
        nL  = is;
        LS += vs[is];
      }
    } else {  // select rectangular coupling
      nr++;
      S += vs[is];
      if (vs[is] > max) {
        max = vs[is];
        tM  = kFALSE;
        nL  = is;
        LS += vs[is];
      }
    }
  }
  col += cM;
  S -= LS;
  LS -= max;
  // evaluate asymmetry
  Int_t lr(0),        // max signal left-right asymmetry wrt central pad
    tr(0),            // left-right innequality for symmetric clusters
    nR(n0 + 1 - nL);  // no of signals to the right of maximum
  if (nL < nR)
    lr = 1;
  else if (nL > nR)
    lr = -1;
  if (!lr && (n0 % 2)) tr = (LS < S ? -1 : 1);
  // compute x and y offset from center pad
  Double_t dx(0.), dy(0.), edx(0.21650635),
    edy(0.77942286);  // fixed error parametrization
  switch (n0) {
    case 1:
      if (nt) {
        dx = -0.5;
        dy = 0;
      }  // T
      else {
        dx = 0.5;
        dy = 0;
      }  // R
      break;
    case 2:
      if (cl->HasOpenStart() && cl->HasOpenStop()) {
        dx = cM ? -1. : 0.;
        dy = -0.5;
      }  // RT
      else {
        dx = 0.;
        dy = 0.5;
      }  // TR
      break;
    case 3:
      if (tM && lr) {
        dx = cM ? -1. : 0.;
        dy = GetYoffset(n0);
      }  // TRT asymm
      else if (!tM && !lr) {
        dx = 0.;
        dy = GetYoffset(n0);
      }  // TRT symm
      else if (tM && !lr) {
        dx = GetXoffset(n0);
        dy = 0.;
      }  // RTR symm
      else if (!tM && lr) {
        dx = GetXoffset(n0);
        dy = cM ? 0.5 : -0.5;
      }  // RTR asymm
      break;
    default:
      dx = GetXoffset(n0);
      dy = GetYoffset(n0);
      break;
  }
  if (dx < -0.5
      && cM > 0) {  // shift graph representation to fit dx[pw] in [-0.5, 0.5]
    dx += 1.;
    col -= 1;
    for (UInt_t idx(0); idx < vx.size(); idx++)
      vx[idx] += 1;
  }
  if (dx > 0.5) {  // dirty fix for compound clusters TODO
    Int_t ishift = Int_t(dx - 0.5) + 1;
    dx -= ishift;
    col += ishift;
    for (UInt_t idx(0); idx < vx.size(); idx++)
      vx[idx] -= ishift;
  }
  dy = dx - dy;  // only on natural scalling !
  // go to cm scale
  dx *= fDigiPar->GetPadSizeX(0);
  dy *= fDigiPar->GetPadSizeY(0);

  // apply systematic correction for x (MC derived)
  Int_t typ = 0;                                    // [0] center hit type
                                                    // [1] side hit type
  if ((n0 == 5 && ((tM && !lr) || (!tM && lr))) ||  // TRTRT symm/asymm
      n0 == 4 || (n0 == 3 && ((tM && !lr) || (!tM && lr != 0))))
    typ = 1;  // RTR symm/asymm
  Double_t xcorr(0.);
  Int_t nbins((NBINSCORRX - 1) >> 1), ii = nbins + TMath::Nint(dx / fgCorrXdx),
                                      i0, i1;
  if (ii < 0 || ii >= NBINSCORRX)
    LOG(warn) << GetName() << "::MakeHit : Idx " << ii
              << " outside range for displacement " << dx << ".";
  else {
    if (dx < fgCorrXdx * ii) {
      i0 = TMath::Max(0, ii - 1);
      i1 = ii;
    } else {
      i0 = ii;
      i1 = TMath::Min(NBINSCORRX - 1, ii + 1);
    }
    Double_t DDx = (fgCorrXval[typ][i1] - fgCorrXval[typ][i0]),
             a = DDx / fgCorrXdx, b = fgCorrXval[typ][i0] - DDx * (i0 - nbins);
    xcorr = 0.1 * (b + a * dx);
  }
  dx += xcorr;
  dy += xcorr;
  if (dx > 0.5 * fDigiPar->GetPadSizeX(0))
    dx = 0.5 * fDigiPar->GetPadSizeX(0);
  else if (dx < -0.5 * fDigiPar->GetPadSizeX(0))
    dx = -0.5 * fDigiPar->GetPadSizeX(0);

  if (dy > 0.5 * fDigiPar->GetPadSizeY(0)) {  //
    //printf("BEFORE dy[%+6.4f] dx[%+6.4f] {n[%d] max[%c] lr[%+d]}\n", dy, dx, n0, tM?'T':'R', lr);
    dy -= fDigiPar->GetPadSizeY(0);
  }
  if (dy < -0.5 * fDigiPar->GetPadSizeY(0)) {  //
    //printf("(BEFORE) dy[%+6.4f] dx[%+6.4f] {n[%d] max[%c] lr[%+d]}\n", dy, dx, n0, tM?'T':'R', lr);
    dy += fDigiPar->GetPadSizeY(0);
  }

  // process y offset
  // apply systematic correction for y (MC derived)
  Float_t fdy(1.), yoff(0.);
  switch (n0) {
    case 3:
      fdy  = fgCorrYval[0][0];
      yoff = fgCorrYval[0][1];
      if (tM && !lr)
        dy -= tr * 0.5 * fDigiPar->GetPadSizeY(0);
      else if (lr)
        dy -= 0.5 * fDigiPar->GetPadSizeY(0);
      ;
      break;
    case 4:
      fdy  = fgCorrYval[1][0];
      yoff = fgCorrYval[1][1];
      if ((!tM && lr == 1) || (tM && lr == -1)) yoff *= -1;
      break;
    case 5:
    case 7:
    case 9:
    case 11:
      fdy  = fgCorrYval[2][0];
      yoff = fgCorrYval[2][1];
      break;
    case 6:
    case 8:
    case 10:
      fdy  = fgCorrYval[3][0];
      yoff = fgCorrYval[3][1];
      if ((!tM && lr == 1) || (tM && lr == -1)) yoff *= -1;
      break;
  }
  dy *= fdy;
  dy += yoff;
  if (dy > 0.5 * fDigiPar->GetPadSizeY(0))
    dy = 0.5 * fDigiPar->GetPadSizeY(0);
  else if (dy < -0.5 * fDigiPar->GetPadSizeY(0))
    dy = -0.5 * fDigiPar->GetPadSizeY(0);

  // get anode wire offset
  Int_t ia(0);
  Float_t ya;  //  anode position in local pad coordinates
  for (; ia <= NANODE; ia++) {
    ya = -1.35 + ia * 0.3;
    if (dy > ya + 0.15) continue;
    break;
  }

  // Error parametrization X : parabolic model on cl size
  Double_t parX[] = {0.713724, -0.318667, 0.0366036};
  Int_t nex       = TMath::Min(n0, 7);
  edx             = parX[0] + parX[1] * nex + parX[2] * nex * nex;
  Double_t parY[] = {0.0886413, 0., 0.0435141};
  edy             = parY[0] + parY[2] * dy * dy;

  if (CWRITE()) {
    printf("row[%2d] col[%2d] sz[%d%c] M[%d%c] dx[mm]=%6.3f dy[mm]=%6.3f "
           "t0[clk]=%llu OVF[%c]\n",
           row,
           col,
           n0,
           (lr ? (lr < 0 ? 'L' : 'R') : 'C'),
           cM,
           (tM ? 'T' : 'R'),
           10 * dx,
           10 * dy,
           t0,
           (ovf ? 'y' : 'n'));
    for (UInt_t idx(0); idx < vs.size(); idx++) {
      printf("%2d%cdt[%2d] s[ADC]=%6.1f+-%6.1f x[pw]=%5.2f+-%5.2f\n",
             idx,
             (UInt_t(nL) == idx ? '*' : ' '),
             vt[idx],
             vs[idx],
             vse[idx],
             vx[idx],
             vxe[idx]);
    }
  }

  // compute energy
  for (UInt_t idx(0); idx < vs.size(); idx++) {
    if (vxe[idx] > 0) {
      fgEdep->SetPoint(idx, vx[idx] + dy / fDigiPar->GetPadSizeY(0), vs[idx]);
      fgEdep->SetPointError(idx, vxe[idx], vse[idx]);
    } else {
      fgEdep->SetPoint(idx, vx[idx], vs[idx]);
      fgEdep->SetPointError(idx, vxe[idx], vse[idx]);
    }
  }
  Double_t xc = vx[n0 + 2];
  for (Int_t ip(vs.size()); ip < fgEdep->GetN(); ip++) {
    //fgEdep->RemovePoint(ip);
    xc += 0.5;
    fgEdep->SetPoint(ip, xc, 0);
    fgEdep->SetPointError(ip, 0., 300);
  }
  if (CWRITE()) fgEdep->Print();

  Double_t e(0.), chi(100), xlo(*vx.begin()), xhi(*vx.rbegin());
  fgPRF->SetParameter(0, max);
  fgPRF->FixParameter(1, dx / fDigiPar->GetPadSizeX(0));
  fgPRF->SetParameter(2, 0.65);
  fgPRF->SetParLimits(2, 0.45, 10.5);
  fgEdep->Fit(fgPRF, "QBN", "goff", xlo - 0.5, xhi + 0.5);
  if (!fgPRF->GetNDF()) return NULL;
  chi = fgPRF->GetChisquare() / fgPRF->GetNDF();
  e   = fgPRF->Integral(xlo - 0.5, xhi + 0.5);

  // apply MC correction
  Float_t gain0 = 210.21387;  //(XeCO2 @ 1900V)
  //   Float_t grel[3] = {1., 0.98547803, 0.93164071},
  //           goff[3][3] = {
  //             {0.05714, -0.09, -0.09},
  //             {0., -0.14742, -0.14742},
  //             {0., -0.29, -0.393}
  //           };
  //   Int_t ian=0;
  //   if(TMath::Abs(dy)<=0.3) ian=0;
  //   else if(TMath::Abs(dy)<=0.6) ian=1;
  //   else if(TMath::Abs(dy)<=0.9) ian=2;
  //   Int_t isize=0;
  //   if(n0<=3) isize=0;
  //   else if(n0<=4) isize=1;
  //   else isize=2;
  Float_t gain = gain0;  //*grel[ian];
  e /= gain;             // apply effective gain
  //e+=goff[ian][isize];  // apply missing energy offset

  TVector3 local_pad, local_pad_err;
  Int_t srow, sector = fDigiPar->GetSectorRow(row, srow);
  fDigiPar->GetPadPosition(sector, col, srow, local_pad, local_pad_err);
  //printf("r[%2d] c[%2d] max[%d] lr[%d] n0[%d] cM[%d] nM[%d] dx[%7.4f] dy[%7.4f] loc[%6.2f %6.2f %6.2f] err[%6.2f %6.2f %6.2f] e[%f] chi[%f]\n", row, col, mtyp, lr, n0, cM, nM, dx, dy, local_pad[0], local_pad[1], local_pad[2], local_pad_err[0], local_pad_err[1], local_pad_err[2], e, chi);
  Double_t local[3] = {local_pad[0] + dx, local_pad[1] + dy, local_pad[2]},
           global[3], globalErr[3] = {edx, edy, 0.};
  LocalToMaster(local, global);

  // process time profile
  for (Int_t idx(1); idx <= n0; idx++) {
    Double_t dtFEE = fgDT[0] * (vs[idx] - fgDT[1]) * (vs[idx] - fgDT[1])
                     / CbmTrdDigi::Clk(CbmTrdDigi::kFASP);
    if (vxe[idx] > 0) vx[idx] += dy / fDigiPar->GetPadSizeY(0);
    fgT->SetPoint(idx - 1, vx[idx], vt[idx] - dtFEE);
  }
  xc = vx[n0 + 2];
  for (Int_t ip(n0); ip < fgT->GetN(); ip++) {
    fgT->SetPoint(ip, xc, 0);
    xc += 0.5;
  }

  Double_t time(-21.),
    edt(26.33),   // should be parametrized as function of da TODO
    tdrift(100);  // should depend on Ua
  if (n0 > 1 && (fgT->Fit("pol1", "QC", "goff") == 0)) {
    TF1* f = fgT->GetFunction("pol1");
    time   = f->GetParameter(0) - fgDT[2];
    if (TMath::IsNaN(time)) time = -21;
    //dtime += TMath::Abs(f->GetParameter(1)*(vx[n0+1] - vx[1]));
  }

  if (CDRAW()) {
    Double_t rangex(vx[0] - 0.25), rangeX(vx[n0 + 2] + 0.25);
    cvs->cd(1);
    hf->Draw("p");
    hf->GetXaxis()->SetRangeUser(rangex, rangeX);
    hf->SetTitle(Form(
      "%d[%d] row[%d] col[%2d] an[%+d] m[%+4.2f] s[%4.2f] E[%7.2f] chi2[%7.2f]",
      ic,
      Int_t(vs.size()),
      row,
      col,
      ia,
      fgPRF->GetParameter(1),
      fgPRF->GetParameter(2),
      e,
      chi));
    hf->GetXaxis()->SetRangeUser(rangex, rangeX);
    hf->GetYaxis()->SetRangeUser(-100., 4500);
    fgEdep->Draw("pl");
    fgPRF->Draw("same");

    cvs->cd(2);
    hf = (TH1*) hf->DrawClone("p");
    hf->GetXaxis()->SetRangeUser(rangex, rangeX);
    hf->GetYaxis()->SetRangeUser(-10, 20);
    //hf->SetTitle(Form("%d row[%d] col[%2d] an[%+d] m[%+4.2f] s[%4.2f] E[%7.2f] chi2[%7.2f]", ic, row, col, ia, fgPRF->GetParameter(1), fgPRF->GetParameter(2), fgPRF->Integral(xlo, xhi), fgPRF->GetChisquare()/fgPRF->GetNDF()));
    //       hf->GetXaxis()->SetRangeUser(xlo-0.25, xhi+0.25);
    //       //hf->GetYaxis()->SetRangeUser(0., 4500);
    fgT->Draw("pl");
    cvs->Modified();
    cvs->Update();
    cvs->SaveAs(Form("cl_%02d_A%d.gif", ic, ia));
  }

  Int_t nofHits  = fHits->GetEntriesFast();
  CbmTrdHit* hit = new ((*fHits)[nofHits])
    CbmTrdHit(fModAddress,
              global,
              globalErr,
              0.,  // sxy chi,
              ic,
              e,  // energy
              CbmTrdDigi::Clk(CbmTrdDigi::kFASP) * (t0 + time) - tdrift + 30.29,
              edt);
  hit->SetClassType();
  hit->SetMaxType(tM);
  if (ovf) hit->SetOverFlow();
  if (CWRITE()) cout << hit->ToString();
  return hit;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRecT::GetXoffset(Int_t n0) const {
  Double_t dx(0.), R(0.);
  for (Int_t ir(1); ir <= n0; ir++) {
    if (vxe[ir] > 0) continue;  // select rectangular coupling
    R += vs[ir];
    dx += vs[ir] * vx[ir];
  }
  if (TMath::Abs(R) > 0) return dx / R;
  LOG(warn) << GetName() << "::GetDx : Unexpected null sum.";
  return 0.;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRecT::GetYoffset(Int_t n0) const {
  Double_t dy(0.), T(0.);
  for (Int_t it(1); it <= n0; it++) {
    if (vxe[it] > 0) {  // select tilted coupling
      T += vs[it];
      dy += vs[it] * vx[it];
    }
  }
  if (TMath::Abs(T) > 0) return dy / T;
  LOG(warn) << GetName() << "::GetDy : Unexpected null sum.";
  return 0.;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRecT::LoadDigis(vector<const CbmTrdDigi*>* digis,
                                  vector<CbmTrdDigi*>* vdgM,
                                  vector<Bool_t>* vmask,
                                  ULong64_t& t0,
                                  Int_t& cM) {
  /* Load digis information in working vectors. 
 * The digis as provided by the digitizer are replaced by the merged one 
 * according to the vmask map. Digis are represented in the normal coordinate system of
 * (pad width [pw], DAQ time [clk], signal [ADC chs]) with rectangular signals at integer 
 * positions.  
 */
  vs.clear();
  vse.clear();
  vx.clear();
  vxe.clear();
  vt.clear();

  cM = 0;       // relative position of maximum signal
  Int_t n0(0),  // number of measured signals
    ovf(1),     // over-flow flag for at least one of the digis
    dt;
  Char_t ddt,            // signal time offset wrt prompt
    dt0(0);              // cluster time offset wrt arbitrary t0
  Double_t r, re(100.),  // rect signal
    t, te(100.),         // tilt signal
    err,                 // final error parametrization for signal
    xc(-0.5),            // running signal-pad position
    max(0.);             // max signal
  Int_t j(0), col0(-1), col1(0);
  const CbmTrdDigi* dg(NULL);
  vector<CbmTrdDigi*>::iterator idgM = vdgM->begin();
  for (vector<const CbmTrdDigi*>::iterator i = digis->begin();
       i != digis->end();
       i++, j++) {
    dg = (*i);
    if ((*vmask)[j]) {
      dg = (*idgM);
      idgM++;
    }
    if (CWRITE()) cout << dg->ToString();
    r = dg->GetCharge(t, dt);
    if (t0 == 0)
      t0 = dg->GetTimeDAQ();  // set arbitrary t0 to avoid double digis loop
    if (col0 < 0) GetPadRowCol(dg->GetAddressChannel(), col0);  //  initialilze
    ddt = dg->GetTimeDAQ() - t0;
    if (ddt < dt0) dt0 = ddt;

    // check column wise organization
    GetPadRowCol(dg->GetAddressChannel(), col1);
    if (col0 + j != col1) {
      LOG(error) << GetName()
                 << "::LoadDigis : digis in cluster not in increasing order !";
      return 0;
    }

    // process tilt signal
    if (t > 0) {
      err = te;
      if (t > 4094) {
        ovf = -1;
        err = 150.;
      }
      t -= CbmTrdFASP::GetBaselineCorr();
      n0++;
      if (t > max) {
        max = t;
        cM  = j;
      }
    } else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(t);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.035);
    xc += 0.5;

    // process rect signal
    ddt += dt;
    if (ddt < dt0) dt0 = ddt;
    if (r > 0) {
      err = re;
      if (r > 4094) {
        ovf = -1;
        err = 150.;
      }
      r -= CbmTrdFASP::GetBaselineCorr();
      n0++;
      if (r > max) {
        max = r;
        cM  = j;
      }
    } else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(r);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.);
    xc += 0.5;
  }

  // remove merged digis if they were created
  if (idgM != vdgM->end())
    LOG(warn) << GetName()
              << "::LoadDigis : not all merged digis have been consumed !";
  for (idgM = vdgM->begin(); idgM != vdgM->end(); idgM++)
    delete (*idgM);

  // add front and back anchor points if needed
  if (TMath::Abs(vs[0]) > 1.e-3) {
    xc  = vx[0];
    ddt = vt[0];
    vs.insert(vs.begin(), 0);
    vse.insert(vse.begin(), 300);
    vt.insert(vt.begin(), ddt);
    vx.insert(vx.begin(), xc - 0.5);
    vxe.insert(vxe.begin(), 0);
  }
  Int_t n(vs.size() - 1);
  if (TMath::Abs(vs[n]) > 1.e-3) {
    xc  = vx[n] + 0.5;
    ddt = vt[n];
    vs.push_back(0);
    vse.push_back(300);
    vt.push_back(ddt);
    vx.push_back(xc);
    vxe.push_back(0.035);
  }
  // recenter time and space profile
  t0 += dt0;
  for (UInt_t idx(0); idx < vx.size(); idx++) {
    vt[idx] -= dt0;
    vx[idx] -= cM;
  }
  return ovf * n0;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRecT::MergeDigis(vector<const CbmTrdDigi*>* digis,
                                    vector<CbmTrdDigi*>* vdgM,
                                    vector<Bool_t>* vmask) {
  /* Merge digis in the cluster if their topology within it allows it although cuts in the 
 * digi merger procedure (CbmTrdFASP::WriteDigi()) were not fulfilled. 
 * Normally this are boundary signals with large time delays wrt neighbors
 */

  CbmTrdDigi* dgM(NULL);
  if (digis->size() < 2) {  // sanity check ... just in case
    LOG(warn) << GetName() << "::MergeDigis : Bad digi config for cluster :";
    return kFALSE;
  }

  Double_t r, t;
  Int_t colR, colT, dt, contor(0);
  Bool_t kFOUND(0);
  for (vector<const CbmTrdDigi*>::iterator idig = digis->begin(),
                                           jdig = idig + 1;
       jdig != digis->end();
       idig++, jdig++, contor++) {
    const CbmTrdDigi* dgT((*idig));  // tilt signal digi
    const CbmTrdDigi* dgR((*jdig));  // rect signal digi
    GetPadRowCol(dgR->GetAddressChannel(), colR);
    GetPadRowCol(dgT->GetAddressChannel(), colT);

    if (colR != colT) continue;

    dgM = new CbmTrdDigi(*dgT);
    r   = dgR->GetCharge(t, dt);
    dgT->GetCharge(t, dt);
    dt = dgR->GetTimeDAQ() - dgT->GetTimeDAQ();
    dgM->SetCharge(t, r, dt);
    Int_t rtrg(dgR->GetTriggerType() & 2), ttrg(dgT->GetTriggerType() & 1);
    dgM->SetTriggerType(rtrg | ttrg);  //merge the triggers
    if (CWRITE()) {
      cout << "MERGE" << endl;
      cout << dgT->ToString();
      cout << dgR->ToString();
      cout << "TO" << endl;
      cout << dgM->ToString();
      cout << "..." << endl;
    }
    kFOUND = 1;

    vdgM->push_back(dgM);
    (*vmask)[contor] = 1;
    jdig             = digis->erase(jdig);
    if (jdig == digis->end()) break;
  }

  if (!kFOUND) {
    LOG(warn) << GetName()
              << "::MergeDigis : Digi to pads matching failed for cluster :";
    return kFALSE;
  }
  return kTRUE;
}

Float_t CbmTrdModuleRecT::fgCorrXdx                 = 0.005;
Float_t CbmTrdModuleRecT::fgCorrXval[2][NBINSCORRX] = {
  {0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  -0.144, -0.091, -0.134, -0.185, -0.120, -0.115,
   -0.125, -0.125, -0.124, -0.124, -0.122, -0.120, -0.119, -0.116, -0.114,
   -0.113, -0.111, -0.109, -0.107, -0.105, -0.102, -0.100, -0.098, -0.097,
   -0.093, -0.091, -0.089, -0.087, -0.084, -0.082, -0.079, -0.077, -0.074,
   -0.072, -0.068, -0.065, -0.062, -0.059, -0.056, -0.053, -0.049, -0.046,
   -0.043, -0.039, -0.036, -0.032, -0.029, -0.025, -0.022, -0.018, -0.015,
   -0.011, -0.007, -0.003, 0.000,  0.003,  0.007,  0.011,  0.014,  0.018,
   0.022,  0.025,  0.029,  0.032,  0.036,  0.039,  0.043,  0.046,  0.049,
   0.053,  0.056,  0.059,  0.062,  0.065,  0.068,  0.071,  0.074,  0.077,
   0.080,  0.082,  0.084,  0.087,  0.090,  0.091,  0.094,  0.096,  0.098,
   0.100,  0.103,  0.104,  0.107,  0.108,  0.110,  0.113,  0.115,  0.116,
   0.120,  0.121,  0.121,  0.123,  0.125,  0.124,  0.127,  0.140,  0.119,
   0.114,  0.028,  0.049,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000},
  {0.003,  0.013,  0.026,  0.039,  0.052,  0.065,  0.078,  0.091,  0.104,
   0.118,  0.132,  0.145,  0.160,  0.174,  0.189,  0.203,  0.219,  0.234,
   0.250,  0.267,  0.283,  0.301,  0.319,  0.338,  0.357,  0.375,  0.398,
   0.419,  0.440,  0.464,  0.491,  0.514,  0.541,  0.569,  0.598,  0.623,
   0.660,  0.696,  0.728,  0.763,  0.804,  0.847,  0.888,  0.930,  0.988,
   1.015,  1.076,  1.128,  1.167,  1.228,  1.297,  1.374,  1.443,  1.511,
   1.564,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,  0.000,
   0.000,  -1.992, -1.884, -1.765, -1.703, -1.609, -1.572, -1.493, -1.426,
   -1.356, -1.309, -1.243, -1.202, -1.109, -1.069, -1.026, -0.970, -0.933,
   -0.881, -0.844, -0.803, -0.767, -0.721, -0.691, -0.659, -0.629, -0.596,
   -0.569, -0.541, -0.514, -0.490, -0.466, -0.441, -0.419, -0.397, -0.377,
   -0.357, -0.337, -0.319, -0.301, -0.283, -0.267, -0.250, -0.234, -0.218,
   -0.203, -0.189, -0.174, -0.160, -0.145, -0.131, -0.119, -0.104, -0.091,
   -0.078, -0.064, -0.052, -0.039, -0.026, -0.013, -0.002}};
Float_t CbmTrdModuleRecT::fgCorrYval[NBINSCORRY][2] = {{2.421729, 0.},
                                                       {1.537359, 0.483472},
                                                       {1.1752, 0.},
                                                       {1.154183, -0.090229}};

ClassImp(CbmTrdModuleRecT)
