#include <PonyTech-FND-Core/time/Timespan.hpp>
#include <PonyTech-FND-Core/stream/Utf8OutputStream.hpp>
#include "../format/IntegralFormat.hpp"

BEGIN_NAMESPACE(FND)

Timespan::Timespan(int day, int hour, int minute, int second, int microsecond)
{
    m_tick  = static_cast<int64_t>(day)    * k_day_in_micro;
    m_tick += static_cast<int64_t>(hour)   * k_hour_in_micro;
    m_tick += static_cast<int64_t>(minute) * k_minute_in_micro;
    m_tick += static_cast<int64_t>(second) * k_second_in_micro;
    m_tick += static_cast<int64_t>(microsecond);
}

void Timespan::set(const Timespan::Unit& src)
{
    m_tick  = static_cast<int64_t>(src.day)    * k_day_in_micro;
    m_tick += static_cast<int64_t>(src.hour)   * k_hour_in_micro;
    m_tick += static_cast<int64_t>(src.minute) * k_minute_in_micro;
    m_tick += static_cast<int64_t>(src.second) * k_second_in_micro;
    m_tick += static_cast<int64_t>(src.microsecond);
    if (src.negative) m_tick = -m_tick;
}

Timespan::Unit Timespan::to_unit() const
{
    Unit unit;
    unit.negative = m_tick < 0;

    auto quotient = static_cast<uint64_t>(m_tick < 0 ? -m_tick : m_tick);
    uint64_t next_quotient;  // we reply on the compile optimization to using idivl for calculate remainder and quotient at once

    next_quotient = quotient / k_second_in_micro;
    unit.microsecond = static_cast<uint32_t>(quotient % k_second_in_micro);
    quotient = next_quotient;
    next_quotient = quotient / k_minute_in_second;
    unit.second = static_cast<uint32_t>(quotient % k_minute_in_second);
    quotient = next_quotient;
    next_quotient = quotient / k_hour_in_minute;
    unit.minute = static_cast<uint32_t>(quotient % k_hour_in_minute);
    quotient = next_quotient;
    next_quotient = quotient / k_day_in_hour;
    unit.hour = static_cast<uint32_t>(quotient % k_day_in_hour);
    quotient = next_quotient;  

    unit.day = static_cast<uint32_t>(quotient);
    return unit;
}

Timespan Timespan::operator*(float factor) const
{
    // we need to take care of mantanis accuracy problem
    double f = factor < 0 ? -factor : factor;
    uint64_t value = m_tick < 0 ? -m_tick : m_tick;
    uint64_t result;

    double lower = static_cast<uint32_t>(value) * f;
    result = (uint64_t) lower;
    double upper = static_cast<uint32_t>(value >> 32) * f;
    result += ((uint64_t) upper) << 32;
    
    return Timespan((factor < 0) ^ (m_tick < 0) ? -result : result);
}

Timespan Timespan::operator*(double factor) const
{
   // we need to take care of mantanis accuracy problem
    double f = factor < 0 ? -factor : factor;
    uint64_t value = m_tick < 0 ? -m_tick : m_tick;
    uint64_t result;

    double lower = static_cast<uint32_t>(value) * f;
    result = (uint64_t) lower;
    double upper = static_cast<uint32_t>(value >> 32) * f;
    result += ((uint64_t) upper) << 32;
    
    return Timespan((factor < 0) ^ (m_tick < 0) ? -result : result);
}


template<class Stream>
void Timespan::text_write(Stream &stream) const
{
    Timespan::Unit unit = to_unit();
    if (unit.negative)  stream.write('-');
    bool omit = true;
    if (!omit || unit.day > 0)
    {
        omit = false;
        stream.write(' ');
        stream.write(unit.day);
        stream.write(" d");
    }
    if (!omit || unit.hour > 0)
    {
        omit = false;
        stream.write(' ');
        stream.write(unit.hour);
        stream.write(" h");
    }
    if (!omit || unit.minute > 0)
    {
        omit = false;
        stream.write(' ');
        stream.write(unit.minute);
        stream.write(" m");
    }
    
    stream.write(' ');
    stream.write(unit.second);
    stream.write('.');
    char data[6];
    Internal::write_integral(data, 6, unit.microsecond, 6, 10, false, '0');
    stream.write(data, 6);
    stream.write(" s");
}

// explicit template instantiation
template void Timespan::text_write<Utf8OutputStream>(Utf8OutputStream&) const;

END_NAMESPACE(FND)
