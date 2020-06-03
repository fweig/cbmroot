#ifndef CBMTESTMINIRICHPAIRBUILDER_H
#define CBMTESTMINIRICHPAIRBUILDER_H

#include "FairTask.h" // mother class

// STD
#include <list>

// CbmRoot
#include "CbmTrbRawMessage.h"

class TClonesArray;
class TH2D;

class CbmTestMiniRichPairBuilder : public FairTask
{
public:
	CbmTestMiniRichPairBuilder();
	virtual ~CbmTestMiniRichPairBuilder();

	virtual InitStatus Init();

	virtual void Exec(Option_t* opt);

	virtual void Finish();

private:

	TClonesArray* fInputTrbRawMessages;

	TH2D* fhToT;

	std::list<CbmTrbRawMessage*> fLeadingEdges;
	std::list<CbmTrbRawMessage*> fTrailingEdges;

	ClassDef(CbmTestMiniRichPairBuilder, 1);
};

#endif // CBMTESTMINIRICHPAIRBUILDER_H
