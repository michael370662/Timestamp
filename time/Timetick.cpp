#include <PonyTech-FND-Core/singleton/AbstractSingleton.hpp>
#include <PonyTech-FND-Core/singleton/SingletonSystem.hpp>
#include <PonyTech-FND-Core/time/Timetick.hpp>
#include <PonyTech-LowLevel-Basic/PonyTech-LowLevel-Basic.h>

BEGIN_NAMESPACE(FND)

static uint64_t tick_unit;

BEGIN_NAMESPACE()
class TimetickInit : public AbstractSingleton
{
    FND_SINGLETON(TimetickInit);
};
TimetickInit::TimetickInit() { tick_unit = basic_system_get_timer_tick_unit(); }
TimetickInit::~TimetickInit() {}

END_NAMESPACE()

static RegisterSingleton<TimetickInit> s_init;

uint64_t Timetick::get_current_tick()
{
    return basic_system_get_current_tick(tick_unit);
}

Timespan Timetick::get()
{
    return Timespan(get_current_tick());
}

END_NAMESPACE(FND)
