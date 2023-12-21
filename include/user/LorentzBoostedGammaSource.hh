//
// Created by Vetle Wegner Ingeberg on 21/12/2023.
//

#ifndef LORENTZBOOSTEDGAMMASOURCE_H
#define LORENTZBOOSTEDGAMMASOURCE_H

#include <globals.hh>
#include <G4ThreeVector.hh>

class G4ParticleGun;
class G4Event;

class LorentzBoostedGammaSource
{

public:
    LorentzBoostedGammaSource(G4ParticleGun *, G4double, G4ThreeVector);
    ~LorentzBoostedGammaSource() = default;

    void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *fParticleGun;
    G4double fEnergy;
    G4double fBeta, fGamma;
    G4ThreeVector fVelNormed;
};



#endif // LORENTZBOOSTEDGAMMASOURCE_H
