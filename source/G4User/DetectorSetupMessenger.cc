//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#include "user/DetectorSetupMessenger.hh"

#include <magic_enum.hpp>

#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4Tokenizer.hh>
#include <G4UIcmdWithABool.hh>


DetectorSetupMessenger::DetectorSetupMessenger()
    : G4UImessenger( )
    , standard_slot{
        {13.7*cm, Detector::Type::clover}, // Slot 1
        {13.7*cm, Detector::Type::clover}, // Slot 2
        {14.*cm, Detector::Type::none}, // Slot 3
        {13.7*cm, Detector::Type::clover}, // Slot 4
        {14.*cm, Detector::Type::none}, // Slot 5
        {13.7*cm, Detector::Type::clover}, // Slot 6
        {13.7*cm, Detector::Type::clover}, // Slot 7
        {14.*cm, Detector::Type::none}, // Slot 8
        {14.*cm, Detector::Type::none}, // Slot 9
        {13.7*cm, Detector::Type::clover}, // Slot 10
        {13.7*cm, Detector::Type::clover}, // Slot 11
        {13.7*cm, Detector::Type::clover}, // Slot 12
        {14.*cm, Detector::Type::ocl_labr}, // Slot 13
        {14.*cm, Detector::Type::none}, // Slot 14
        {14.*cm, Detector::Type::none}, // Slot 15
        {14.*cm, Detector::Type::ocl_labr} // Slot 16
        }
    , small_slot{
        {14.*cm, Detector::Type::fta_labr}, // Slot 1
        {14.*cm, Detector::Type::fta_labr}, // Slot 2
        {14.*cm, Detector::Type::fta_labr}, // Slot 3
        {14.*cm, Detector::Type::fta_labr}, // Slot 4
        {14.*cm, Detector::Type::fta_labr}, // Slot 5
        {14.*cm, Detector::Type::none}, // Slot 6
        {14.*cm, Detector::Type::none}, // Slot 7
        {14.*cm, Detector::Type::fta_labr} // Slot 8
        }
    , particle_array{
        {22.5*mm, 330*um, Detector::Type::s2_silicon},
        {25.5*mm, 1090*um, Detector::Type::s2_silicon}
        }
    , targetPresent( true )
    , framePresent( true )
    , targetChamberPresent( true )
    , fGammaDetectorDir( new G4UIdirectory("/Detector/gamma/") )
    , fParticleDetectorDir( new G4UIdirectory("/Detector/particle/") )
    , fSTDGammaDetectorTypeCmd( new G4UIcommand("/Detector/gamma/std/type", this) )
    , fSTDGammaDetectorDistanceCmd( new G4UIcommand("/Detector/gamma/std/distance", this) )
    , fSmallGammaDetectorTypeCmd( new G4UIcommand("/Detector/gamma/small/type", this) )
    , fSmallGammaDetectorDistanceCmd( new G4UIcommand("/Detector/gamma/small/distance", this) )
    , fParticleDetectorPresentCmd( new G4UIcommand("/Detector/particle/present", this) )
    , fParticleDetectorPosCmd( new G4UIcommand("/Detector/particle/distance", this) )
    , fParticleDetectorThicknessCmd( new G4UIcommand("/Detector/particle/thickness", this) )
    , fTargetPresent( new G4UIcmdWithABool("/Detector/target", this) )
    , fFramePresent( new G4UIcmdWithABool("/Detector/frame", this) )
    , fTargetChamberPresent( new G4UIcmdWithABool("/Detector/targetchamber", this) )
    , commands{fSTDGammaDetectorTypeCmd, fSTDGammaDetectorDistanceCmd,
               fSmallGammaDetectorTypeCmd, fSmallGammaDetectorDistanceCmd,
               fParticleDetectorPresentCmd, fParticleDetectorPosCmd, fParticleDetectorThicknessCmd,
               fTargetPresent, fFramePresent, fTargetChamberPresent}
{

    fGammaDetectorDir->SetGuidance("UI commands for setting gamma detectors");
    fParticleDetectorDir->SetGuidance("UI commands for setting particle detectors");

    fSTDGammaDetectorTypeCmd->SetGuidance("Set gamma detector type in a given slot");
    fSTDGammaDetectorTypeCmd->SetGuidance("[Usage] /Detector/gamma/std/type N type");
    fSTDGammaDetectorTypeCmd->SetGuidance("\tN: (int) Slot number, between 0 and 15");
    fSTDGammaDetectorTypeCmd->SetGuidance("\ttype: (str) Detector type");
    fSTDGammaDetectorTypeCmd->SetGuidance("Type can be: 'none', 'clover', 'ocl_labr'");

    auto *param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=15");
    fSTDGammaDetectorTypeCmd->SetParameter(param);

    param = new G4UIparameter("type", 's', false);
    param->SetParameterCandidates("none clover ocl_labr");
    fSTDGammaDetectorTypeCmd->SetParameter(param);

    fSTDGammaDetectorDistanceCmd->SetGuidance("Set distance of detector type in a given slot");
    fSTDGammaDetectorDistanceCmd->SetGuidance("[Usage] /Detector/gamma/std/distance N x unit");
    fSTDGammaDetectorDistanceCmd->SetGuidance("\tN: (int) Slot number, has to be between 0 and 15");
    fSTDGammaDetectorDistanceCmd->SetGuidance("\tx: (double) Distance between detector and target");
    fSTDGammaDetectorDistanceCmd->SetGuidance("\tunit: (str) distance unit");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=15");
    fSTDGammaDetectorDistanceCmd->SetParameter(param);

    param = new G4UIparameter("x", 'd', false);
    param->SetParameterRange("x>0");
    fSTDGammaDetectorDistanceCmd->SetParameter(param);

    param = new G4UIparameter("unit", 's', false);
    param->SetParameterCandidates("nm um mm cm m km");
    fSTDGammaDetectorDistanceCmd->SetParameter(param);

    fSmallGammaDetectorTypeCmd->SetGuidance("Set gamma detector type in a given slot");
    fSmallGammaDetectorTypeCmd->SetGuidance("[Usage] /Detector/gamma/small/type N type");
    fSmallGammaDetectorTypeCmd->SetGuidance("\tN: (int) Slot number, between 0 and 15");
    fSmallGammaDetectorTypeCmd->SetGuidance("\ttype: (str) Detector type");
    fSmallGammaDetectorTypeCmd->SetGuidance("Type can be: 'none' or 'fta_labr'");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=7");
    fSmallGammaDetectorTypeCmd->SetParameter(param);

    param = new G4UIparameter("type", 's', false);
    param->SetParameterCandidates("none fta_labr");
    fSmallGammaDetectorTypeCmd->SetParameter(param);

    fSmallGammaDetectorDistanceCmd->SetGuidance("Set distance of detector type in a given slot");
    fSmallGammaDetectorDistanceCmd->SetGuidance("[Usage] /Detector/gamma/small/distance N x unit");
    fSmallGammaDetectorDistanceCmd->SetGuidance("\tN: (int) Slot number, has to be between 0 and 7");
    fSmallGammaDetectorDistanceCmd->SetGuidance("\tx: (double) Distance between detector and target");
    fSmallGammaDetectorDistanceCmd->SetGuidance("\tunit: (str) distance unit");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=7");
    fSmallGammaDetectorDistanceCmd->SetParameter(param);

    param = new G4UIparameter("x", 'd', false);
    param->SetParameterRange("x>0");
    fSmallGammaDetectorDistanceCmd->SetParameter(param);

    param = new G4UIparameter("unit", 's', false);
    param->SetParameterCandidates("nm um mm cm m km");
    fSmallGammaDetectorDistanceCmd->SetParameter(param);

    fParticleDetectorPresentCmd->SetGuidance("Select if particle detector are present or not");
    fParticleDetectorPresentCmd->SetGuidance("[Usage] /Detector/particle/present N present");
    fParticleDetectorPresentCmd->SetGuidance("\tN: (int) Front or back detector (either 0 or 1)");
    fParticleDetectorPresentCmd->SetGuidance("\tpresent: (bool) True or false");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=1");
    fParticleDetectorPresentCmd->SetParameter(param);
    fParticleDetectorPresentCmd->SetParameter(new G4UIparameter("present", 'b', false));

    fParticleDetectorPosCmd->SetGuidance("Set the distance between target and detector");
    fParticleDetectorPosCmd->SetGuidance("[Usage] /Detector/particle/distance N x unit");
    fParticleDetectorPosCmd->SetGuidance("\tN: (int) Front or back detector (either 0 or 1)");
    fParticleDetectorPosCmd->SetGuidance("\tx: (double) Distance between detector and target");
    fParticleDetectorPosCmd->SetGuidance("\tunit: (str) distance unit");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=1");
    fParticleDetectorPosCmd->SetParameter(param);

    param = new G4UIparameter("x", 'd', false);
    fParticleDetectorPosCmd->SetParameter(param);

    param = new G4UIparameter("unit", 's', false);
    param->SetParameterCandidates("nm um mm cm m km");
    fParticleDetectorPosCmd->SetParameter(param);

    fParticleDetectorThicknessCmd->SetGuidance("Set the thickness of the particle detector");
    fParticleDetectorThicknessCmd->SetGuidance("[Usage] /Detector/particle/thickness N x unit");
    fParticleDetectorThicknessCmd->SetGuidance("\tN: (int) Front or back detector (either 0 or 1)");
    fParticleDetectorThicknessCmd->SetGuidance("\tx: (double) Thickness of detector");
    fParticleDetectorThicknessCmd->SetGuidance("\tunit: (str) distance unit");

    param = new G4UIparameter("N", 'i', false);
    param->SetParameterRange("N>=0&&N<=1");
    fParticleDetectorThicknessCmd->SetParameter(param);

    param = new G4UIparameter("x", 'd', false);
    param->SetParameterRange("x>0");
    fParticleDetectorThicknessCmd->SetParameter(param);

    param = new G4UIparameter("unit", 's', false);
    param->SetParameterCandidates("nm um mm cm m km");
    fParticleDetectorThicknessCmd->SetParameter(param);

    fTargetPresent->SetGuidance("Select if target are present or not");
    fTargetPresent->SetGuidance("[Usage] /Detector/target present");
    fTargetPresent->SetGuidance("\tpresent: (bool) If target are present or not. Default true");
    fTargetPresent->SetParameterName("present", false);
    fTargetPresent->SetDefaultValue(targetPresent);

    fFramePresent->SetGuidance("Select if detector frame are present or not");
    fFramePresent->SetGuidance("[Usage] /Detector/frame present");
    fFramePresent->SetGuidance("\tpresent: (bool) If detector frame are present or not. Default true");
    fFramePresent->SetParameterName("present", false);
    fFramePresent->SetDefaultValue(framePresent);

    fTargetChamberPresent->SetGuidance("Select if target are present or not");
    fTargetChamberPresent->SetGuidance("[Usage] /Detector/targetchamber present");
    fTargetChamberPresent->SetGuidance("\tpresent: (bool) If target are present or not. Default true");
    fTargetChamberPresent->SetParameterName("present", false);
    fTargetChamberPresent->SetDefaultValue(targetPresent);
}

