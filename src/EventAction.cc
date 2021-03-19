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

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: G4UserEventAction(),
CLOVER_energy{-9.,-9.,-9.,-9.,-9.,-9.,-9.,-9.,-9.},
BGO_energy{-9.,-9.,-9.,-9.,-9.,-9.,-9.,-9.,-9.},
OCLLABR_energy{-9.,-9.},
FTALABR_energy{-9.,-9.,-9.,-9.,-9.,-9.}
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    ////////////////////////////////////////////////////////////////////
    ////    Flushing all the arrays and resetting the boolean tests
    
    evtNb = evt->GetEventID();

    for(G4int i=0; i<9; i++)
    {
        for (G4int k=0; k<CLOVER_TotalTimeSamples; k++)
        {
            CLOVER_EDep[i][k] = 0;
            
            for(G4int j=0; j<4; j++)
            {
                CLOVER_HPGeCrystal_EDep[i][j][k] = 0;
                CLOVER_BGO_EDep[i][j][k] = 0;
                CLOVER_HPGeCrystal_EDepVETO[i][j][k] = false;
            }
        }
        
        for (G4int k=0; k<CLOVER_Shield_BGO_TotalTimeSamples; k++)
        {
            for(G4int l=0; l<16; l++)
            {
                CLOVER_BGO_EDep[i][l][k] = 0;
            }
        }
    }
    
    
    
    //Initializing LABR-array
    for(G4int i=0; i<2; i++)
    {
        for (G4int k=0; k< OCLLABR_TotalSampledTime; k++)
        {
            OCLLABR_EDep[i][k] = 0.;
        }
    }

    for(G4int i=0; i<6; i++)
    {
        for (G4int k=0; k< FTALABR_TotalSampledTime; k++)
        {
            FTALABR_EDep[i][k] = 0.;
        }
    }
    
    for(G4int i=0; i<9; i++) CLOVER_energy[i]=0.0;
    for(G4int i=0; i<9; i++) BGO_energy[i]=0.0;
    for(G4int i=0; i<2; i++) OCLLABR_energy[i]=0.0;
    for(G4int i=0; i<6; i++) FTALABR_energy[i]=0.0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
    // Accumulate statistics
    //
    
    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    //Fill ntuple here
    for(G4int k=0; k<9; k++)analysisManager->FillNtupleDColumn(k,CLOVER_energy[k]);
    for(G4int k=0; k<9; k++)analysisManager->FillNtupleDColumn(k+9,BGO_energy[k]);
    for(G4int k=0; k<2; k++)analysisManager->FillNtupleDColumn(k+18,OCLLABR_energy[k]);
    for(G4int k=0; k<6; k++)analysisManager->FillNtupleDColumn(k+20,FTALABR_energy[k]);
    analysisManager->AddNtupleRow(0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
