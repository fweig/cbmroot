// Author Cruz Garcia
// Based on TimesliceReader.* from Michael Krieger
// ------------------------------------------------------- 
// Use this implementation at your discretion. This is 
// a temporal implementation of a the DPB ID until it is
// included in the microslice header. 
// Developed for the DESY 2017 testbeam
// -------------------------------------------------------

#include "TimesliceReader20.hpp"
#include <unordered_map>

namespace spadic {

struct TimesliceReader20::TimesliceReader20_ {
    
    std::unordered_map<uint16_t, spadic::MessageReader> _readers;
    std::unordered_map<uint16_t, uint16_t> _eLinkSources; // <source addr, link>

    TimesliceReader20_() : _readers{},_eLinkSources{} {}

    // Microslice contents are different, thereofre use another implementation
    void add_mc(const flib_dpb_20::MicrosliceContents20& mc)
    {
        for (auto& dtm : mc.dtms()) {
	        
            // printf("DTM Addr: %04x Link: %04x Size: %04x Payload: %04x\n", dtm.addr, dtm.link, dtm.size, *dtm.data);
            // TODO distinguish same source address from different components
            //auto& reader = _readers[dtm.addr];
            //auto& reader = _readers[dtm.link];  // Add by the eLink??
            
            // Not the most elegant way of givin a reader addr, but
            // ok for now

            // Formwer way
            //auto id = dtm.addr;
            //auto id = dtm.addr + dtm.link;
            // Proposed for DESY
            auto id = ((dtm.addr & 0xff) << 8) + ((dtm.link & 0xff00) >> 8);
            
            auto& reader = _readers[id];
            reader.add_buffer(dtm.data, dtm.size);
            _eLinkSources[id] = dtm.link;
        }
    }
};

TimesliceReader20::TimesliceReader20() : _t {new TimesliceReader20_} {}
TimesliceReader20::~TimesliceReader20() {}

void TimesliceReader20::add_timeslice(const fles::Timeslice& ts)
{
    for (size_t c {0}; c < ts.num_components(); c++) {
        add_component(ts, c);
    }
}

void TimesliceReader20::add_component(const fles::Timeslice& ts, size_t component)
{
    //printf("***Component: %x Link: %x\n", component, ts.descriptor(component, 0).eq_id);

    for (size_t m {0}; m < ts.num_core_microslices(); m++) {  // num_microslices includes the overlapping slices, while num_core_microslices -- num_microslices needs "component"

        auto& desc = ts.descriptor(component, m);
        auto *content = ts.content(component, m);

        // interpret raw bytes as 16-bit unsigned integers, assuming the
        // native byte order is correct
        auto p = reinterpret_cast<const uint16_t *>(content);
        auto s = desc.size * sizeof(*content) / sizeof(*p);

        _t->add_mc({p, s});
    }
}

std::unordered_set<uint16_t> TimesliceReader20::sources() const
{
    std::unordered_set<uint16_t> result;
    for (auto& item : _t->_readers) {
        auto addr = item.first;
        result.insert(addr);
    }
    return result;
}

std::unique_ptr<spadic::Message> TimesliceReader20::get_message(uint16_t source_addr) const
{
    return _t->_readers[source_addr].get_message();
}

uint16_t TimesliceReader20::get_link(uint16_t source_addr) const
{
    return _t->_eLinkSources[source_addr];
}

} // namespace
