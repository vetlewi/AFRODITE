//
// Created by Vetle Wegner Ingeberg on 21/12/2023.
//

#include "user/PrimaryGeneratorActionMessenger.hh"
#include "user/PrimaryGeneratorAction.hh"

#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4SystemOfUnits.hh>

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction *source)
    : fSource( source )
    , sourceDirectory( new G4UIdirectory("/source/") )
    , gammaEnergy( new G4UIcmdWithADoubleAndUnit("/source/energy", this) )
    , sourceVelocity( new G4UIcmdWith3Vector("/source/velocity", this) )
{
    sourceDirectory->SetGuidance("Gamma source control commands.");
    gammaEnergy->SetGuidance("Set rest frame energy of the gammas emitted by the gamma source");
    gammaEnergy->SetDefaultUnit("keV");
    gammaEnergy->SetUnitCandidates("eV keV MeV GeV TeV");
    gammaEnergy->SetDefaultValue(1332.492 * keV);
    gammaEnergy->SetParameterName("Eg", false);
    gammaEnergy->SetRange("Eg > 0");
    sourceVelocity->SetGuidance("Set velocity of the gamma source");
    sourceVelocity->SetParameterName("Vx", "Vy", "Vz", false);
    //sourceVelocity->SetRange("Vx*Vx + Vy*Vy + Vz*Vz < 1");

}

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger()
{
    delete sourceVelocity;
    delete gammaEnergy;
    delete sourceDirectory;
};

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if ( command == gammaEnergy ){
        fSource->SetEnergy(gammaEnergy->GetNewDoubleValue(newValue));
    } else if ( command == sourceVelocity ){
        fSource->SetVelocity(sourceVelocity->GetNew3VectorValue(newValue));
    } else {
        // Should emit an exception here...
    }
}

G4String PrimaryGeneratorActionMessenger::GetCurrentValue(G4UIcommand *command)
{
    G4String cv;
    if ( command == gammaEnergy )
        cv = gammaEnergy->ConvertToStringWithBestUnit(fSource->GetEnergy());
    else if ( command == sourceVelocity )
        cv = sourceVelocity->ConvertToString(fSource->GetVelocity());
    return cv;
}