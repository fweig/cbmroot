/**
 * \file CbmMatch.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 *
 * Base data class for storing RECO-to-MC matching information.
 **/

#ifndef CBMMATCH_H_
#define CBMMATCH_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t
#include <TObject.h>     // for TObject

#include <string>        // for string
#include <memory>        // for unique_ptr
#include <vector>        // for vector

#include "CbmLink.h"     // for CbmLink

class CbmMatch : public TObject
{
public:
   /**
    * \brief Default constructor.
    */
   CbmMatch();

   /**
    * \brief Destructor.
    */
   virtual ~CbmMatch();

   /* Accessors */
   const CbmLink& GetLink(Int_t i) const { return fLinks[i]; }
   const std::vector<CbmLink>& GetLinks() const { return fLinks; }
   const CbmLink& GetMatchedLink() const { return fLinks[fMatchedIndex]; }
   Int_t GetNofLinks() const { return fLinks.size(); }
   Double_t GetTotalWeight() const { return fTotalWeight; }

   /* Modifiers */
   void AddLinks(const CbmMatch& match);
   void AddLink(const CbmLink& newLink);
   void AddLink(Double_t weight, Int_t index, Int_t entry = -1, Int_t file = -1);
   void ClearLinks();

   /**
    * \brief Return string representation of the object.
    * \return String representation of the object.
    **/
   virtual std::string ToString() const;

protected:
   std::vector<CbmLink> fLinks; // List of links to MC
   Double_t fTotalWeight; // Sum of all reference weights
   Int_t fMatchedIndex; // Index of the matched reference in fReferences array

   ClassDef(CbmMatch, 1);
};

typedef std::unique_ptr<CbmMatch> up_CbmMatch;

#endif /* CBMMATCH_H_ */
