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

#include <CADMesh/CADMesh.hh>
#include "DetectorConstruction.hh"


#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Trd.hh"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"


#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"


#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "CloverFactory.hh"


#include <fstream>
#include <string>

#ifndef SRC_PATH
#define SRC_PATH "../"
#endif // SRC_PATH

#ifndef PLY_PATH
#define PLY_PATH SRC_PATH"Mesh-Models"
#endif // PLY_PATH

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
    , fCheckOverlaps(false)
    , WorldSize( 5. * m )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    
    //////////////////////////////////////////////////////////
    ////                                                  ////
    ////            DETECTOR ARRAY POSITIONING            ////
    ////                                                  ////
    //////////////////////////////////////////////////////////
    
    ////////////////////////////
    ////    CLOVER SETUP
    
    CLOVER_AllPresent_Override = false;
    CLOVER_AllAbsent_Override = false;
    
    CLOVER_Shield_AllPresent_Override = false;
    CLOVER_Shield_AllAbsent_Override = false;
    
    
    //  CLOVER 1
    CLOVER_Presence[0] = false;
    CLOVER_Shield_Presence[0] = false;
    CLOVER_Distance[0] = 14.*cm;
    CLOVER_phi[0] = 0*deg;
    CLOVER_theta[0] = 90*deg;
    CLOVER_rotm[0].rotateY(-90.*deg);
    
    //  CLOVER 2
    CLOVER_Presence[1] = false;
    CLOVER_Shield_Presence[1] = true;
    CLOVER_Distance[1] = 21.*cm;
    CLOVER_phi[1] = 45*deg;
    CLOVER_theta[1] = 90*deg;
    CLOVER_rotm[1].rotateX(45.0*deg);
    CLOVER_rotm[1].rotateY(-90.0*deg);
    
    //  CLOVER 3
    CLOVER_Presence[2] = false;
    CLOVER_Shield_Presence[2] = false;
    CLOVER_Distance[2] = 21.*cm;
    CLOVER_phi[2] = 135*deg;
    CLOVER_theta[2] = 90*deg;
    CLOVER_rotm[2].rotateX(45.0*deg);
    CLOVER_rotm[2].rotateY(90.0*deg);

    //  CLOVER 4
    CLOVER_Presence[3] = false;
    CLOVER_Shield_Presence[3] = false;
    CLOVER_Distance[3] = 21.*cm;
    CLOVER_phi[3] = 180*deg;
    CLOVER_theta[3] = 90*deg;
    CLOVER_rotm[3].rotateY(90.0*deg);
    
    //  CLOVER 5
    CLOVER_Presence[4] = false;
    CLOVER_Shield_Presence[4] = false;
    CLOVER_Distance[4] = 21.*cm;
    CLOVER_phi[4] = 225*deg;
    CLOVER_theta[4] = 90*deg;
    CLOVER_rotm[4].rotateX(-45.0*deg);
    CLOVER_rotm[4].rotateY(90.0 *deg);
    
    //  CLOVER 6
    CLOVER_Presence[5] = false;
    CLOVER_Shield_Presence[5] = false;
    CLOVER_Distance[5] = 21.*cm;
    CLOVER_phi[5] = 0*deg;
    CLOVER_theta[5] = 45*deg;
    CLOVER_rotm[5].rotateY(-135.0*deg);
    
    //  CLOVER 7
    CLOVER_Presence[6] = false;
    CLOVER_Shield_Presence[6] = false;
    CLOVER_Distance[6] = 21.*cm;
    CLOVER_phi[6] = 90*deg;
    CLOVER_theta[6] = 45*deg;
    CLOVER_rotm[6].rotateX(135.0*deg);

    //  CLOVER 8
    CLOVER_Presence[7] = false;
    CLOVER_Shield_Presence[7] = false;
    CLOVER_Distance[7] = 21.*cm;
    CLOVER_phi[7] = -90*deg;
    CLOVER_theta[7] = 45*deg;
    CLOVER_rotm[7].rotateX(225*deg);
    
    
    for (G4int i=0; i<numberOf_CLOVER; i++)
    {
        if(CLOVER_AllPresent_Override) CLOVER_Presence[i] = true;
        if(CLOVER_AllAbsent_Override) CLOVER_Presence[i] = false;
        if(CLOVER_AllPresent_Override && CLOVER_AllAbsent_Override) CLOVER_Presence[i] = false;
        
        if(CLOVER_Shield_AllPresent_Override) CLOVER_Shield_Presence[i] = true;
        if(CLOVER_Shield_AllAbsent_Override) CLOVER_Shield_Presence[i] = false;
        if(CLOVER_Shield_AllPresent_Override && CLOVER_Shield_AllAbsent_Override) CLOVER_Shield_Presence[i] = false;
    }
    
   

    ////////////////////////////
    ////    OCL LaBr3 Detectors
    
    OCLLaBr3_AllPresent_Override = false;
    OCLLaBr3_AllAbsent_Override = false;
    
    
    // LaBr3 Detector 1
    OCLLaBr3_Presence[0] = true;
    OCLLaBr3_Distance[0] = 10.*cm+14.*cm;
    OCLLaBr3_phi[0] = -45.*deg;
    OCLLaBr3_theta[0] = 90.*deg;
    //OCLLaBr3_rotm[0].rotateX(270*deg);
    //OCLLaBr3_rotm[0].rotateZ(OCLLaBr3_theta[0]);
    //OCLLaBr3_rotm[0].rotateY(OCLLaBr3_phi[0]);
    
    //  LaBr3 Detector 2
    OCLLaBr3_Presence[1] = true;
    OCLLaBr3_Distance[1] = 10.*cm+14.*cm;
    OCLLaBr3_phi[1] = -(90.)*deg;
    OCLLaBr3_theta[1] = 45.*deg;

    
    for (G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
        OCLLaBr3_rotm[i].rotateX(270*deg);
        OCLLaBr3_rotm[i].rotateZ(OCLLaBr3_theta[i]);
        OCLLaBr3_rotm[i].rotateY(OCLLaBr3_phi[i]);

        if(OCLLaBr3_AllPresent_Override) OCLLaBr3_Presence[i] = true;
        if(OCLLaBr3_AllAbsent_Override) OCLLaBr3_Presence[i] = false;
        if(OCLLaBr3_AllPresent_Override && OCLLaBr3_AllAbsent_Override) OCLLaBr3_Presence[i] = false;
    }

    ////////////////////////////
    ////    FTA LaBr3 Detectors

    FTALaBr3_AllPresent_Override = false;
    FTALaBr3_AllAbsent_Override = false;


    // LaBr3 Detector 1
    FTALaBr3_Presence[0] = true;
    FTALaBr3_Distance[0] = 14.*cm;
    FTALaBr3_phi[0] = -135*deg;
    FTALaBr3_theta[0] = 54.5*deg;


    // LaBr3 Detector 2
    FTALaBr3_Presence[1] = true;
    FTALaBr3_Distance[1] = 14.*cm;
    FTALaBr3_phi[1] = -45*deg;
    FTALaBr3_theta[1] = 54.5*deg;

    // LaBr3 Detector 3
    FTALaBr3_Presence[2] = true;
    FTALaBr3_Distance[2] = 14.*cm;
    FTALaBr3_phi[2] = 45.*deg;
    FTALaBr3_theta[2] = (180.-56.)*deg;

    // LaBr3 Detector 4
    FTALaBr3_Presence[3] = true;
    FTALaBr3_Distance[3] = 14.*cm;
    FTALaBr3_phi[3] = 45.*deg;
    FTALaBr3_theta[3] = 54.5*deg;


    // LaBr3 Detector 5
    FTALaBr3_Presence[4] = true;
    FTALaBr3_Distance[4] = 14.*cm;
    FTALaBr3_phi[4] = 135.*deg;
    FTALaBr3_theta[4] = 54.5*deg;

    // LaBr3 Detector 6
    FTALaBr3_Presence[5] = true;
    FTALaBr3_Distance[5] = 14.*cm;
    FTALaBr3_phi[5] = 135.*deg;
    FTALaBr3_theta[5] = (180.-56.)*deg;


    for (G4int i=0; i<numberOf_FTALaBr3; i++)
    {

        FTALaBr3_rotm[i].rotateX(270*deg);
        FTALaBr3_rotm[i].rotateZ(FTALaBr3_theta[i]);
        FTALaBr3_rotm[i].rotateY(FTALaBr3_phi[i]);

        if(FTALaBr3_AllPresent_Override) FTALaBr3_Presence[i] = true;
        if(FTALaBr3_AllAbsent_Override) FTALaBr3_Presence[i] = false;
        if(FTALaBr3_AllPresent_Override && FTALaBr3_AllAbsent_Override) FTALaBr3_Presence[i] = false;
    }


    
    
    ////////////////////////////////////////////////////
    ////                                            ////
    ////            AFRODITE VAULT SETUP            ////
    ////                                            ////
    ////////////////////////////////////////////////////

    
    ////////////////////////////////////////////////////////
    ////                                                ////
    ////        AFRODITE VAULT - STRUCTURE SETUP        ////
    ////                                                ////
    ////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    ////    New AFRODITE Target Chamber by Mathis
    AFRODITE_MathisTC_Presence = true;
    
    /////////////////////////////////////
    ////    AFRODITE Target
    AFRODITE_Target_Presence = false;
    
    
    // Define materials
    DefineMaterials();
    
    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();
    
    //  NIST Material Database - Materials
    nistManager->FindOrBuildMaterial("G4_Galactic");
    nistManager->FindOrBuildMaterial("G4_AIR");
    nistManager->FindOrBuildMaterial("G4_BGO");
    nistManager->FindOrBuildMaterial("G4_Ge");
    nistManager->FindOrBuildMaterial("G4_Al");
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    nistManager->FindOrBuildMaterial("G4_MYLAR");
    nistManager->FindOrBuildMaterial("G4_W");
    nistManager->FindOrBuildMaterial("G4_Ar");
    nistManager->FindOrBuildMaterial("G4_Be");
    nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    nistManager->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    nistManager->FindOrBuildMaterial("G4_LITHIUM_CARBONATE");
    
    //  NIST Elementary Material Database - ELEMENTS
    nistManager->FindOrBuildElement("H");
    nistManager->FindOrBuildElement("C");
    nistManager->FindOrBuildElement("N");
    nistManager->FindOrBuildElement("O");
    nistManager->FindOrBuildElement("Fe");
    nistManager->FindOrBuildElement("Co");
    nistManager->FindOrBuildElement("Ni");
    nistManager->FindOrBuildElement("Cu");
    nistManager->FindOrBuildElement("Pb");
    nistManager->FindOrBuildElement("W");
    nistManager->FindOrBuildElement("Li");
    
    
    // Vacuum
    //new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
    
    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

    
    //////////////////////////////////////
    //          Get Materials           //
    //////////////////////////////////////
    
    ////    NIST Defined Elemental Material
    G4Material* G4_Al_Material  = G4Material::GetMaterial("G4_Al");

    ////    NIST Defined Materials and Compounds
    G4Material* G4_Galactic_Material = G4Material::GetMaterial("G4_Galactic");
    G4Material* G4_LITHIUM_CARBONATE_Material = G4Material::GetMaterial("G4_LITHIUM_CARBONATE");
    
    
    
    //////////////////////////////////////////////////////////
    //                      WORLD                           //
    //////////////////////////////////////////////////////////
    
    G4Box* SolidWorld = new G4Box("World", WorldSize/2, WorldSize/2, WorldSize/2);
    
    G4LogicalVolume*
    LogicWorld = new G4LogicalVolume(SolidWorld,                //its solid
                                     G4_Galactic_Material,      //its material
                                     "World");                  //its name
    G4VPhysicalVolume*
    PhysiWorld = new G4PVPlacement(0,                        //no rotation
                                   G4ThreeVector(),          //at (0,0,0)
                                   LogicWorld,               //its logical volume
                                   "World",                  //its name
                                   0,                        //its mother  volume
                                   false,                    //no boolean operation
                                   0);                       //copy number
    
    
    
    //////////////////////////////////////////////////////////
    //                      VACUUM CHAMBER
    //////////////////////////////////////////////////////////
    
    G4ThreeVector positionVacuumChamber = G4ThreeVector(0,0,0);
    
    G4Box* SolidVacuumChamber = new G4Box("VacuumChamber", (200./2)*cm, (200./2)*cm, (200./2)*cm);
    
    G4LogicalVolume* LogicVacuumChamber = new G4LogicalVolume(SolidVacuumChamber, G4_Galactic_Material,"VacuumChamber");
    
    PhysiVacuumChamber =
        new G4PVPlacement(0,               // no rotation
                          positionVacuumChamber, // at (x,y,z)
                          LogicVacuumChamber,       // its logical volume
                          "VacuumChamber",       // its name
                          LogicWorld,         // its mother  volume
                          false,           // no boolean operations
                          0,               // copy number
                          fCheckOverlaps); // checking overlaps
    
    
    
    
    
    //////////////////////////////////////////////////////////
    //              Scattering Chamber - CADMesh
    //////////////////////////////////////////////////////////
    
    if(AFRODITE_MathisTC_Presence)
    {
        G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
        G4cout << "Reading TC from " << PLY_PATH"/STRUCTURES/MathisTC/MathisTC.ply" << G4endl;
        CADMesh * mesh_MathisTC = new CADMesh(PLY_PATH"/STRUCTURES/MathisTC/MathisTC.ply", "PLY", mm, offset_MathisTC, false);
        
        G4VSolid * SolidMathisTC = mesh_MathisTC->TessellatedMesh();
        
        G4LogicalVolume* LogicMathisTC = new G4LogicalVolume(SolidMathisTC, G4_Al_Material, "BACTAR", 0, 0, 0);
        
        new G4PVPlacement(0,               // no rotation
                          G4ThreeVector(), // at (x,y,z)
                          LogicMathisTC,       // its logical volume
                          "BACTAR",       // its name
                          LogicVacuumChamber,         // its mother  volume
                          false,           // no boolean operations
                          0,               // copy number
                          fCheckOverlaps); // checking overlaps
        
        
        //  Visualisation
        G4VisAttributes* AFRODITE_MathisTC_VisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
        AFRODITE_MathisTC_VisAtt->SetForceSolid(true);
        LogicMathisTC->SetVisAttributes(AFRODITE_MathisTC_VisAtt);
    }
    
    
    //////////////////////////////////////////////////////////
    //                      TARGET
    //////////////////////////////////////////////////////////
    
    G4ThreeVector positionTarget = G4ThreeVector(0,0,0);
    
    G4Box* SolidTarget = new G4Box("Target", (20./2)*mm, (20./2)*mm, (2.42/2)*um);
    
    G4LogicalVolume* LogicTarget = new G4LogicalVolume(SolidTarget, G4_LITHIUM_CARBONATE_Material,"Target",0,0,0);
    
    if(AFRODITE_Target_Presence)
    {
        new G4PVPlacement(0,               // no rotation
                          positionTarget, // at (x,y,z)
                          LogicTarget,       // its logical volume
                          "Target",       // its name
                          LogicVacuumChamber,         // its mother  volume
                          false,           // no boolean operations
                          0,               // copy number
                          fCheckOverlaps); // checking overlaps
    }
    
    
    ////////////////////////////////////////////////////
    //               CLOVER INITIALIZATION            //
    ////////////////////////////////////////////////////


    bool have_shield = std::any_of(CLOVER_Shield_Presence, CLOVER_Shield_Presence+sizeof(CLOVER_Shield_Presence),
                                   [](const G4bool &p){ return p; });
    CloverFactory cloverFactory(have_shield);
    for(G4int i=0; i< numberOf_CLOVER ; i++)
    {
        CLOVER_position[i] = (CLOVER_Distance[i])*G4ThreeVector( sin(CLOVER_theta[i]) * cos(CLOVER_phi[i]), sin(CLOVER_theta[i]) * sin(CLOVER_phi[i]), cos(CLOVER_theta[i]));
        CLOVER_transform[i] = G4Transform3D(CLOVER_rotm[i],CLOVER_position[i]);
        cloverFactory.Construct(LogicVacuumChamber, CLOVER_position[i], CLOVER_rotm[i], i, fCheckOverlaps, CLOVER_Presence[i], CLOVER_Shield_Presence[i]);
    }

    ////////////////////////////////////////////////////
    //            OCL LaBr3 INITIALIZATION            //
    ////////////////////////////////////////////////////
    
    for(G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
        OCLLaBr3_position[i] = OCLLaBr3_Distance[i]
                                *G4ThreeVector( -sin(OCLLaBr3_theta[i]) * cos(OCLLaBr3_phi[i]), 
                                                cos(OCLLaBr3_theta[i]),
                                                sin(OCLLaBr3_theta[i]) * sin(OCLLaBr3_phi[i]));

        /////////////////////////////
        //          LaBr3 Detectors
        if(OCLLaBr3_Presence[i])
        {
           labr3[i] = new OCLLaBr3();
           labr3[i]->SetRotation(OCLLaBr3_rotm[i]);
           labr3[i]->SetPosition(OCLLaBr3_position[i]);
           labr3[i]->Placement(i,  PhysiVacuumChamber, fCheckOverlaps);
        }

    }


    ////////////////////////////////////////////////////
    //            FTA LaBr3 INITIALIZATION            //
    ////////////////////////////////////////////////////

    for(G4int i=0; i<numberOf_FTALaBr3; i++)
    {
        FTALaBr3_position[i] = (FTALaBr3_Distance[i])
                               *G4ThreeVector( -sin(FTALaBr3_theta[i]) * cos(FTALaBr3_phi[i]),
                                               cos(FTALaBr3_theta[i]),
                                               sin(FTALaBr3_theta[i]) * sin(FTALaBr3_phi[i])
        );

        /////////////////////////////
        //          LaBr3 Detectors
        if(FTALaBr3_Presence[i])
        {
            ftalabr3[i] = new FTALaBr3();
            ftalabr3[i]->SetRotation(FTALaBr3_rotm[i]);
            ftalabr3[i]->SetPosition(FTALaBr3_position[i]);
            ftalabr3[i]->Placement(i,  PhysiVacuumChamber, fCheckOverlaps);
        }

    }
    
    
    //////////////////////////////////////////////////////////
    //                      VISUALISATION
    //////////////////////////////////////////////////////////
    
    G4VisAttributes* World_VisAtt= new G4VisAttributes(G4Colour(0., 0., 0.));
    World_VisAtt->SetVisibility(true);
    LogicWorld->SetVisAttributes(World_VisAtt);
    
    //G4VisAttributes* Target_VisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    //LogicTarget->SetVisAttributes(Target_VisAtt);
    
    
    ////////////////////////////
    //  Vacuum Chamber
    
    G4VisAttributes* VacuumChamber_VisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    //VacuumChamber_VisAtt->SetVisibility(false);
    LogicVacuumChamber->SetVisAttributes(VacuumChamber_VisAtt);
    
    
    //
    //always return the physical World
    //
    return PhysiWorld;
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
