// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmMcbm2018MsCrcCheck                    -----
// -----                    Created 02.02.2019 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MsCrcCheck.h"

#include "CbmFormatDecHexPrintout.h"
#include "CbmFormatMsHeaderPrintout.h"

#include "Timeslice.hpp"
#include "TimesliceInputArchive.hpp"
#include "TimesliceSubscriber.hpp"

#include <Logger.h>

#include <fstream>
#include <iomanip>
#include <iostream>

CbmMcbm2018MsCrcCheck::CbmMcbm2018MsCrcCheck()
  : fFileName("")
  , fInputFileList(new TObjString())
  , fFileCounter(0)
  , fTSNumber(0)
  , fTSCounter(0)
  , fSource(nullptr)
{
}

CbmMcbm2018MsCrcCheck::~CbmMcbm2018MsCrcCheck() {}

void CbmMcbm2018MsCrcCheck::Run()
{
  while (OpenNextFile()) {
    while (auto timeslice = fSource->get()) {
      const fles::Timeslice& ts = *timeslice;
      auto tsIndex              = ts.index();

      if (0 == tsIndex % 1000) { LOG(info) << "Reading Timeslice " << tsIndex; }  // if( 0 == tsIndex % 1000 )

      UInt_t fuNbCoreMsPerTs = ts.num_core_microslices();
      UInt_t fuNbComponents  = ts.num_components();
      /// Loop over core microslices
      for (UInt_t uMsIdx = 0; uMsIdx < fuNbCoreMsPerTs; uMsIdx++) {
        /// Loop over registered components
        for (UInt_t uMsCompIdx = 0; uMsCompIdx < fuNbComponents; ++uMsCompIdx) {
          bool bCrcOk = ts.get_microslice(uMsCompIdx, uMsIdx).check_crc();

          if (!bCrcOk) {
            auto msDescriptor        = ts.descriptor(uMsCompIdx, uMsIdx);
            uint32_t uSize           = msDescriptor.size;
            const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));
            LOG(info) << "-----------------------------------------------------"
                         "----------";
            //                  LOG(info) << Form( " CRC error for TS %6llu MS %3u Component %2u, dump following", tsIndex, uMsIdx, uMsCompIdx );
            LOG(info) << " CRC error for TS " << FormatDecPrintout(tsIndex, 6)
                      << Form(" MS %3u Component %2u, dump following", uMsIdx, uMsCompIdx);
            LOG(info) << "-----------------------------------------------------"
                         "----------";
            /*
                  LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
                  LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                                    static_cast<unsigned int>(msDescriptor.hdr_id),
                                    static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                                    static_cast<unsigned int>(msDescriptor.sys_id),
                                    static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                                    msDescriptor.size, msDescriptor.offset );
*/
            LOG(info) << FormatMsHeaderPrintout(msDescriptor);
            std::stringstream ss;
            for (UInt_t uByte = 0; uByte < uSize; ++uByte) {
              ss << Form("%02x", msContent[uByte]);
              if (3 == uByte % 4) ss << " ";
              if (15 == uByte % 16) ss << "\n";
            }  // for( UInt_t uByte = 0; uByte < uSize; ++uByte )
            if (0 == uSize % 16) ss << "\n";
            LOG(info) << ss.str();
          }  // if( !bCrcOk )
        }    // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fuNbComponents; ++uMsCompIdx )
      }      // for( UInt_t uMsIdx = 0; uMsIdx < fuNbCoreMsPerTs; uMsIdx ++ )

      fTSCounter++;
      if (0 == fTSCounter % 10000) {
        LOG(info) << "Analysed " << fTSCounter << " TS ";
      }  // if( 0 == fTSCounter % 10000 )
    }    // while( auto timeslice = fSource->get() )

    /// If no more data and file mode, try to read next file in List
    if (fSource->eos() && 0 < fFileName.Length()) {
      fFileCounter++;  // Increment file counter to go to next item in List
    }                  // if( fSource->eos() && 0 < fFileName.Length() )
  }                    // while( OpenNextFile() )
}

Bool_t CbmMcbm2018MsCrcCheck::OpenNextFile()
{
  // First Close and delete existing source
  if (nullptr != fSource) delete fSource;

  if (fFileCounter < fInputFileList.GetSize()) {
    // --- Open current input file
    TObjString* tmp = dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
    fFileName       = tmp->GetString();

    LOG(info) << "Open the Flib input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.Data(), "r");
    if (!inputFile) {
      LOG(error) << "Input file " << fFileName << " doesn't exist.";
      return kFALSE;
    }
    fclose(inputFile);
    fSource = new fles::TimesliceInputArchive(fFileName.Data());
    if (!fSource) {
      LOG(error) << "Could not open input file.";
      return kFALSE;
    }
  }  // if( fFileCounter < fInputFileList.GetSize() )
  else {
    LOG(info) << "End of files list reached: file counter is " << fFileCounter << " for " << fInputFileList.GetSize()
              << " entries in the file list.";
    return kFALSE;
  }  // else of if( fFileCounter < fInputFileList.GetSize() )

  return kTRUE;
}


ClassImp(CbmMcbm2018MsCrcCheck)
