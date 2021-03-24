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


#include "OCLLaBr3.hh"
#include "FTALaBr3.hh"
#include "Constants.hh"


class G4VPhysicalVolume;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override = default;
    
public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    
    /////////////////////////////
    //          WORLD
    G4double WorldSize;
    
    
    /////////////////////////////////////
    //          CLOVER DETECTORS
    /////////////////////////////////////
    
    G4bool              CLOVER_AllPresent_Override;
    G4bool              CLOVER_AllAbsent_Override;
    G4bool              CLOVER_Presence[numberOf_CLOVER];
    G4double            CLOVER_Distance[numberOf_CLOVER];
    G4RotationMatrix    CLOVER_rotm[numberOf_CLOVER];
    G4Transform3D       CLOVER_transform[numberOf_CLOVER];
    G4ThreeVector       CLOVER_position[numberOf_CLOVER];
    G4double            CLOVER_phi[numberOf_CLOVER];
    G4double            CLOVER_theta[numberOf_CLOVER];
    
    ///////////////////////////////////////////////////////////////
    //          CLOVER - BGO Shield   (Manufacturer: Cyberstar)
    ///////////////////////////////////////////////////////////////
    
    G4bool              CLOVER_Shield_AllPresent_Override;
    G4bool              CLOVER_Shield_AllAbsent_Override;
    G4bool              CLOVER_Shield_Presence[numberOf_CLOVER_Shields];
    
    
    /////////////////////////////////////
    //          OCL LaBr3 DETECTORS
    /////////////////////////////////////
    
    OCLLaBr3*           labr3[numberOf_OCLLaBr3];

    G4bool              OCLLaBr3_AllPresent_Override;
    G4bool              OCLLaBr3_AllAbsent_Override;
    G4bool              OCLLaBr3_Presence[numberOf_OCLLaBr3];
    G4double            OCLLaBr3_Distance[numberOf_OCLLaBr3];
    G4RotationMatrix    OCLLaBr3_rotm[numberOf_OCLLaBr3];
    G4Transform3D       OCLLaBr3_transform[numberOf_OCLLaBr3];
    G4ThreeVector       OCLLaBr3_position[numberOf_OCLLaBr3];
    G4double            OCLLaBr3_phi[numberOf_OCLLaBr3];
    G4double            OCLLaBr3_theta[numberOf_OCLLaBr3];

    /////////////////////////////////////
    //          FTA LaBr3 DETECTORS
    /////////////////////////////////////

    FTALaBr3*           ftalabr3[numberOf_FTALaBr3];

    G4bool              FTALaBr3_AllPresent_Override;
    G4bool              FTALaBr3_AllAbsent_Override;
    G4bool              FTALaBr3_Presence[numberOf_FTALaBr3];
    G4double            FTALaBr3_Distance[numberOf_FTALaBr3];
    G4RotationMatrix    FTALaBr3_rotm[numberOf_FTALaBr3];
    G4Transform3D       FTALaBr3_transform[numberOf_FTALaBr3];
    G4ThreeVector       FTALaBr3_position[numberOf_FTALaBr3];
    G4double            FTALaBr3_phi[numberOf_FTALaBr3];
    G4double            FTALaBr3_theta[numberOf_FTALaBr3];
    

    //////////////////////////////////////
    //          Vacuum Chamber
    //////////////////////////////////////

    G4VPhysicalVolume* PhysiVacuumChamber;

    ////////////////////////////////
    ////        STRUCTURES      ////
    ////////////////////////////////
    
    /////////////////////////////////////
    //  K600 BACTAR Target Chamber
    G4bool      AFRODITE_MathisTC_Presence;
    
    /////////////////////////////////////
    //  K600 Target
    G4bool      AFRODITE_Target_Presence;
    
    
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

