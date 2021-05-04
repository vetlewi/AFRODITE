//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#include "DetectorSetupMessenger.hh"

#include <magic_enum.hpp>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>

STDSlotInfo_t::STDSlotInfo_t(const char *param)
{
    std::string _type;
    char units[30];

    std::istringstream is(param);
    is >> detectorID >> _type >> slotDistance >> units;
    type = magic_enum::enum_cast<Detector::Type>(_type).value();
    slotDistance *= G4UnitDefinition::GetValueOf(units);
}


CMDSlotID::CMDSlotID(const char* theCommandPath, G4UImessenger* theMessenger)
    : G4UIcommand(theCommandPath, theMessenger)
{
    auto* slotParam = new G4UIparameter('i');
    SetParameter(slotParam);
    auto* detNoParam = new G4UIparameter('i');
    SetParameter(detNoParam);
    auto* detTypeParam = new G4UIparameter('s');
    SetParameter(detNoParam);
    auto* detDistParam = new G4UIparameter('d');
    SetParameter(detDistParam);
    auto* distUnitParam = new G4UIparameter('s');
    SetParameter(distUnitParam);
    slotParam->SetParameterName("Slot");
    detNoParam->SetParameterName("ID");
    detTypeParam->SetParameterName("type");
    detDistParam->SetParameterName("R");
    distUnitParam->SetParameterName("Unit");

    slotParam->SetParameterRange("Slot >= 0 && slot <= 15");
    detNoParam->SetParameterRange("ID >= 0 && ID <= 15");
}

std::pair<int, STDSlotInfo_t> CMDSlotID::GetSlotInfo(const char *param)
{
    int slotID, detectorID;
    std::string type;
    double slotDistance;
    std::string units;

    std::istringstream is(param);
    is >> slotID >> detectorID >> type >> slotDistance >> units;
    magic_enum::enum_cast<Detector::Type>(type).value();
    slotDistance *= G4UnitDefinition::GetValueOf(units);

    return std::make_pair(slotID,
                          STDSlotInfo_t{detectorID, magic_enum::enum_cast<Detector::Type>(type).value(),
                                            slotDistance});
}

DetectorSetupMessenger::DetectorSetupMessenger()
    : G4UImessenger( )
    , std_slots{ {4, Detector::Type::clover, 13.7*cm}, // pos 1
                 {6, Detector::Type::clover, 13.7*cm}, // pos 2
                 {0, Detector::Type::none, 13.7*cm}, // pos 3
                 {5, Detector::Type::clover, 13.7*cm}, // pos 4
                 {0, Detector::Type::none, 13.7*cm}, // pos 5
                 {1, Detector::Type::clover, 13.7*cm}, // pos 6
                 {0, Detector::Type::clover, 13.7*cm}, // pos 7
                 {0, Detector::Type::none, 13.7*cm}, // pos 8
                 {0, Detector::Type::none, 13.7*cm}, // pos 9
                 {2, Detector::Type::clover, 13.7*cm}, // pos 10
                 {7, Detector::Type::clover, 13.7*cm}, // pos 11
                 {3, Detector::Type::clover, 13.7*cm}, // pos 12
                 {0, Detector::Type::ocl_labr, 14.*cm}, // pos 13
                 {0, Detector::Type::none, 13.7*cm}, // pos 14
                 {0, Detector::Type::none, 13.7*cm}, // pos 15
                 {1, Detector::Type::ocl_labr, 14.*cm}} // pos 16
{
    fDirectory = new G4UIdirectory("/setup/detectors/");

    slotCmd = new CMDSlotID("/setup/detectors/slot", this);
    slotCmd->SetGuidance("Set a detector");
}

DetectorSetupMessenger::~DetectorSetupMessenger()
{
    //delete fDirectory;
    //delete slotCmd;
}

void DetectorSetupMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if ( command == slotCmd ){
        auto res = slotCmd->GetSlotInfo(newValue.c_str());
        if ( res.first < 0 || res.first > 15 ){
            G4Exception(__PRETTY_FUNCTION__, "slotID error",
                        G4ExceptionSeverity::JustWarning,
                        "Invalid slot ID");
        }
        std_slots[res.first] = res.second;
    }
}