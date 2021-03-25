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
#include <FTFP_BERT.hh>

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <thread>
#include <cxxopts.hpp>
#include <termcolor/termcolor.hpp>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    cxxopts::Options options("AFRODITE", "A Geant4 model of the AFRODITE array");
    options.add_options()
            ("m,macro", "Macro file", cxxopts::value<std::string>())
            ("t,threads", "Number of worker threads", cxxopts::value<int>())
            ("h,help", "Print usage");

    G4UIExecutive* ui = 0;
    std::string macro = "";
    G4int threads = std::thread::hardware_concurrency();
    if ( argc == 1 ) {
        ui = new G4UIExecutive(argc, argv);
    } else {
        try {
            auto result = options.parse(argc, argv);
            if ( result.count("help") ){
                std::cout << options.help() << std::endl;
                return 0;
            }
            macro = result["macro"].as<std::string>();
            threads = result["threads"].as<int>();
        } catch (const std::exception &e){
            std::cerr << termcolor::bold << termcolor::red;
            std::cerr << "Error: " << e.what() << termcolor::reset << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }
    }

    // Choose the Random engine
    //
    CLHEP::RanluxEngine defaultEngine( 1234567, 4 );
    G4Random::setTheEngine( &defaultEngine );
    G4int seed = time( NULL );
    G4Random::setTheSeed( seed );

    auto *runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    runManager->SetNumberOfThreads(threads);

    // Initialize classes
    runManager->SetUserInitialization(new DetectorConstruction);

    ////////////////////////////////////////////////////////////////////
    //      Initialising the Physics List with Radioactive Decay
    ////////////////////////////////////////////////////////////////////

    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization);

    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if ( !ui ) {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    //
    delete visManager;
    delete runManager;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
