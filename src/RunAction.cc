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

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction()
, fMessenger( new G4GenericMessenger(this, "/FilePath/", "Output file path") )
, fOutput_file( "AFRODITE.root" )
{
    G4GenericMessenger::Command& outputCmd =
            fMessenger->DeclareProperty("output", fOutput_file, "Path to output file");
    outputCmd.SetDefaultValue("AFRODITE.root");

    // set printing event number per 10 000 event
    G4RunManager::GetRunManager()->SetPrintProgress(10000);
    
    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in Analysis.hh
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;
    
    // Create directories
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(1);
    //analysisManager->SetFirstHistoId(1);
    
    // Book histograms, ntuple
   
    // Creating ntuple
    analysisManager->CreateNtuple("DataTreeSim", "AFRODITE and OCL detectors");
    
    ////////////////////////////////////////////////////
    ////    CLOVER Detectors
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy0");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy1");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy2");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy3");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy4");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy5");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy6");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy7");
    analysisManager->CreateNtupleDColumn(0, "CLOVER_Energy8");
    
     ////////////////////////////////////////////////////
    ////    BGO Detectors
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy0");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy1");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy2");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy3");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy4");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy5");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy6");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy7");
    analysisManager->CreateNtupleDColumn(0, "BGO_Energy8");
    
    ////////////////////////////////////////////////////
    ////    OCL LABR Detectors

    analysisManager->CreateNtupleDColumn(0, "OCLLABR_Energy0");
    analysisManager->CreateNtupleDColumn(0, "OCLLABR_Energy1");

    ////////////////////////////////////////////////////
    ////    FTA LABR Detectors

    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy0");
    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy1");
    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy2");
    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy3");
    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy4");
    analysisManager->CreateNtupleDColumn(0, "FTALABR_Energy5");
    
    analysisManager->FinishNtuple(0);
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    //inform the runManager to save random number seed
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Open an output file
    //
    G4String fileName = fOutput_file;
    //Requesting filename from terminal. This is because it takes so long to load the geometry and I want to be able to change files without having to reload the geometry.
    //Comment out the line below if you want to just use the file name "AFRODITE.root"
    // G4cin>>fileName;
    G4cout << "Writing to file: " << fileName << G4endl;
    analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
    
    // print histogram statistics
    //
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    

    
    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
