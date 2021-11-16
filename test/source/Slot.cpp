//
// Created by Vetle Wegner Ingeberg on 04/05/2021.
//

#include <doctest/doctest.h>

#include <detector/DetectorFactory.hh>
#include <DetectorSetupMessenger.hh>
#include <magic_enum.hpp>

#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

#include <iostream>
#include <sstream>


std::ostream &operator<<(std::ostream &os, const STDSlotInfo_t &slotInfo)
{
    os << "Slot " << slotInfo.detectorID << "\n";
    os << "\tType: " << magic_enum::enum_name(slotInfo.type) << "\n";
    os << "\tDistance: " << slotInfo.slotDistance;
    return os;
}

STDSlotInfo_t GetValues(const char *param)
{
    int slotID;
    std::string type;
    double distance;
    char units[30];

    std::istringstream is(param);
    is >> slotID >> type >> distance >> units;

    auto etype = magic_enum::enum_cast<Detector::Type>(type);

    return {slotID, etype.value(), distance*G4UnitDefinition::GetValueOf(units)};
}

TEST_CASE("Slot converter")
{
    SUBCASE("Test mm conversion") {
        auto test_str = "4 clover 32 mm";
        auto res = GetValues(test_str);

        REQUIRE(res.detectorID == 4);
        REQUIRE(res.slotDistance == 32 * mm);
        REQUIRE(res.type == Detector::Type::clover);
    }

    SUBCASE("Test cm conversion"){
        auto test_str = "4 clover 32 cm";
        auto res = GetValues(test_str);

        REQUIRE(res.detectorID == 4);
        REQUIRE(res.slotDistance == 32 * cm);
        REQUIRE(res.type == Detector::Type::clover);
    }

    SUBCASE("Detector messenger"){
        auto test_str = "7 8 clover 32 cm";
        DetectorSetupMessenger messenger;
        messenger.SetNewValue(messenger.getSlotCMD(), test_str);
        assert(messenger.GetSlot(7).detectorID == 8);
        assert(messenger.GetSlot(7).type == Detector::clover);
    }

}