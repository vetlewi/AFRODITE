#include "user/Co60SourceGenerator.hh"

#include <cmath>
#include <vector>

#include <G4ThreeVector.hh>
#include <G4ParticleGun.hh>
#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

#define NUMBER_OF_POINTS 1000001 // We will use 1M samples to generate the PDF



std::vector<double> get_distribution(int num_samples)
{
    std::vector<double> samples;
    // We need to generate num_samples between 0 and pi
    double dx = M_PI/num_samples;
    for ( int i = 0 ; i < num_samples ; ++i ){
        double cosx = std::cos(i*dx);
        double value = 1 + cosx*cosx/8.0 + cosx*cosx*cosx*cosx/24.0;
        samples.push_back(value);
    }
    return samples;
}


Co60EventGenerator::Co60EventGenerator(G4ParticleGun *gun)
    : fParticleGun( gun )
    , distribution( get_distribution( NUMBER_OF_POINTS ) )
    , randAngularDist( distribution.data(), distribution.size(), 0 )
{
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particleDefinition);
}

double Co60EventGenerator::randAng() {
    return randAngularDist.fire() * M_PI; // Should be tested...
}

void Co60EventGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // 1st step: Determine what level is initially populated.
    // Only two possible levels can be populated, either the 2505.748 or the 1332.508 level.
    // Probability is 99.88 for the 2505.748 keV level.

    // First we generate the direction since we will always need a direction. Easier w/r/t branching I would imagine
    double theta = M_PI * G4UniformRand();
    double phi = 2 * M_PI * G4UniformRand();

    double x = std::sin(theta)*std::cos(phi);
    double y = std::sin(theta)*std::sin(phi);
    double z = std::cos(theta);

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x, y, z));

    // Generate a random number between 0 and 1. If less or equal to 0.12 then we populate the 1332.508 keV level
    if ( G4UniformRand() <= 0.0012 ){ // Populate only the 1332.508 keV. Will only emit the 1332.501 keV gamma ray
        fParticleGun->SetParticleEnergy(1332.501 * keV);
        fParticleGun->GeneratePrimaryVertex(anEvent);
        return;
    }

    // If we reach this point it means that we need to generate a second gamma ray. We will ignore the small branching
    // to the 2158.632 keV state and the ground state as these have intensities of 0.0076 and 2 * 10-4, respectively.
    // First we will generate the first gamma ray. That has an energy of 1173.228 keV
    fParticleGun->SetParticleEnergy(1173.228 * keV);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    // Now we will generate the angles of the second gamma ray
    double alpha = randAng(); // Will return an angle between 0 and pi.
    double beta = 2*M_PI*G4UniformRand();

    x = std::sin(theta)*std::cos(phi)*std::cos(alpha) + std::cos(theta)*std::cos(phi)*std::sin(alpha)*std::cos(beta);
    x -= std::sin(phi)*std::sin(alpha)*std::sin(beta);

    y = std::sin(theta)*std::sin(phi)*std::cos(alpha) + std::cos(theta)*std::sin(phi)*std::sin(alpha)*std::cos(beta);
    y += std::cos(phi)*std::sin(alpha)*std::sin(beta);

    z = std::cos(theta)*std::cos(alpha) - std::sin(theta)*std::sin(alpha)*std::cos(beta);

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x, y, z));
    fParticleGun->SetParticleEnergy(1332.501 * keV);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}