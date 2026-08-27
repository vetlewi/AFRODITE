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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include <G4VUserDetectorConstruction.hh>
#include <G4RotationMatrix.hh>
#include <G4Transform3D.hh>

#include "Constants.hh"
#include "Frame/DetectorFrame.hh"


class G4VPhysicalVolume;
class DetectorSetupMessenger;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

public:
    G4VPhysicalVolume* Construct() override;

private:
    // methods
    //
    G4VPhysicalVolume* DefineVolumes();

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

    /////////////////////////////
    //          WORLD
    G4double WorldSize;

    /////////////////////////////////////
    //  Runtime-configurable detector frame (slots) and its UI messenger.
    //  All slots are empty by default; populate them via /AFRODITE/ macros.
    /////////////////////////////////////
    DetectorFrame           fFrame;
    DetectorSetupMessenger *fMessenger;

    /////////////////////////////////////
    //          S2 SILICON DETECTORS
    //  On-axis particle telescopes; not part of the frame slots.
    /////////////////////////////////////
    G4bool              S2_Silicon_AllPresent_Override;
    G4bool              S2_Silicon_AllAbsent_Override;
    G4bool              S2_Silicon_Presence[numberOfSi];
    G4double            S2_Silicon_Distance[numberOfSi];
    G4double            S2_Silicon_Thickness[numberOfSi];
    G4RotationMatrix    S2_Silicon_rotm[numberOfSi];
    G4Transform3D       S2_Silicon_transform[numberOfSi];
    G4ThreeVector       S2_Silicon_position[numberOfSi];
    G4double            S2_Silicon_phi[numberOfSi];
    G4double            S2_Silicon_theta[numberOfSi];

    //////////////////////////////////////
    //          Vacuum Chamber
    //////////////////////////////////////

    G4VPhysicalVolume* PhysiVacuumChamber;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

