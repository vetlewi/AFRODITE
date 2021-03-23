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
//      Main program of the AFRODITE code (PR239A branch)
//

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"


#include <G4RunManagerFactory.hh>

#include <G4UImanager.hh>
#include <G4UIcommand.hh>

#include <G4PhysListFactory.hh>
#include <G4RadioactiveDecayPhysics.hh>

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " AFRODITE [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
        G4cerr << "   note: -t option is available only for multi-threaded mode."
               << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    // Evaluate arguments
    //
    if ( argc > 7 ) {
        PrintUsage();
        return 1;
    }

    G4String macro;
    G4String session;
    G4int nThreads = 0;
    for ( G4int i=1; i<argc; i=i+2 ) {
        if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
        else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
        else if ( G4String(argv[i]) == "-t" ) {
            nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
        }
        else {
            PrintUsage();
            return 1;
        }
    }

    // Choose the Random engine
    //
    CLHEP::RanluxEngine defaultEngine( 1234567, 4 );
    G4Random::setTheEngine( &defaultEngine );
    G4int seed = time( NULL );
    G4Random::setTheSeed( seed );

    auto *runManager = G4RunManagerFactory::CreateRunManager();
    runManager->SetUserInitialization(new DetectorConstruction);

    if ( nThreads == 0 )
        runManager->SetNumberOfThreads(1);
    else
        runManager->SetNumberOfThreads(nThreads);

    ////////////////////////////////////////////////////////////////////
    //      Initialising the Physics List with Radioactive Decay
    ////////////////////////////////////////////////////////////////////

    G4PhysListFactory factory;
    auto *phys = factory.GetReferencePhysList("QGSP_BERT");
    phys->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(phys);
    runManager->SetUserInitialization(new ActionInitialization);

    // Initialize G4 kernel
    //
    runManager->Initialize();

    auto *visManager = new G4VisExecutive;
    visManager->Initialize();

    auto *uiManager = G4UImanager::GetUIpointer();

    if ( !macro.empty() ) {
        // batch mode
        G4String command = "/control/execute ";
        uiManager->ApplyCommand(command+macro);
    } else {
        // interactive mode : define UI session
#ifdef G4UI_USE
        auto *ui = new G4UIExecutive(argc, argv, session);
#ifdef G4VIS_USE
        uiManager->ApplyCommand("/control/execute init_vis.mac");
#else
        uiManager->ApplyCommand("/control/execute init.mac");
#endif // G4VIS_USE
        if (ui->IsGUI())
            uiManager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
#endif // G4UI_USE
    }

    G4cout << "Segfaults after this message are not a problem" << G4endl;
#ifdef G4VIS_USE
    delete visManager;
#endif // G4VIS_USE
    delete runManager;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
