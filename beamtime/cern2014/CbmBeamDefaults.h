/*
 * Deprecation Warning: this class is not used any longer in the new cbmtrdtools which are the base parameter classes for any new analysis.
 * It is just needed for older tasks, so it cannot be removed yet.
 */

#ifndef CBMBEAMDEFAULTS_H
#define CBMBEAMDEFAULTS_H 1

#include <map>


/**  DetectorID enumerator  **/
enum Exp_Group {kFlesRich=0xE000,
                kFlesMuenster=0xE001,
                kFlesFrankfurt=0xE002,
                kFlesBucarest=0xE003,
		kFlesHodo1=0xE004,
		kFlesHodo2=0xE005};
/** Maximum number of Syscores which could be in the system **/
const Int_t NrOfSyscores = 1;

/* Number of Active Syscores in the System */
const Int_t NrOfActiveSyscores=1;
/** Number of AFCKs which could be in the system **/
const Int_t NrOfAfcks = 4; // was 4
/** Maximum number of Spadics which could attached to one Syscore **/
const Int_t NrOfSpadics =3; // was 3
const Int_t NrOfHalfSpadics = NrOfSpadics*2;

/* Number of Active Spadics on each Syscore the System */
const Int_t NrOfActiveSpadics=1;
const Int_t NrOfActiveHalfSpadics = NrOfActiveSpadics*2;

/** Base address which will be added to each half spadic number
    the first halfspadic which is connected to the Syscore
    get SpadicBaseAddress+0, the second SpadicBaseAddress +1 ... */
const Int_t SpadicBaseAddress = 0;
const Int_t BaseEquipmentID = kFlesRich;

/*
static const std::map<Int_t, Int_t> groupToExpMap
  = { std::make_pair(kMuenster,0),
      std::make_pair(kFrankfurt,1),
      std::make_pair(Bucarest,2)
};
*/
static const std::map<Int_t, Int_t> groupToExpMap = {
  {kFlesRich,0},
  {kFlesMuenster,1},
  {kFlesFrankfurt,2},
  {kFlesBucarest,3}
};

#endif
