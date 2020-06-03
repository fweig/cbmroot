/**
 * \file CbmLitToolFactory.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Tool factory for creation of littrack algorithms.
 **/

#ifndef CBMLITTOOLFACTORY_H_
#define CBMLITTOOLFACTORY_H_

#include "base/CbmLitPtrTypes.h"

#include <string>
#include <iostream>

using std::string;

class CbmLitToolFactory
{
public:

   /**
    * \brief Create track extrapolation tool by name.
    * \param[in] name Tool name.
    * \return Created track extrapolation tool.
    */
   static TrackExtrapolatorPtr CreateTrackExtrapolator(
      const string& name);

   /**
    * \brief Create track propagation tool by name.
    * \param[in] name Tool name.
    * \return Created track propagation tool.
    */
   static TrackPropagatorPtr CreateTrackPropagator(
      const string& name);

   /**
    * \brief Create track update tool by name.
    * \param[in] name Tool name.
    * \return Created track update tool.
    */
   static TrackUpdatePtr CreateTrackUpdate(
      const string& name);

   /**
    * \brief Create track fit tool by name.
    * \param[in] name Tool name.
    * \return Created track fit tool.
    */
   static TrackFitterPtr CreateTrackFitter(
      const string& name);

   /**
    * \brief Create track selection tool by name.
    * \param[in] name Tool name.
    * \return Created track selection tool.
    */
   static TrackSelectionPtr CreateTrackSelection(
      const string& name);

   /**
    * \brief Create track selection tool by name.
    * \param[in] name Tool name.
    * \return Created track selection tool.
    */
   static TrackFinderPtr CreateTrackFinder(
      const string& name);

   /**
    * \brief Create hot to track merge tool by name.
    * \param[in] name Tool name.
    * \return Created hit to track merge tool.
    */
   static HitToTrackMergerPtr CreateHitToTrackMerger(
      const string& name);

private:
   /**
    * \brief Constructor.
    * Constructor is protected since singleton pattern is used.
    * Pointer to the object is returned by static Instance() method.
    */
   CbmLitToolFactory();

   /**
    * \brief Destructor.
    */
   virtual ~CbmLitToolFactory();

   /**
    * \brief Copy constructor.
    */
   CbmLitToolFactory(const CbmLitToolFactory&);

   /**
    * \brief Assignment operator.
    */
   const CbmLitToolFactory& operator=(const CbmLitToolFactory&);
};

#endif /*CBMLITTOOLFACTORY_H_*/