DetectorSetupMessenger::~DetectorSetupMessenger()
{
    delete fGammaDetectorDir;
    delete fParticleDetectorDir;
    for ( auto &v : commands )
        delete v;
}

void DetectorSetupMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if ( command == fSTDGammaDetectorTypeCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        auto type_str = next();
        auto type = magic_enum::enum_cast<Detector::Type>(type_str);
        standard_slot[slotNo].type = type.value();
    }

    if ( command == fSTDGammaDetectorDistanceCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        G4double x = StoD(next());
        G4double unit = G4UIcommand::ValueOf(next());
        standard_slot[slotNo].distance = x*unit;
    }

    if ( command == fSmallGammaDetectorTypeCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        auto type_str = next();
        auto type = magic_enum::enum_cast<Detector::Type>(type_str);
        small_slot[slotNo].type = type.value();
    }

    if ( command == fSmallGammaDetectorDistanceCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        G4double x = StoD(next());
        G4double unit = G4UIcommand::ValueOf(next());
        small_slot[slotNo].distance = x*unit;
    }

    if ( command == fParticleDetectorPresentCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        bool present = StoB(next());

        particle_array[slotNo].type = ( present ) ? Detector::Type::s2_silicon : Detector::Type::none;
    }

    if ( command == fParticleDetectorPosCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        G4double x = StoD(next());
        G4double unit = G4UIcommand::ValueOf(next());
        particle_array[slotNo].distance = x*unit;
    }

    if ( command == fParticleDetectorThicknessCmd ){
        G4Tokenizer next( newValue );

        G4int slotNo = StoI(next());
        G4double x = StoD(next());
        G4double unit = G4UIcommand::ValueOf(next());
        particle_array[slotNo].thickness = x*unit;
    }

    if ( command == fTargetPresent ){
        targetPresent = fTargetPresent->GetNewBoolValue(newValue);
    }

    if ( command == fFramePresent ){
        framePresent = fFramePresent->GetNewBoolValue(newValue);
    }

    if ( command == fTargetChamberPresent )
        targetChamberPresent = fTargetChamberPresent->GetNewBoolValue(newValue);
}

G4String DetectorSetupMessenger::GetCurrentValue(G4UIcommand *command)
{
    for ( auto v : commands ){
        if ( command == v )
            return command->GetCurrentValue();
    }
    return "";
}