
#include "CbmFormatMsHeaderPrintout.h"

std::string
FormatMsHeaderPrintout(const fles::MicrosliceDescriptor& msDescriptor) {
  std::stringstream ss;
  ss << "hi hv eqid flag si sv idx/start        crc      size     offset"
     << std::endl
     << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<unsigned int>(msDescriptor.hdr_id) << " " << std::setw(2)
     << static_cast<unsigned int>(msDescriptor.hdr_ver) << " " << std::setw(4)
     << msDescriptor.eq_id << " " << std::setw(4) << msDescriptor.flags << " "
     << std::setw(2) << static_cast<unsigned int>(msDescriptor.sys_id) << " "
     << std::setw(2) << static_cast<unsigned int>(msDescriptor.sys_ver) << " "
     << std::setw(16) << msDescriptor.idx << " " << std::setw(8)
     << msDescriptor.crc << " " << std::setw(8) << msDescriptor.size << " "
     << std::setw(16) << msDescriptor.offset << std::dec << std::setfill(' ');
  return ss.str();
}

std::ostream& operator<<(std::ostream& os,
                         const fles::MicrosliceDescriptor& msDescriptor) {
  char cPrevFill = os.fill('0');
  return os << "hi hv eqid flag si sv idx/start        crc      size     offset"
            << std::endl
            << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<unsigned int>(msDescriptor.hdr_id) << " "
            << std::setw(2) << static_cast<unsigned int>(msDescriptor.hdr_ver)
            << " " << std::setw(4) << msDescriptor.eq_id << " " << std::setw(4)
            << msDescriptor.flags << " " << std::setw(2)
            << static_cast<unsigned int>(msDescriptor.sys_id) << " "
            << std::setw(2) << static_cast<unsigned int>(msDescriptor.sys_ver)
            << " " << std::setw(16) << msDescriptor.idx << " " << std::setw(8)
            << msDescriptor.crc << " " << std::setw(8) << msDescriptor.size
            << " " << std::setw(16) << msDescriptor.offset << std::dec
            << std::setfill(cPrevFill);
}
