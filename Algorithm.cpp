#include <PonyTech-FND-Base/PonyTech-FND-Base.hpp>
#include "../catch.hpp"

using namespace FND;

TEST_CASE("find min and max","[base]")
{
    SECTION("min")
    {
        size_t pos[3];
        pos[0] = 4;
        pos[1] = 56;
        pos[2] = 0;
        size_t min_index = Misc::minimum(pos, 3);
        REQUIRE(min_index == 2);
    }
    SECTION("max")
    {
        size_t pos [3];
        pos[0] = 4;
        pos[1] = 56;
        pos[2] = 0;        
        size_t max_index = Misc::maximum(pos, 3);
        REQUIRE(max_index == 1);
    }
}