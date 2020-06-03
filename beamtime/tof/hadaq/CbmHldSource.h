/** @file CbmHldSource.h
 ** @author Christian Simon <csimon@physi.uni-heidelberg.de>
 ** @date 2015-11-25
 **/

#ifndef CBMHLDSOURCE_H
#define CBMHLDSOURCE_H 1

#include "FairMbsSource.h"

//#include "TObjArray.h" // already included in "FairMbsSource.h"
#include "TString.h"

namespace hadaq
{
  struct RawEvent;
}

class TList;


/** @class CbmHldSource
 ** @brief ...
 ** @author C. Simon <csimon@physi.uni-heidelberg.de>
 ** @version 1.0
 **
 ** ...
 **
 **/
class CbmHldSource : public FairMbsSource
{
  public:

    /** default constructor **/
    CbmHldSource();

    /** default destructor **/
    ~CbmHldSource();

    /** inherited pure virtual methods from FairSource **/
    Bool_t Init();
    Int_t ReadEvent(UInt_t = 0);
    void Close();

    void AddFile(const TString& tFileName);
    void AddPath(const TString& tFileDirectory,
                 const TString& tFileNameWildCard);

  protected:

  private:
    /**private methods*/

    /** copy constructor **/
    CbmHldSource(const CbmHldSource&);

    /** assignment operator **/
    CbmHldSource& operator=(const CbmHldSource&);

    Bool_t OpenNextFile();
    void CloseFile();

    /**private members*/
    
    TList* fFileNames;
    Int_t fNFiles;
    Int_t fCurrentFile;
    TString fCurrentFileName;
  	Int_t fNEvents;
  	Int_t fCurrentEvent;
    std::ifstream* fFileStream;
    Char_t* fDataBuffer;
    Bool_t fNoMoreEvents;

    ClassDef(CbmHldSource,2)
};

#endif
