#include <PonyTech-FND-Core/time/Datetime.hpp>
#include <PonyTech-FND-Core/string/String.hpp>
#include <PonyTech-FND-Core/string/ToString.hpp>
#include <PonyTech-FND-Core/string/ConstString.hpp>
#include <PonyTech-FND-Core/string/StringBuilder.hpp>
#include <PonyTech-FND-Core/container/array/OnstackArray.hpp>
#include <PonyTech-LowLevel-Basic/PonyTech-LowLevel-Basic.h>
#include "../format/IntegralFormat.hpp"


BEGIN_NAMESPACE(FND)

static const char* g_month[] = {"Nul","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static const char* g_am_pm[] = {"AM","PM"};
static const char g_format_pattern[] = { 'y', 'M', 'd', 'H', 'h', 'm', 's', 'a' };

int Datetime::compare(const Datetime& rhs) const
{
    auto self_time = basic_system_time_struct2value(m_year, m_mon, m_day, m_hour,m_min, m_sec);
    auto rhs_time = basic_system_time_struct2value(rhs.m_year, rhs.m_mon, rhs.m_day, rhs.m_hour,rhs.m_min, rhs.m_sec);
    return self_time == rhs_time ? 0:(self_time >rhs_time? 1:-1);
}

void Datetime::set(uint32_t  s, uint32_t  m, uint32_t  h, uint32_t  d, uint32_t  mo, uint32_t  y)
{
    this->m_sec = s;
    this->m_min = m;    
    this->m_hour = h;
    this->m_day = d;
    this->m_mon = mo;        
    this->m_year = y;
}

void Datetime::copy(const Datetime& src)
{
    this->m_sec = src.m_sec;
    this->m_min = src.m_min;    
    this->m_hour = src.m_hour;
    this->m_day = src.m_day;
    this->m_mon = src.m_mon;        
    this->m_year = src.m_year;
}

Timespan Datetime::operator-(const Datetime& rhs) const
{
    int64_t self_time = static_cast<int64_t> (basic_system_time_struct2value(m_year, m_mon, m_day, m_hour,m_min, m_sec));
    int64_t rhs_time = static_cast<int64_t> (basic_system_time_struct2value(rhs.m_year, rhs.m_mon, rhs.m_day, rhs.m_hour,rhs.m_min, rhs.m_sec));    
    return Timespan::from_second(self_time - rhs_time);
}

Datetime Datetime::operator+(const Timespan& rhs) const
{
    auto self_time =  basic_system_time_struct2value(m_year, m_mon, m_day, m_hour,m_min, m_sec);
    auto output_time = self_time + rhs.total_second();
   
    uint32_t year, month, day, hour, minute,second;
    basic_system_time_value2struct(output_time, &year, &month, &day, &hour, &minute,&second);
    return Datetime(year, month, day, hour, minute, second);
}

const Datetime& Datetime::operator+=(const Timespan& rhs)
{
    *this = *this + rhs;
    return *this;
}  

Datetime Datetime::now()
{
    uint32_t year, month, day, hour, minute,second;
    basic_system_time_get_now(&year, &month, &day, &hour, &minute,&second);    
    return Datetime(year, month, day, hour, minute, second);
}


String Datetime::string() const
{
    auto index = (m_mon > Misc::count_of(g_month) ? 0: m_mon);
    return ToString::format("{0}-{1}-{2} {3}:{4}:{5}", m_year,ConstString(g_month[index]),m_day,m_hour,m_min,m_sec);
}

String Datetime::string(const PxConstString& format_pattern) const
{
    StringBuilder builder;

    const size_t cnt = Misc::count_of(g_format_pattern);
    size_t pos[cnt];
    uint32_t value[cnt];
    value[0] = m_year;
    value[1] = m_mon;
    value[2] = m_day;
    value[3] = m_hour;
    value[4] = (m_hour == 12 || m_hour == 0) ? 12 : (m_hour % 12);
    value[5] = m_min;
    value[6] = m_sec;
    value[7] = m_hour / 12;

    // initailize the position of each pattern character
    for(size_t i=0; i<cnt; i++) 
    {
        pos[i] = format_pattern.index_of(g_format_pattern[i]);
    }


    size_t offset = 0;
    OnstackArray<char, 20> integral_value;

    for(size_t min_index = Misc::minimum(pos, cnt);pos[min_index] != SizetTraits::not_found; min_index = Misc::minimum(pos, cnt))
    {
        auto start_pos = pos[min_index];
        auto corresponding_ch = format_pattern.at(start_pos);
        // append the non pattern character
        {   ConstString data;
            data.from_substring(format_pattern, offset, start_pos - offset);
            builder.append(data);
        }

        // find the end of corresponding pattern character
        auto end_pos = format_pattern.not_index_of(corresponding_ch, start_pos);
        auto width = end_pos == SizetTraits::not_found ? (format_pattern.length() - start_pos) : (end_pos - start_pos);

        auto max_size = Misc::maximum(static_cast<size_t>(20), width);
        integral_value.resize(max_size);

        // write the integer of corresponding pattern
        if (corresponding_ch == 'a') 
        {
            builder.append(ConstString(g_am_pm[value[min_index]]));
        } 
        else  if (corresponding_ch == 'M' && width == 3) 
        {
            size_t mon_pos = value[min_index] >= Misc::count_of(g_month) ? 0 : value[min_index];
            builder.append(ConstString(g_month[mon_pos]));
        }
        else  if (corresponding_ch == 'y' && width == 2) 
        {

            size_t interral_cnt = Internal::write_integral(integral_value.ptr(), integral_value.count(), (value[min_index] % 100),  width, 10, false, '0'); 
            builder.append(integral_value.ptr(), interral_cnt);           
        }
        else {
            size_t interral_cnt = Internal::write_integral(integral_value.ptr(), integral_value.count(), value[min_index],  width, 10, false, '0'); 
            builder.append(integral_value.ptr(), interral_cnt);
        }

        // update next pos of that pattern
        offset = end_pos == SizetTraits::not_found ? format_pattern.length() :  end_pos;
        pos[min_index] = end_pos == SizetTraits::not_found ? SizetTraits::not_found : format_pattern.index_of(corresponding_ch, end_pos);
    }
    
    if (offset != format_pattern.length())    
    {   // append the non pattern character
        ConstString data;
        data.from_substring(format_pattern, offset, format_pattern.length() - offset);
        builder.append(data);
    }
    return builder.get();
}

END_NAMESPACE(FND)