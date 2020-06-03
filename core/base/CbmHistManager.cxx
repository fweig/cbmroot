/**
 * \file CbmHistManager.cxx
 * \brief Histogram manager.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmHistManager.h"

#include "CbmUtils.h"                               // for SaveCanvasA...

#include <TAxis.h>                                        // for TAxis
#include <TCanvas.h>                                      // for TCanvas
#include <TCollection.h>                                  // for TIter
#include <TDirectoryFile.h>                               // for TDirectoryFile
#include <TGenericClassInfo.h>                            // for TGenericCla...
#include <TList.h>                                        // for TList
#include <TObject.h>                                      // for TObject
#include <TClass.h>                                       // for TClass
#include <TDirectory.h>                                   // for TDirectory
#include <TGraph.h>                                       // for TGraph
#include <TGraph2D.h>                                     // for TGraph2D
#include <TH1.h>                                          // for TH1
#include <TH2.h>                                          // for TH2
#include <TKey.h>                                         // for TKey
#include <TNamed.h>                                       // for TNamed
#include <TProfile.h>                                     // for TProfile
#include <TProfile2D.h>                                   // for TProfile2D

#include <boost/regex.hpp>                                // for basic_regex, regex, regex_match

#include <algorithm>                                      // for max, min, fill
#include <cassert>                                        // for assert
#include <exception>                                      // for exception
#include <iostream>                                       // for string
#include <limits>                                         // for numeric_limits
#include <map>                                            // for map, operat...
#include <string>                                         // for basic_string
#include <vector>                                         // for vector

using std::string;
using std::map;
using std::vector;
using std::exception;
using std::sort;

class CompareTNamedMore:
   public std::binary_function<
   const TNamed*,
   const TNamed*,
   Bool_t>
{
public:
   Bool_t operator()(const TNamed* object1, const TNamed* object2) const {
      return string(object1->GetName()) > string(object2->GetName());
   }
};

CbmHistManager::CbmHistManager():
      fMap(), fCanvases()
{

}

CbmHistManager::~CbmHistManager()
{

}

template<class T> vector<T> CbmHistManager::ObjectVector(
         const string& pattern) const
{
  vector<T> objects;

  try {
	const boost::regex e(pattern);
	map<string, TNamed*>::const_iterator it;
	for (it = fMap.begin(); it != fMap.end(); it++) {
		if (boost::regex_match(it->first, e)){
			T ObjectPointer = dynamic_cast<T>(it->second);
			if (ObjectPointer != nullptr)
				objects.push_back(ObjectPointer);
		}

	}
  } catch (exception& ex) {
    LOG(info) << "Exception in CbmHistManager::ObjectVector: " << ex.what();
  }

  sort(objects.begin(), objects.end(), CompareTNamedMore());
  return objects;
}

vector<TH1*> CbmHistManager::H1Vector(
      const string& pattern) const
{
	return ObjectVector<TH1*>(pattern);
}

vector<TH2*> CbmHistManager::H2Vector(
      const string& pattern) const
{
	return ObjectVector<TH2*>(pattern);
}

vector<TGraph*> CbmHistManager::G1Vector(
      const string& pattern) const
{
	return ObjectVector<TGraph*>(pattern);
}

vector<TGraph2D*> CbmHistManager::G2Vector(
      const string& pattern) const
{
	return ObjectVector<TGraph2D*>(pattern);
}

vector<TProfile*> CbmHistManager::P1Vector(
      const string& pattern) const
{
   return ObjectVector<TProfile*>(pattern);
}

vector<TProfile2D*> CbmHistManager::P2Vector(
      const string& pattern) const
{
   return ObjectVector<TProfile2D*>(pattern);
}

void CbmHistManager::WriteToFile()
{
   map<string, TNamed*>::iterator it;
   for (it = fMap.begin(); it != fMap.end(); it++){
      it->second->Write();
   }
}

void CbmHistManager::ReadFromFile(
       TFile* file)
{
    assert(file != nullptr);
    LOG(info) << "CbmHistManager::ReadFromFile";
    TDirectory* dir = gDirectory;
    TIter nextkey(dir->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*) nextkey())) {
        TObject* obj = key->ReadObj();
        AddTNamedObject(obj);
        AddTDirectoryObject(obj);
    }
}

void CbmHistManager::AddTNamedObject(
         TObject* obj)
{
    if (obj->IsA()->InheritsFrom (TH1::Class()) || obj->IsA()->InheritsFrom (TGraph::Class()) || obj->IsA()->InheritsFrom (TGraph2D::Class())) {
        TNamed* h = (TNamed*) obj;
        //TNamed* h1 = (TNamed*)file->Get(h->GetName());
        Add(string(h->GetName()), h);
    }
}

void CbmHistManager::AddTDirectoryObject(
         TObject* obj)
{
    if (obj->IsA()->InheritsFrom (TDirectoryFile::Class())) {
        TDirectoryFile* fileDir = (TDirectoryFile*)obj;
        TIter nextkey(fileDir->GetListOfKeys());
        TKey *key2;
        while ((key2 = (TKey*) nextkey())) {
            TObject* obj2 = key2->ReadObj();
            AddTNamedObject(obj2);
            AddTDirectoryObject(obj2);
        }
    }
}

void CbmHistManager::Clear(Option_t*)
{
   map<string, TNamed*>::iterator it;
   for (it = fMap.begin(); it != fMap.end(); it++) {
      delete (*it).second;
   }
   fMap.clear();
}

void CbmHistManager::ShrinkEmptyBinsH1(
      const string& histName)
{
   TH1* hist = H1(histName);
   Int_t nofBins = hist->GetNbinsX();
   Int_t minShrinkBin = std::numeric_limits<Int_t>::max();
   Int_t maxShrinkBin = std::numeric_limits<Int_t>::min();
   Bool_t isSet = false;
   for (Int_t iBin = 1; iBin <= nofBins; iBin++) {
      Double_t content = hist->GetBinContent(iBin);
      if (content != 0.) {
         minShrinkBin = std::min(iBin, minShrinkBin);
         maxShrinkBin = std::max(iBin, maxShrinkBin);
         isSet = true;
      }
   }
   if (isSet) {
      hist->GetXaxis()->SetRange(minShrinkBin, maxShrinkBin);
     // hist->GetYaxis()->SetRange(minShrinkBin, maxShrinkBin);
   }
}

void CbmHistManager::ShrinkEmptyBinsH1ByPattern(
      const string& pattern)
{
	vector<TH1*> effHistos = H1Vector(pattern);
	Int_t nofEffHistos = effHistos.size();
	for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
		ShrinkEmptyBinsH1(effHistos[iHist]->GetName());
	}
}

void CbmHistManager::ShrinkEmptyBinsH2(
      const string& histName)
{
   TH1* hist = H2(histName);
   Int_t nofBinsX = hist->GetNbinsX();
   Int_t nofBinsY = hist->GetNbinsY();
   Int_t minShrinkBinX = std::numeric_limits<Int_t>::max();
   Int_t maxShrinkBinX = std::numeric_limits<Int_t>::min();
   Int_t minShrinkBinY = std::numeric_limits<Int_t>::max();
   Int_t maxShrinkBinY = std::numeric_limits<Int_t>::min();
   Bool_t isSet = false;
   for (Int_t iBinX = 1; iBinX <= nofBinsX; iBinX++) {
      for (Int_t iBinY = 1; iBinY <= nofBinsY; iBinY++) {
         Double_t content = hist->GetBinContent(iBinX, iBinY);
         if (content != 0.) {
            minShrinkBinX = std::min(iBinX, minShrinkBinX);
            maxShrinkBinX = std::max(iBinX, maxShrinkBinX);
            minShrinkBinY = std::min(iBinY, minShrinkBinY);
            maxShrinkBinY = std::max(iBinY, maxShrinkBinY);
            isSet = true;
         }
      }
   }
   if (isSet) {
      hist->GetXaxis()->SetRange(minShrinkBinX, maxShrinkBinX);
      hist->GetYaxis()->SetRange(minShrinkBinY, maxShrinkBinY);
   }
}

void CbmHistManager::ShrinkEmptyBinsH2ByPattern(
      const string& pattern)
{
   vector<TH1*> effHistos = H1Vector(pattern);
   Int_t nofEffHistos = effHistos.size();
   for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
      ShrinkEmptyBinsH2(effHistos[iHist]->GetName());
   }
}

void CbmHistManager::Scale(
      const string& histName,
      Double_t scale)
{
	H1(histName)->Scale(scale);
}

void CbmHistManager::ScaleByPattern(
      const string& pattern,
      Double_t scale)
{
	vector<TH1*> effHistos = H1Vector(pattern);
	Int_t nofEffHistos = effHistos.size();
	for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
		Scale(effHistos[iHist]->GetName(), scale);
	}
}

void CbmHistManager::NormalizeToIntegral(
      const string& histName)
{
   TH1* hist = H1(histName);
   hist->Scale(1. / hist->Integral());
}

void CbmHistManager::NormalizeToIntegralByPattern(
      const string& pattern)
{
   vector<TH1*> effHistos = H1Vector(pattern);
   Int_t nofEffHistos = effHistos.size();
   for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
      NormalizeToIntegral(effHistos[iHist]->GetName());
   }
}

void CbmHistManager::Rebin(
      const string& histName,
      Int_t ngroup)
{
	TH1* hist = H1(histName);
	if (ngroup > 1) {
		hist->Rebin(ngroup);
		hist->Scale(1. / (Double_t)ngroup);
	}
}

void CbmHistManager::RebinByPattern(
      const string& pattern,
      Int_t ngroup)
{
	vector<TH1*> effHistos = H1Vector(pattern);
	Int_t nofEffHistos = effHistos.size();
	for (Int_t iHist = 0; iHist < nofEffHistos; iHist++) {
		Rebin(effHistos[iHist]->GetName(), ngroup);
	}
}

string CbmHistManager::ToString() const
{
	string str = "CbmHistManager list of histograms:\n";
	map<string, TNamed*>::const_iterator it;
	for (it = fMap.begin(); it != fMap.end(); it++){
		str += it->first + "\n";
	}
	return str;
}

TCanvas* CbmHistManager::CreateCanvas(
		const std::string& name,
		const std::string& title,
		Int_t width,
		Int_t height)
{
	TCanvas* c = new TCanvas(name.c_str(), title.c_str(), width, height);
	fCanvases.push_back(c);
	return c;
}

void CbmHistManager::SaveCanvasToImage(
		const std::string& outputDir,
		const std::string& options)
{
	for (unsigned int i = 0; i < fCanvases.size(); i++) {
		Cbm::SaveCanvasAsImage(fCanvases[i], outputDir, options);
	}
}

ClassImp(CbmHistManager)
