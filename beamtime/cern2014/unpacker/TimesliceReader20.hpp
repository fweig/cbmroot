//! \author Michael Krieger

/**
 * Read SPADIC Messages out of Timeslices containing Microslices using the
 * "packed DTM" format.
 */

#ifndef SPADIC_TIMESLICE_READER
#define SPADIC_TIMESLICE_READER

#include "Message.hpp"
#include "Timeslice.hpp"
#include "MicrosliceContents20.hpp"
#include <unordered_set>

namespace spadic {

struct TimesliceReader20
{
    TimesliceReader20();
    ~TimesliceReader20();

    void add_timeslice(const fles::Timeslice& ts);
    void add_component(const fles::Timeslice& ts, size_t component);
    std::unordered_set<uint16_t> sources() const;
    std::unique_ptr<spadic::Message> get_message(uint16_t source_addr) const;
    uint16_t get_link(uint16_t source_addr) const;

private:
    struct TimesliceReader20_;
    std::unique_ptr<TimesliceReader20_> _t;
};

} // namespace

#endif
