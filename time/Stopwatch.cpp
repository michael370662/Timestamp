#include <PonyTech-FND-Core/time/Stopwatch.hpp>
#include <PonyTech-FND-Core/time/Timetick.hpp>

BEGIN_NAMESPACE(FND)

Stopwatch::Stopwatch()
{
    reset();
}

void Stopwatch::reset()
{
    m_start  = 1;
    m_pause  = 1;
    m_offset = 0;
}

void Stopwatch::resume()
{
    if (m_pause == 0) return;

    m_offset += m_pause - m_start;
    m_pause = 0;
    m_start = Timetick::get_current_tick();
}

void Stopwatch::pause() 
{
    if (m_pause != 0) return;
    m_pause = Timetick::get_current_tick();
}

Timespan Stopwatch::get() const
{
    auto val = m_pause != 0 ? m_pause : Timetick::get_current_tick();
    return Timespan(m_offset + (val - m_start));
}

void Stopwatch::operator+=(const Timespan& src)
{
    m_offset += src.tick();
}

void Stopwatch::operator-=(const Timespan& src)
{
    m_offset -= src.tick();
}

END_NAMESPACE(FND)
