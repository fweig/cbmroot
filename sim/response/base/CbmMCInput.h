/** @file CbmMCInput.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 09.11.2018
 **/

#ifndef CBMMCINPUT_H
#define CBMMCINPUT_H 1

#include <set>
#include "TChain.h"
#include "TF1.h"
#include "TObject.h"
#include "CbmDefs.h"


/** @class CbmMCInput
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 9 November 2018
 ** @brief An MC (transport) input to digitisation in CBM
 **
 ** CbmMCInput gives access to the entries of a TChain according to the
 ** specified access mode through the method GetNextEntry(). The access to
 ** the tree entries can be sequential with stopping at the end
 ** (mode = kRegular), sequential round-the-corner (mode kRepeat) or random
 ** (mode = kRandom).
 **/
class CbmMCInput : public TObject
{

  public:

    /** @brief Default constructor **/
    CbmMCInput();


    /** @brief Constructor
     ** @param chain  Pointer to input file chain
     ** @param mode   Access mode (see EAccessMode)
     **/
    CbmMCInput(TChain* chain, ECbmTreeAccess mode = ECbmTreeAccess::kRegular);


    /** @brief Destructor **/
    virtual ~CbmMCInput();


    /** @brief List of branches
     ** @value Reference to branch list
     **/
    std::set<TString>& GetBranchList();


    /** @brief Pointer to chain
     ** @value Pointer to TChain object
     */
    TChain* GetChain() const {
      return fChain;
    }


    /** @brief Get the next unused entry from the chain
     ** @value Id of tree entry.
     **
     ** The method returns -1 if the maximum number of entries is exceeded.
     **/
    Int_t GetNextEntry();


    /** @brief Maximal number of events to be read from the input.
     ** @value Number of entries in the tree for kRegular. -1 else.
     **/
    Int_t GetMaxNofEvents() const {
      return ( fMode == ECbmTreeAccess::kRegular ? fChain->GetEntries() : -1);
    }


    /** @brief Tree access mode
     ** @value Access mode
     **/
    ECbmTreeAccess GetMode() const {
      return fMode;
    }


    /** @brief Number of entries
     ** @value Number of entries in this input chain.
     **/
    Long64_t GetNofEntries() const {
      return fChain->GetEntries();
    }


    /** @brief Number of used entries
     ** @value Number of successful calls to GetNextEvent()
     **/
    UInt_t GetNofUsedEntries() const {
      return fNofUsedEntries;
    }



  private:

    TChain* fChain;                //! Input chain
    ECbmTreeAccess fMode;          // Access mode to tree
    std::set<TString> fBranches;   // List of branch names
    UInt_t fLastUsedEntry;         // Index of last used entry
    UInt_t fNofUsedEntries;        // Number of used entries


    /** @brief Read list of branches from file
     ** @value Number of branches
     **
     ** The list of branches is stored as TList in each file.
     **/
    UInt_t ReadBranches();


    ClassDef(CbmMCInput, 1);

};

#endif /* CBMMCINPUT_H */
