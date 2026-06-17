//
// Created by Vetle Wegner Ingeberg on 21/12/2023.
//

#ifndef LORENTZBOOSTEDGAMMASOURCEMESSENGER_HH
#define LORENTZBOOSTEDGAMMASOURCEMESSENGER_HH

class PrimaryGeneratorAction;
class G4UIcmdWith3Vector;
class G4UIcmdWithADoubleAndUnit;

#include <G4UImessenger.hh>

class PrimaryGeneratorActionMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction *source);
    ~PrimaryGeneratorActionMessenger();

    void SetNewValue(G4UIcommand *command, G4String newValue);
    G4String GetCurrentValue(G4UIcommand *command);

private:
    PrimaryGeneratorAction *fSource;

    G4UIdirectory *sourceDirectory;
    G4UIcmdWithADoubleAndUnit *gammaEnergy;
    G4UIcmdWith3Vector *sourceVelocity;

};

#endif //LORENTZBOOSTEDGAMMASOURCEMESSENGER_HH
