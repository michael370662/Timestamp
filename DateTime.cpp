#include <PonyTech-FND-Core/PonyTech-FND-Core.hpp>
#include "../catch.hpp"
#include "../StdStringHelper.hpp"

using namespace FND;

TEST_CASE("Testing DateTime print_format", "[core]")
{
    Datetime datetime(2019,6,17,11,55,59);
    REQUIRE(toStdString(datetime.string()) == "2019-Jun-17 11:55:59");
}
TEST_CASE("Testing DateTime comparison", "[core]")
{
    Datetime datetime(2019,6,17,11,55,59);
    Datetime datetime1(2019,6,19,11,9,22);
    Datetime result(2019,6,19,11,9,22);
    result = datetime + (datetime1 - datetime);
    REQUIRE(toStdString(result.string()) == "2019-Jun-19 11:9:22");
}
TEST_CASE("Testing DateTime custom format", "[core]")
{
    SECTION("default")
    {
        Datetime datetime(2019,6,17,11,55,59);
        ConstString str("yyyy-MMM-dd HH:mm:ss");
        REQUIRE(toStdString(datetime.string(str)) == "2019-Jun-17 11:55:59");
    }
    SECTION("Tossup")
    {
        Datetime datetime(2019,6,17,0,55,59);
        ConstString str("YOLO!!yy-MM-dd hh:mm:ss a <-");
        REQUIRE(toStdString(datetime.string(str)) == "YOLO!!19-06-17 12:55:59 AM <-");
    }
}