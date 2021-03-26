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

#include <cstring>

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
    // get volume of the current step
    G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    // get volume name of the current step
    const char *volumeName = volume->GetName().c_str();


    if ( strcmp(volumeName, "CLOVER_HPGeCrystal") == 0 ){
        fEventAction->CLOVER_energy[volume->GetCopyNo()/4] +=aStep->GetTotalEnergyDeposit()/keV;
    } else if ( strcmp(volumeName, "CLOVER_Shield_BGOCrystal") == 0 ){
        fEventAction->BGO_energy[volume->GetCopyNo()/16] += aStep->GetTotalEnergyDeposit()/keV;
    } else if ( strcmp(volumeName, "OCL_Crystal") == 0 ){
        fEventAction->OCLLABR_energy[volume->GetCopyNo()] += aStep->GetTotalEnergyDeposit()/keV;
    } else if ( strcmp(volumeName, "LaBr3_Crystal_Physical") == 0 ){
        fEventAction->FTALABR_energy[volume->GetCopyNo()] += aStep->GetTotalEnergyDeposit()/keV;
    } else if ( strcmp(volumeName, "Active_Si_area") == 0 ){

        // TODO: Find ring/sector from angle.
        // TODO: Add results to tree


    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
