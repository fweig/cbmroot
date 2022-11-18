macro(define_fairroot_targets)

  # Define FairRoot::Tools target
  add_library(FairRoot::Tools SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::Tools PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}FairTools${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::Tools INTERFACE
    ${FAIRROOT_INCLUDE_DIR}   
  )

  target_link_libraries(FairRoot::Tools INTERFACE
    FairLogger::FairLogger
    ROOT::Core
    ROOT::Graf
    ROOT::Hist
    ROOT::Gpad
    ROOT::RIO
    ROOT::MathCore
  )

  # Define FairRoot::Alignment target
  add_library(FairRoot::Alignment SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::Alignment PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Alignment${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::Alignment INTERFACE
    ${FAIRROOT_INCLUDE_DIR}   
  )

  target_link_libraries(FairRoot::Alignment INTERFACE
    FairRoot::Tools

    ROOT::Geom
  )

  # Define FairRoot::ParBase target
  add_library(FairRoot::ParBase SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::ParBase PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}ParBase${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::ParBase INTERFACE
    ${FAIRROOT_INCLUDE_DIR}
  )
  
  target_link_libraries(FairRoot::ParBase INTERFACE
    FairRoot::Tools

    ROOT::Core
    ROOT::RIO
  )

  # Define FairRoot::GeoBase target
  add_library(FairRoot::GeoBase SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::GeoBase PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}GeoBase${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::GeoBase INTERFACE
    ${FAIRROOT_INCLUDE_DIR}
  ) 

  target_link_libraries(FairRoot::ParBase INTERFACE
    FairRoot::Tools

    ROOT::Core
    ROOT::MathCore
    ROOT::Geom
    ROOT::Graf3d # TRotMatrix
  )

  # Define FairRoot::MbsAPI target
  add_library(FairRoot::MbsAPI SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::MbsAPI PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}MbsAPI${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::MbsAPI INTERFACE
    ${FAIRROOT_INCLUDE_DIR}
  )

  # Define FairRoot::Base target
  add_library(FairRoot::Base SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::Base PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Base${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::Base INTERFACE
    ${FAIRROOT_INCLUDE_DIR}  
  )

  target_link_libraries(FairRoot::Base INTERFACE
    FairRoot::Alignment
    FairRoot::Tools
    FairRoot::ParBase
    FairRoot::GeoBase
    Boost::serialization
    FairRoot::MbsAPI

    ROOT::RHTTP # THttpServer
    ROOT::Core
    ROOT::EG
    ROOT::Geom
    ROOT::GeomPainter # TGeoTrack
    ROOT::Hist
    ROOT::MathCore
    ROOT::Matrix
    ROOT::Net # TSocket
    ROOT::Physics
    ROOT::Proof
    ROOT::RIO
    ROOT::Tree
    VMCLibrary
    ROOT::Gdml
  )


  # The Online library was added with FairRoot v18.8.0 and contains the
  # FairRunOnline class and some other online realted classes. Before this
  # FairRoot version the code was contained in the Base library.
  # If the FairRoot version contains the library define the target, if the
  # FairRoot version doesn't contain the library create an alias to
  # FairRoot::Base. This allows to already use the target FairRoot::Online
  # already before
  FairRootVersion()
  if(${FairRoot_VERSION} VERSION_LESS 18.8)
    add_library(FairRoot::Online ALIAS FairRoot::Base)
  else()
    # Define FairRoot::Online target
    add_library(FairRoot::Online SHARED IMPORTED GLOBAL)
    set_target_properties(FairRoot::Online PROPERTIES
      IMPORTED_LOCATION
      ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Online${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
    target_include_directories(FairRoot::Online INTERFACE
      ${FAIRROOT_INCLUDE_DIR}
    )

    target_link_libraries(FairRoot::Online INTERFACE
      FairRoot::Tools
      FairRoot::Base
      FairRoot::ParBase
      FairRoot::MbsAPI

      ROOT::RHTTP # THttpServer
      ROOT::Core
      ROOT::Geom
      ROOT::Net # TSocket
    )
  endif()

  # Define FairRoot::EventDisplay target
  add_library(FairRoot::EventDisplay SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::EventDisplay PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}EventDisplay${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::EventDisplay INTERFACE
    ${FAIRROOT_INCLUDE_DIR}    
  )

  target_link_libraries(FairRoot::EventDisplay INTERFACE
    FairRoot::Tools
    FairRoot::Base # FairRootManager, FairRunAna, FairTSBufferFunctional, FairTimeStamp, FairEventManager

    ROOT::Core
    ROOT::EG          # TDatabasePDG
    ROOT::Eve         # TEveBoxSet
    ROOT::Ged         # TGedFrame
    ROOT::Geom        # TGeoManager, TGeoNode, TGeoVolume
    ROOT::GeomPainter # TGeoTrack
    ROOT::Gui         # TGFrame, TGNumberEntry, TGWindow
    ROOT::MathCore    # TMathBase
    ROOT::Physics     # TVector3
    ROOT::RGL         # TGLViewer, TGLCameraOverlay, TGLClip, TGLLightSet
    ROOT::Tree        # TChain
    ROOT::RIO
    ROOT::XMLIO       # TXMLEngine
    ROOT::XMLParser   # TXMLNode, TXMLAttr, TDOMParser
  )

  # Define FairRoot::TrackBase target
  add_library(FairRoot::TrackBase SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::TrackBase PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}TrkBase${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::TrackBase INTERFACE
    ${FAIRROOT_INCLUDE_DIR}
    
  )

  target_link_libraries(FairRoot::TrackBase INTERFACE
    FairRoot::Tools
    FairRoot::Base # FairRunAna, FairField

    ROOT::Core
    ROOT::Physics
    ROOT::MathCore
    ROOT::Matrix
  )

  # Define FairRoot::Generators target
  add_library(FairRoot::Generators SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::Generators PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Gen${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::Generators INTERFACE
    ${FAIRROOT_INCLUDE_DIR}    
  )

  target_link_libraries(FairRoot::Generators INTERFACE
    FairRoot::Base # FairPrimaryGenerator, FairGenerator, FairIon, FairParticle, FairRunSim
    FairRoot::Tools

    ROOT::Core
    ROOT::EG
    ROOT::MathCore
    ROOT::Hist
    ROOT::Physics
  )

  # Define FairRoot::BaseMQ target
  add_library(FairRoot::BaseMQ SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::BaseMQ PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}BaseMQ${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::BaseMQ INTERFACE
    ${FAIRROOT_INCLUDE_DIR}   
  )

  target_link_libraries(FairRoot::BaseMQ INTERFACE
    FairRoot::Base # FairTask, FairRunAna, FairRootFileSink, FairFileSource
    FairRoot::ParBase # FairParRootFileIo, FairRuntimeDb
    FairRoot::Tools
    FairRoot::MbsAPI

    FairMQ::FairMQ

    Boost::serialization
    Boost::filesystem

    ROOT::Net # TMessage
    ROOT::Core # Rtypes, TClonesArray
    ROOT::RIO # TFile
    ROOT::Tree # TTree
  )

  # Define FairRoot::ParMQ target
  add_library(FairRoot::ParMQ SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::ParMQ PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}ParMQ${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::ParMQ INTERFACE
    ${FAIRROOT_INCLUDE_DIR}    
  )

  target_link_libraries(FairRoot::ParMQ INTERFACE
    FairRoot::Base # FairRunIdGenerator
    FairRoot::BaseMQ # Serialization policies
    FairRoot::ParBase # FairRuntimeDb, ...
    FairRoot::Tools # FairLogger

    FairMQ::FairMQ

    ROOT::Core
    ROOT::Geom
  )

  # Define FairRoot::Geane target
  add_library(FairRoot::Geane SHARED IMPORTED GLOBAL)
  set_target_properties(FairRoot::Geane PROPERTIES
    IMPORTED_LOCATION
    ${FAIRROOT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Geane${CMAKE_SHARED_LIBRARY_SUFFIX}
  )
  target_include_directories(FairRoot::Geane INTERFACE
    ${FAIRROOT_INCLUDE_DIR}
  )

  target_link_libraries(FairRoot::Geane INTERFACE
    FairRoot::Base # FairField, FairTask, FairPropagator
    FairRoot::TrkBase # FairTrackPar

    geant321

    ROOT::Core
    ROOT::EG
    ROOT::Geom
    VMCLibrary
    ROOT::Physics
    ROOT::MathCore
  )

endmacro()
