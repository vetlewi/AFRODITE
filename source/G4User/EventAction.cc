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

#include "user/EventAction.hh"
#include "user/RunAction.hh"
#include "user/Analysis.hh"
#include "user/DetectorConstruction.hh"

#include <G4RunManager.hh>
#include <G4Event.hh>

#include <algorithm>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
    : G4UserEventAction()
    , geometry( reinterpret_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()) )
    , CLOVER_energy( geometry->GetNumClover() * numberOf_CLOVER_Crystals )
    , BGO_energy( geometry->GetNumClover() )
    , OCLLABR_energy( geometry->GetNumOCL() )
    , FTALABR_energy( geometry->GetNumFTA() )
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
    ////////////////////////////////////////////////////////////////////
    ////    Flushing all the arrays
    ////////////////////////////////////////////////////////////////////
    CLOVER_energy.reset();
    BGO_energy.reset();
    OCLLABR_energy.reset();
    FTALABR_energy.reset();

#if ANALYZE_SI_DETECTORS
    memset(DeltaE_ring_energy, 0, sizeof(DeltaE_ring_energy));
    memset(DeltaE_sector_energy, 0, sizeof (DeltaE_sector_energy));
#endif // ANALYZE_SI_DETECTORS
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *)
{

    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();


    //Fill ntuple here
    G4int idx = 0;
    for ( auto &clover : CLOVER_energy)
        analysisManager->FillNtupleDColumn(idx++, clover);
    for ( auto &bgo : BGO_energy )
        analysisManager->FillNtupleDColumn(idx++, bgo);
    for ( auto &ocl_labr : OCLLABR_energy)
        analysisManager->FillNtupleDColumn(idx++, ocl_labr);
    for ( auto &fta_labr : FTALABR_energy )
        analysisManager->FillNtupleDColumn(idx++, fta_labr);
#if ANALYZE_SI_DETECTORS
    for ( auto &ring : DeltaE_ring_energy )
        analysisManager->FillNtupleDColumn(idx++, ring);
    for ( auto &sect : DeltaE_sector_energy )
        analysisManager->FillNtupleDColumn(idx++, sect);
#endif // ANALYZE_SI_DETECTORS
    analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
