//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#include "DetectorSetupMessenger.hh"

#include <magic_enum.hpp>

#include <G4UnitsTable.hh>

STDSlotInfo_t::STDSlotInfo_t(const char *param)
{
    std::string _type;
    char units[30];

    std::istringstream is(param);
    is >> slotID >> _type >> slotDistance >> units;
    type = magic_enum::enum_cast<Detector::Type>(_type).value();
    slotDistance *= G4UnitDefinition::GetValueOf(units);
}