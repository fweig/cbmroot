
#include "CbmFormatMsBufferPrintout.h"
#include "CbmFormatMsHeaderPrintout.h"
#include <ios>

std::string FormatTsHeaderPrintout(const fles::Timeslice& ts) {
  std::stringstream ss;

  uint64_t min_num_microslices   = UINT64_MAX;
  uint64_t max_num_microslices   = 0;
  uint64_t total_num_microslices = 0;
  uint64_t min_microslice_size   = UINT64_MAX;
  uint64_t max_microslice_size   = 0;
  uint64_t total_microslice_size = 0;

  size_t nbComps           = ts.num_components();
  size_t nbMicroslicesCore = ts.num_core_microslices();
  for (uint64_t compIdx = 0; compIdx < nbComps; ++compIdx) {
    uint64_t num_microslices = ts.num_microslices(compIdx);
    min_num_microslices      = std::min(min_num_microslices, num_microslices);
    max_num_microslices      = std::max(max_num_microslices, num_microslices);
    total_num_microslices += num_microslices;
    for (uint64_t msIdx = 0; msIdx < num_microslices; ++msIdx) {
      uint64_t size = ts.descriptor(compIdx, msIdx).size;
      total_microslice_size += size;
      min_microslice_size = std::min(min_microslice_size, size);
      max_microslice_size = std::max(max_microslice_size, size);
    }  // for( uint64_t msIdx = 0; msIdx < num_microslices; ++msIdx )
  }    // for( uint64_t compIdx = 0; compIdx < nbComps; ++compIdx )

  uint64_t min_overlap = min_num_microslices - nbMicroslicesCore;
  uint64_t max_overlap = max_num_microslices - nbMicroslicesCore;

  ss << "Timeslice " << std::setw(6) << ts.index() << " with " << std::setw(3)
     << nbComps << " components "
     << " x " << std::setw(6) << nbMicroslicesCore << " core microslices";
  if (0 != nbComps) {
    ss << " (+";
    if (min_overlap != max_overlap) {
      ss << std::setw(3) << min_overlap << std::setw(3) << ".." << max_overlap;
    }  // if( min_overlap != max_overlap )
    else {
      ss << std::setw(3) << min_overlap;
    }  // else of if( min_overlap != max_overlap )
    ss << " overlap) = " << std::setw(9) << total_num_microslices << std::endl;
    ss
      << "\tmicroslice size min/avg/max: " << std::setw(6)
      << min_microslice_size << " / " << std::fixed << std::setprecision(0)
      << std::setw(6)
      << (static_cast<double>(total_microslice_size) / total_num_microslices)
      //         << std::defaultfloat // commented out as not included in GCC 4.9.2 => restore defaults, probably not needed
      << std::setprecision(6)  // restore defaults, probably not needed
      << " / " << std::setw(6) << max_microslice_size << " bytes" << std::endl;
  }  // if( 0 != nbComps )

  return ss.str();
}

std::string FormatTsPrintout(const fles::Timeslice& ts) {
  std::stringstream ss;
  ss << FormatTsHeaderPrintout(ts);

  size_t nbComps = ts.num_components();
  if (0 != nbComps) {
    size_t nbMicroslicesCore = ts.num_core_microslices();
    size_t nbMicroslicesOverlap =
      ts.num_microslices(0) - ts.num_core_microslices();

    for (size_t compIdx = 0; compIdx < nbComps; ++compIdx) {
      ss << "Core Microslices for component " << std::setw(3) << compIdx
         << std::endl;
      for (size_t msIdx = 0; msIdx < nbMicroslicesCore; ++msIdx) {
        ss << ts.descriptor(compIdx, msIdx);
        ss << FormatMsBufferPrintout(ts, compIdx, msIdx);
        ss << "----------------------------------------------" << std::endl;
      }  // for( size_t msIdx = 0; msIdx < nbMicroslicesCore; ++msIdx )
      ss << "Overlap Microslices for component " << std::setw(3) << compIdx
         << std::endl;
      for (size_t msIdx = 0; msIdx < nbMicroslicesOverlap; ++msIdx) {
        ss << ts.descriptor(compIdx, msIdx + nbMicroslicesCore);
        ss << FormatMsBufferPrintout(ts, compIdx, msIdx + nbMicroslicesCore);
        ss << "----------------------------------------------" << std::endl;
      }  // for( size_t msIdx = 0; msIdx < nbMicroslicesOverlap; ++msIdx )
    }    // for( size_t comp = 0; comp < nbComps; ++comp )
  }      // if( 0 != nbComps )
  ss << "**********************************************" << std::endl;

  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const fles::Timeslice& ts) {
  os << FormatTsHeaderPrintout(ts);

  size_t nbComps           = ts.num_components();
  size_t nbMicroslicesCore = ts.num_core_microslices();
  size_t nbMicroslicesOverlap =
    ts.num_microslices(0) - ts.num_core_microslices();

  for (size_t compIdx = 0; compIdx < nbComps; ++compIdx) {
    os << "Core Microslices for component " << std::setw(3) << compIdx
       << std::endl;
    for (size_t msIdx = 0; msIdx < nbMicroslicesCore; ++msIdx) {
      os << ts.descriptor(compIdx, msIdx);
      os << FormatMsBufferPrintout(ts, compIdx, msIdx);
      os << "----------------------------------------------" << std::endl;
    }  // for( size_t msIdx = 0; msIdx < nbMicroslicesCore; ++msIdx )
    os << "Overlap Microslices for component " << std::setw(3) << compIdx
       << std::endl;
    for (size_t msIdx = 0; msIdx < nbMicroslicesOverlap; ++msIdx) {
      os << ts.descriptor(compIdx, msIdx + nbMicroslicesCore);
      os << FormatMsBufferPrintout(ts, compIdx, msIdx + nbMicroslicesCore);
      os << "----------------------------------------------" << std::endl;
    }  // for( size_t msIdx = 0; msIdx < nbMicroslicesOverlap; ++msIdx )
  }    // for( size_t comp = 0; comp < nbComps; ++comp )
  os << "**********************************************" << std::endl;

  return os;
}
