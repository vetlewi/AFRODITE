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

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                               const DetectorConstruction* detectorConstruction,
                               EventAction* eventAction)
: G4UserSteppingAction(),
fDetConstruction(detectorConstruction),
fEventAction(eventAction)
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    
    // get particle name/definition
    //G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    //G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    
    // get particle lifetime
    //G4double lifetime = aStep->GetTrack()->GetDefinition()->GetIonLifeTime()/ns;
    
    // get interaction time of the current step
    interactiontime = preStepPoint->GetGlobalTime()/ns;
    
    // get volume of the current step
    G4VPhysicalVolume* volume = theTouchable->GetVolume();
    
    // get volume name of the current step
    volumeName = volume->GetName();
    
    
    //G4cout << "Here is the particleName    "<< particleName << G4endl;
    //G4cout << "Here is the lifetime    "<< lifetime << G4endl;
    //G4cout << "Here is the interactiontime    "<< interactiontime << G4endl;
    //G4cout << "Here is the TEST    "<< TEST << G4endl;
    //G4cout << "Here is the interactiontime    "<< interactiontime << G4endl;
    //G4cout << "                                "<< G4endl;
    
    
    
    ////////////////////////////////////////////////
    //                  CLOVERS
    ////////////////////////////////////////////////
    
    if((interactiontime < CLOVER_TotalSampledTime) && (volumeName == "CLOVER_HPGeCrystal"))
    {
        channelID = volume->GetCopyNo();
        
        CLOVERNo = channelID/4;
        CLOVER_HPGeCrystalNo = channelID%4;
        
        /*
         G4cout << "Here is the copyNo    "<< copyNo << G4endl;
         G4cout << "Here is the CLOVERNo    "<< CLOVERNo << G4endl;
         G4cout << "Here is the CLOVER_HPGeCrystalNo    "<< CLOVER_HPGeCrystalNo << G4endl;
         G4cout << " "<< G4endl;
         */
        
        iTS = interactiontime/CLOVER_SamplingTime;
        edepCLOVER_HPGeCrystal = aStep->GetTotalEnergyDeposit()/keV;

        fEventAction->CLOVER_energy[CLOVERNo] +=edepCLOVER_HPGeCrystal; 

        // G4cout<<"We are inside volume "<<volumeName<<" ID is "<<channelID<<" time "<<iTS<<" energy "<<edepCLOVER_HPGeCrystal<<G4endl;
        
        // fEventAction->AddEnergyCLOVER_HPGeCrystal(CLOVERNo, CLOVER_HPGeCrystalNo, iTS, edepCLOVER_HPGeCrystal);
    }
    
    if((interactiontime < CLOVER_Shield_BGO_TotalSampledTime) && (volumeName == "CLOVER_Shield_BGOCrystal"))
    {
        channelID = volume->GetCopyNo();
        
        CLOVER_BGOCrystalNo = channelID%16;
        
        edepCLOVER_BGOCrystal= aStep->GetTotalEnergyDeposit()/keV;


        fEventAction->BGO_energy[CLOVERNo] +=edepCLOVER_BGOCrystal; 

        // G4cout<<"We are inside volume "<<volumeName<<" ID is "<<channelID<<" time "<<iTS<<" energy "<<edepCLOVER_HPGeCrystal<<G4endl;
        
        // fEventAction->AddEnergyCLOVER_HPGeCrystal(CLOVERNo, CLOVER_HPGeCrystalNo, iTS, edepCLOVER_HPGeCrystal);
    }


    ////////////////////////////////////////////////
    //              LABR DETECTORs FROM OCL
    ////////////////////////////////////////////////
    
    
    if((interactiontime < LABR_TotalSampledTime) && (volumeName == "Crystal"))
    {
        point = aStep->GetPreStepPoint();
        touch = point->GetTouchableHandle();

        depth = 5;                        // Copy nr in caes it really is the DetectorVolume
        GrandMotherPhysicalName = touch->GetVolume(depth)->GetName();
        LABRNo = touch->GetCopyNumber(depth); // of depth=5 mother: "OCLDetector"
        
        if (GrandMotherPhysicalName =="OCLDetector") {
            iTS = interactiontime/LABR_SamplingTime;
            edepLABR_Crystal = aStep->GetTotalEnergyDeposit()/keV;

            fEventAction->LABR_energy[LABRNo] += edepLABR_Crystal;
            
            // fEventAction->AddEnergyLABR_Crystal(LABRNo, iTS, edepLABR_Crystal);
// 
            // G4cout<<"We are inside volume "<<volumeName<<" ID is "<<LABRNo<<" time "<<iTS<<" energy "<<edepLABR_Crystal<<G4endl;
            // G4cout<<"The function: "<< (fEventAction->AddEnergyLABR_Crystal(channelID, iTS, edepLABR_Crystal))<<G4endl;
        }
    }

    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
