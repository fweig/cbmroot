
#include "CbmFormatMsBufferPrintout.h"

std::string FormatMsBufferPrintout(const fles::Timeslice& ts, const size_t uMsCompIdx, const size_t uMsIdx,
                                   const uint32_t uBlocksPerLine)
{
  fles::MicrosliceDescriptor msDescriptor = ts.descriptor(uMsCompIdx, uMsIdx);
  const uint8_t* msContent                = reinterpret_cast<const uint8_t*>(ts.content(uMsCompIdx, uMsIdx));

  return FormatMsBufferPrintout(msDescriptor, msContent, uBlocksPerLine);
}

std::string FormatMsBufferPrintout(const fles::MicrosliceDescriptor& msDescriptor, const uint8_t* msContent,
                                   const uint32_t uBlocksPerLine)
{
  uint32_t uMsSize        = msDescriptor.size / 4;  // Size in 32 bit blocks
  const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);

  std::stringstream ss;
  ss << "Microslice buffer content:" << std::endl << std::setfill('0');
  for (uint32_t uBlock = 0; uBlock < uMsSize; ++uBlock) {
    ss << "0x" << std::hex << std::setw(8) << pInBuff[uBlock] << " ";

    if (uBlocksPerLine - 1 == uBlock % uBlocksPerLine)
      ss << " : " << std::dec << std::setw(4) << uBlock - 1 << std::endl;
  }  // for( uint32_t uBlock = 0; uBlock < uMsSize; ++uBlock )
  if (0 < uMsSize % uBlocksPerLine) ss << std::endl;
  ss << std::dec << std::setfill(' ');

  return ss.str();
}
