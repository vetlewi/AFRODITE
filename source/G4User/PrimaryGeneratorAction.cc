//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//      ----------------------------------------------------------------
//                          AFRODITE (iThemba Labs)
//      ----------------------------------------------------------------
//
//      Github repository: https://www.github.com/KevinCWLi/AFRODITE
//
//      Main Author:    K.C.W. Li
//
//      email: likevincw@gmail.com
//
#include "user/PrimaryGeneratorAction.hh"
#include "user/PrimaryGeneratorActionMessenger.hh"

#include <Randomize.hh>

#include <G4Event.hh>
#include <G4ParticleGun.hh>

#include <G4ParticleTable.hh>
#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : fParticleGun( new G4ParticleGun( 1 ) )
    , fMessenger( new PrimaryGeneratorActionMessenger(this) )
    , fEnergy( 1332.492 * keV )
    , fBeta( 0. )
    , fGamma( 1/sqrt(1 - fBeta*fBeta) )
    , fVelVertex( G4ThreeVector(0,0,0) )
{


    // Default values
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particleDefinition);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fMessenger;
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    // First we generate the direction since we will always need a direction. Easier w/r/t branching I would imagine
    double theta = M_PI * G4UniformRand();
    double phi = 2 * M_PI * G4UniformRand();

    G4ThreeVector eventVertex(std::sin(theta)*std::cos(phi), std::sin(theta)*std::sin(phi), std::cos(theta));

    fParticleGun->SetParticleMomentumDirection(eventVertex);

    double scattering_angle = fVelVertex.angle(eventVertex);
    double labEnergy = fEnergy/(fGamma*(1 - fBeta*cos(scattering_angle)));

   //create vertex
   fParticleGun->SetParticleEnergy(labEnergy);
   fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::SetEnergy(const G4double &energy)
{
    fEnergy = energy;
}

void PrimaryGeneratorAction::SetVelocity(const G4ThreeVector &velocity)
{
    fBeta = velocity.mag();
    fGamma = 1/sqrt(1 - fBeta*fBeta);
    fVelVertex = velocity.unit();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

