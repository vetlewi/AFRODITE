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

#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include <algorithm>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction() : G4UserEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
    ////////////////////////////////////////////////////////////////////
    ////    Flushing all the arrays
    memset(CLOVER_energy, 0, sizeof(CLOVER_energy));
    memset(BGO_energy, 0, sizeof(CLOVER_energy));
    memset(OCLLABR_energy, 0, sizeof(CLOVER_energy));
    memset(FTALABR_energy, 0, sizeof(CLOVER_energy));
#if ANALYZE_SI_DETECTORS
    memset(DeltaE_ring_energy, 0, sizeof(DeltaE_ring_energy));
    memset(DeltaE_sector_energy, 0, sizeof (DeltaE_sector_energy));
#endif // ANALYZE_SI_DETECTORS
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *e)
{

    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Get all hits

    e->GetHCofThisEvent()->GetNumberOfCollections();

    //Fill ntuple here
    G4int idx = 0;
    for ( auto &clover : CLOVER_energy)
        analysisManager->FillNtupleDColumn(idx++, clover);
    for ( auto &bgo : BGO_energy )
        analysisManager->FillNtupleDColumn(idx++, bgo);
    for ( auto &labr : OCLLABR_energy)
        analysisManager->FillNtupleDColumn(idx++, labr);
    for ( auto &labr : FTALABR_energy )
        analysisManager->FillNtupleDColumn(idx++, labr);
#if ANALYZE_SI_DETECTORS
    for ( auto &ring : DeltaE_ring_energy )
        analysisManager->FillNtupleDColumn(idx++, ring);
    for ( auto &sect : DeltaE_sector_energy )
        analysisManager->FillNtupleDColumn(idx++, sect);
#endif // ANALYZE_SI_DETECTORS
    analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
