/**
* \file CbmRichProjectionProducerBase.h
*
* \brief Base class for STS track projections onto the photodetector plane.
*
* \author Semen Lebedev
* \date 2012
**/

#ifndef CBM_RICH_PROJECTION_PRODUCER_BASE
#define CBM_RICH_PROJECTION_PRODUCER_BASE

class TClonesArray;

/**
* \class CbmRichProjectionProducerBase
*
* \brief Base class for STS track projections onto the photodetector plane.
*
* \author Semen Lebedev
* \date 2012
**/
class CbmRichProjectionProducerBase
{
public:
   /**
    * brief Default constructor.
    */
   CbmRichProjectionProducerBase() { }

   /**
    * \brief Destructor.
    */
   virtual ~CbmRichProjectionProducerBase() { }


   /**
    * \brief Initialization in case one needs to initialize some TCloneArrays.
    */
   virtual void Init() { }

   /**
    * Creates track projections onto the photodetector plane.
    * \param[out] richProj Array of track projections onto the photodetector plane.
    **/
   virtual void DoProjection(
         TClonesArray* richProj) = 0;

private:
   /**
    * \brief Copy constructor.
    */
   CbmRichProjectionProducerBase(const CbmRichProjectionProducerBase&);

   /**
    * \brief Assignment operator.
    */
   CbmRichProjectionProducerBase& operator=(const CbmRichProjectionProducerBase&);
};

#endif

