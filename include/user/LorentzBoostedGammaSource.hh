//
// Created by Vetle Wegner Ingeberg on 21/12/2023.
//

#ifndef LORENTZBOOSTEDGAMMASOURCE_H
#define LORENTZBOOSTEDGAMMASOURCE_H

#include <globals.hh>
#include <G4ThreeVector.hh>
#include <G4PhysicalConstants.hh>

class G4ParticleGun;
class G4Event;

class LorentzBoostedGammaSource
{

public:
    LorentzBoostedGammaSource(G4ParticleGun *, G4double, G4ThreeVector);
    ~LorentzBoostedGammaSource() = default;

    void GeneratePrimaries(G4Event *);

    void SetEnergy(const G4double &energy);
    G4double GetEnergy() const { return fEnergy; }
    void SetVelocity(const G4ThreeVector &velocity);
    G4ThreeVector GetVelocity() const { return fVelNormed * fBeta * c_light; }

private:
    G4ParticleGun *fParticleGun;
    G4double fEnergy;
    G4double fBeta, fGamma;
    G4ThreeVector fVelNormed;
};



#endif // LORENTZBOOSTEDGAMMASOURCE_H
