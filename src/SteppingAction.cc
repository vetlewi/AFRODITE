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

SteppingAction::SteppingAction(EventAction* eventAction)
    : G4UserSteppingAction()
    , fEventAction(eventAction){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    const G4TouchableHandle& theTouchable = preStepPoint->GetTouchableHandle();
    
    // get interaction time of the current step
    G4double interactiontime = preStepPoint->GetGlobalTime()/ns;
    
    // get volume of the current step
    G4VPhysicalVolume* volume = theTouchable->GetVolume();
    
    // get volume name of the current step
    const G4String& volumeName = volume->GetName();
    
    
    ////////////////////////////////////////////////
    ///                 CLOVERS                  ///
    ////////////////////////////////////////////////
    
    if((interactiontime < CLOVER_TotalSampledTime) && (volumeName == "CLOVER_HPGeCrystal"))
    {
        //G4int CLOVERNo = volume->GetCopyNo()/4;
        //G4int CLOVER_HPGeCrystalNo = channelID%4;
        fEventAction->CLOVER_energy[volume->GetCopyNo()/4] +=aStep->GetTotalEnergyDeposit()/keV;
    }
    
    if((interactiontime < CLOVER_Shield_BGO_TotalSampledTime) && (volumeName == "CLOVER_Shield_BGOCrystal"))
    {
        //G4int CLOVER_BGOCrystalNo = volume->GetCopyNo()%16;
        fEventAction->BGO_energy[volume->GetCopyNo()/16] += aStep->GetTotalEnergyDeposit()/keV;

    }


    ////////////////////////////////////////////////
    ///         OCL LABR DETECTORS FROM          ///
    ////////////////////////////////////////////////
    
    
    if((interactiontime < OCLLABR_TotalSampledTime) && (volumeName == "OCL_Crystal"))
    {
        fEventAction->OCLLABR_energy[volume->GetCopyNo()] += aStep->GetTotalEnergyDeposit()/keV;
    }

    ////////////////////////////////////////////////
    ///         FTA LABR DETECTORS FROM          ///
    ////////////////////////////////////////////////

    if ((interactiontime < FTALABR_TotalSampledTime) && (volumeName == "FTA_Crystal"))
    {
        G4int did = volume->GetCopyNo();
        fEventAction->FTALABR_energy[did] += aStep->GetTotalEnergyDeposit()/keV;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
