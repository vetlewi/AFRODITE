//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_DETECTORSETUPMESSENGER_H
#define AFRODITE_DETECTORSETUPMESSENGER_H

#include <G4UImessenger.hh>

#include <detector/DetectorFactory.hh>

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;

class DetectorFrame;

//! Parsed representation of a "<slotID> <type> <distance> <unit>" command.
//! Distance is converted to Geant4 internal units on construction.
struct STDSlotInfo_t {
    int slotID = 0;
    Detector::Type type = Detector::Type::none;
    double slotDistance = 0.;

    STDSlotInfo_t() = default;
    STDSlotInfo_t(const int &id, const Detector::Type &typ, const double &distance)
        : slotID( id ), type( typ ), slotDistance( distance ){}

    explicit STDSlotInfo_t(const char *param);
};

//! UI messenger exposing the AFRODITE detector frame to /control macros.
//!
//! Commands (all available before /run/initialize):
//!   /AFRODITE/frame/large/set    <id 0-15> <none|clover|ocl_labr|fta_labr> <distance> <unit>
//!   /AFRODITE/frame/large/shield <id 0-15> <true|false>
//!   /AFRODITE/frame/small/set    <id 0-7>  <none|fta_labr> <distance> <unit>
//!   /AFRODITE/structure/chamber  <true|false>
//!   /AFRODITE/structure/frame    <true|false>
//!   /AFRODITE/structure/target   <true|false>
class DetectorSetupMessenger : public G4UImessenger
{
public:
    explicit DetectorSetupMessenger(DetectorFrame &frame);
    ~DetectorSetupMessenger() override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
    //! Apply a "set" command to the large (large == true) or small slot family.
    void ApplySet(bool large, const G4String &params);

    DetectorFrame &fFrame;

    G4UIdirectory *fDir;
    G4UIdirectory *fFrameDir;
    G4UIdirectory *fLargeDir;
    G4UIdirectory *fSmallDir;
    G4UIdirectory *fStructureDir;

    G4UIcommand *fLargeSetCmd;
    G4UIcommand *fLargeShieldCmd;
    G4UIcommand *fSmallSetCmd;

    G4UIcmdWithABool *fChamberCmd;
    G4UIcmdWithABool *fFrameCmd;
    G4UIcmdWithABool *fTargetCmd;
};

#endif //AFRODITE_DETECTORSETUPMESSENGER_H
