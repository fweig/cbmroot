/** @file CbmLmdDebug.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.12.2014
 **/

#ifndef CBMLMDDEBUG_H
#define CBMLMDDEBUG_H 1

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, UInt_t, ULong_t, Bool_t, Long64_t
#include <stdint.h>      // for uint32_t
#include <map>           // for map
#include <utility>       // for pair
#include <TObject.h>     // for TObject
#include <TString.h>     // for TString

namespace roc { class Iterator; }
namespace roc { class Message; }

/** @class CbmLmdDebug
 ** @brief Read and analyse message stream from lmd file
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.12.2014
 ** @version 1.0
 **
 ** This lightweight class reads messages from a lmd file and does some
 ** basic statistics and log output for debugging purposes.
 **/
class CbmLmdDebug: public TObject {

	public:

		CbmLmdDebug(const char* fileName = "");

		virtual ~CbmLmdDebug();

    /** Initialisation **/
    virtual Bool_t Init();

    /** Execution
     ** @param nMessages  Messages to be read. -1 means all in the file.
     **/
		virtual Int_t Run(Int_t nMessages = -1);


	private:

		TString fLmdFileName;           ///< Name of input lmd file
    roc::Iterator* fRocIter;        ///< ROC iterator
    roc::Message*  fCurrentMessage; //! Current ROC message

    /** Current epoch marker for each ROC **/
    std::map<Int_t, uint32_t> fCurrentEpoch;

    // Counters
    UInt_t fNofMsgType[8];   ///< Messages per type
    std::map<std::pair<Int_t, Int_t>, UInt_t> fNofHitNxy;  ///<  Hit messages per NXYTER
    std::map<Int_t, UInt_t> fNofSync;  ///< Sync messages per ROC

    // Last message info
    Int_t   fLastMsgType;
    Int_t   fLastMsgRocId;
    Int_t   fLastMsgNxyId;
    ULong_t fLastMsgTime;

    // Run start and end time
    ULong_t fTimeStart;
    ULong_t fTimeStop;

    Long64_t fMaxTimeDisorder; ///< Maximal time disordering of messages


    // Print statistics
    void PrintStats();

    // Store message info
    void SetLastMessage();

    CbmLmdDebug(const CbmLmdDebug&);
    CbmLmdDebug& operator=(const CbmLmdDebug&);
         
    ClassDef(CbmLmdDebug,1);

};

#endif /* CBMLMDDEBUG_H */
