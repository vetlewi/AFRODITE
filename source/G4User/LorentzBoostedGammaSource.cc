//
// Created by Vetle Wegner Ingeberg on 21/12/2023.
//

#include "user/LorentzBoostedGammaSource.hh"

#include <globals.hh>
#include <Randomize.hh>

#include <G4ParticleGun.hh>
#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>


LorentzBoostedGammaSource::LorentzBoostedGammaSource(G4ParticleGun *gun, G4double energy, G4ThreeVector velocity)
    : fParticleGun( gun )
    , fEnergy( energy )
    , fBeta( velocity.mag() / c_light )
    , fGamma( 1/sqrt(1  - fBeta*fBeta) )
    , fVelNormed( velocity.unit() )
{
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particleDefinition);
}

void LorentzBoostedGammaSource::GeneratePrimaries(G4Event *anEvent)
{
    // First we generate the direction since we will always need a direction. Easier w/r/t branching I would imagine
    double theta = M_PI * G4UniformRand();
    double phi = 2 * M_PI * G4UniformRand();

    double x = std::sin(theta)*std::cos(phi);
    double y = std::sin(theta)*std::sin(phi);
    double z = std::cos(theta);

    G4ThreeVector eventVertex(x, y, z);

    fParticleGun->SetParticleMomentumDirection(eventVertex);

    double scattering_angle = fVelNormed.angle(eventVertex);

    double EnergyLab = fEnergy/(fGamma*(1 - fBeta*cos(scattering_angle)));

    fParticleGun->SetParticleEnergy(EnergyLab);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}