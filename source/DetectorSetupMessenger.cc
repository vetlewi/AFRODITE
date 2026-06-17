//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#include "DetectorSetupMessenger.hh"

#include <sstream>
#include <string>

#include <globals.hh>
#include <magic_enum/magic_enum.hpp>

#include <Frame/DetectorFrame.hh>

#include <G4UIdirectory.hh>
#include <G4UIcommand.hh>
#include <G4UIparameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UnitsTable.hh>
#include <G4ApplicationState.hh>

STDSlotInfo_t::STDSlotInfo_t(const char *param)
{
    std::string _type;
    char units[30];

    std::istringstream is(param);
    is >> slotID >> _type >> slotDistance >> units;
    type = magic_enum::enum_cast<Detector::Type>(_type).value();
    slotDistance *= G4UnitDefinition::GetValueOf(units);
}

namespace {

//! Build a "/AFRODITE/.../set" command taking <id> <type> <distance> <unit>.
G4UIcommand *MakeSetCommand(const char *path, const char *allowed, G4UImessenger *m)
{
    auto *cmd = new G4UIcommand(path, m);
    cmd->SetGuidance("Place a detector in a frame slot, or empty it with type 'none'.");
    cmd->SetGuidance(std::string("  <id> <type> <distance> <unit>, type in {").append(allowed).append("}").c_str());

    auto *id = new G4UIparameter("id", 'i', false);
    cmd->SetParameter(id);
    auto *type = new G4UIparameter("type", 's', false);
    cmd->SetParameter(type);
    auto *distance = new G4UIparameter("distance", 'd', true);
    distance->SetDefaultValue(0.0);
    cmd->SetParameter(distance);
    auto *unit = new G4UIparameter("unit", 's', true);
    unit->SetDefaultValue("cm");
    cmd->SetParameter(unit);

    cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    return cmd;
}

void Warn(const char *where, const std::string &what)
{
    G4Exception(where, "DetectorSetup", JustWarning, what.c_str());
}

} // namespace

DetectorSetupMessenger::DetectorSetupMessenger(DetectorFrame &frame)
    : fFrame( frame )
{
    fDir = new G4UIdirectory("/AFRODITE/");
    fDir->SetGuidance("AFRODITE geometry setup commands.");

    fFrameDir = new G4UIdirectory("/AFRODITE/frame/");
    fFrameDir->SetGuidance("Detector frame slot configuration.");

    fLargeDir = new G4UIdirectory("/AFRODITE/frame/large/");
    fLargeDir->SetGuidance("Large frame slots (0-15): clover, ocl_labr or fta_labr.");

    fSmallDir = new G4UIdirectory("/AFRODITE/frame/small/");
    fSmallDir->SetGuidance("Small frame slots (0-7): fta_labr only.");

    fStructureDir = new G4UIdirectory("/AFRODITE/structure/");
    fStructureDir->SetGuidance("Passive structure presence.");

    fLargeSetCmd = MakeSetCommand("/AFRODITE/frame/large/set", "none, clover, ocl_labr, fta_labr", this);
    fSmallSetCmd = MakeSetCommand("/AFRODITE/frame/small/set", "none, fta_labr", this);

    fLargeShieldCmd = new G4UIcommand("/AFRODITE/frame/large/shield", this);
    fLargeShieldCmd->SetGuidance("Enable/disable the BGO shield of a clover in a large slot.");
    fLargeShieldCmd->SetGuidance("  <id> <true|false>");
    auto *shieldId = new G4UIparameter("id", 'i', false);
    fLargeShieldCmd->SetParameter(shieldId);
    auto *shieldFlag = new G4UIparameter("shield", 'b', false);
    fLargeShieldCmd->SetParameter(shieldFlag);
    fLargeShieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fChamberCmd = new G4UIcmdWithABool("/AFRODITE/structure/chamber", this);
    fChamberCmd->SetGuidance("Place the scattering chamber.");
    fChamberCmd->SetParameterName("present", false);
    fChamberCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFrameCmd = new G4UIcmdWithABool("/AFRODITE/structure/frame", this);
    fFrameCmd->SetGuidance("Place the detector frame.");
    fFrameCmd->SetParameterName("present", false);
    fFrameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTargetCmd = new G4UIcmdWithABool("/AFRODITE/structure/target", this);
    fTargetCmd->SetGuidance("Place the target.");
    fTargetCmd->SetParameterName("present", false);
    fTargetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorSetupMessenger::~DetectorSetupMessenger()
{
    delete fLargeSetCmd;
    delete fLargeShieldCmd;
    delete fSmallSetCmd;
    delete fChamberCmd;
    delete fFrameCmd;
    delete fTargetCmd;
    delete fLargeDir;
    delete fSmallDir;
    delete fStructureDir;
    delete fFrameDir;
    delete fDir;
}

void DetectorSetupMessenger::ApplySet(bool large, const G4String &params)
{
    const char *where = large ? "/AFRODITE/frame/large/set" : "/AFRODITE/frame/small/set";

    STDSlotInfo_t info;
    try {
        info = STDSlotInfo_t(params.c_str());
    } catch (const std::exception &) {
        Warn(where, "could not parse '" + params + "': expected <id> <type> <distance> <unit>");
        return;
    }

    const bool valid = large ? DetectorFrame::validLarge(info.slotID)
                             : DetectorFrame::validSmall(info.slotID);
    if (!valid) {
        Warn(where, "slot id " + std::to_string(info.slotID) + " is out of range");
        return;
    }

    const bool allowed = large ? DetectorFrame::largeAllows(info.type)
                               : DetectorFrame::smallAllows(info.type);
    if (!allowed) {
        Warn(where, std::string("detector type '") + std::string(magic_enum::enum_name(info.type))
                    + "' is not allowed in this slot family");
        return;
    }

    Slot &slot = large ? fFrame.large(info.slotID) : fFrame.small(info.slotID);
    slot.type = info.type;
    slot.distance = info.slotDistance;  // angle is fixed by the frame and left unchanged
}

void DetectorSetupMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fLargeSetCmd) {
        ApplySet(true, newValue);
    } else if (command == fSmallSetCmd) {
        ApplySet(false, newValue);
    } else if (command == fLargeShieldCmd) {
        std::istringstream is(newValue);
        int id; std::string flag;
        is >> id >> flag;
        if (!DetectorFrame::validLarge(id)) {
            Warn("/AFRODITE/frame/large/shield", "slot id " + std::to_string(id) + " is out of range");
            return;
        }
        fFrame.large(id).shield = G4UIcommand::ConvertToBool(flag.c_str());
    } else if (command == fChamberCmd) {
        fFrame.place_chamber = fChamberCmd->GetNewBoolValue(newValue);
    } else if (command == fFrameCmd) {
        fFrame.place_frame = fFrameCmd->GetNewBoolValue(newValue);
    } else if (command == fTargetCmd) {
        fFrame.place_target = fTargetCmd->GetNewBoolValue(newValue);
    }
}
