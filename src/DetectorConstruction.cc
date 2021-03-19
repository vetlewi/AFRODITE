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

#include "CADMesh/CADMesh.hh"
#include "DetectorConstruction.hh"


#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Trd.hh"

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4Isotope.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4PhysicalConstants.hh"

#include "G4UniformMagField.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"

#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"


#include "MagneticFieldMapping.hh"
//#include "G4BlineTracer.hh"

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
    , fAbsorberPV(nullptr)
    , fGapPV(nullptr)
    , fCheckOverlaps(false)
    , WorldSize( 15. * m )
    , PhysiCLOVER_HPGeCrystal(nullptr)
    , PhysiCLOVER_Shield_BGOCrystal(nullptr)
    , PhysiCLOVER_Shield_PMT(nullptr)
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
    CLOVER_Shield_AllAbsent_Override = true;
    
    
    //  CLOVER 1
    CLOVER_Presence[0] = true;
    CLOVER_Shield_Presence[0] = true;
    CLOVER_Distance[0] = 21.*cm;
    CLOVER_phi[0] = 0*deg;
    CLOVER_theta[0] = 90*deg;
    CLOVER_rotm[0].rotateY(-90.*deg);
    
    //  CLOVER 2
    CLOVER_Presence[1] = true;
    CLOVER_Shield_Presence[1] = true;
    CLOVER_Distance[1] = 21.*cm;
    CLOVER_phi[1] = 45*deg;
    CLOVER_theta[1] = 90*deg;
    CLOVER_rotm[1].rotateX(45.0*deg);
    CLOVER_rotm[1].rotateY(-90.0*deg);
    
    //  CLOVER 3
    CLOVER_Presence[2] = true;
    CLOVER_Shield_Presence[2] = true;
    CLOVER_Distance[2] = 21.*cm;
    CLOVER_phi[2] = 135*deg;
    CLOVER_theta[2] = 90*deg;
    CLOVER_rotm[2].rotateX(45.0*deg);
    CLOVER_rotm[2].rotateY(90.0*deg);

    //  CLOVER 4
    CLOVER_Presence[3] = true;
    CLOVER_Shield_Presence[3] = true;
    CLOVER_Distance[3] = 21.*cm;
    CLOVER_phi[3] = 180*deg;
    CLOVER_theta[3] = 90*deg;
    CLOVER_rotm[3].rotateY(90.0*deg);
    
    //  CLOVER 5
    CLOVER_Presence[4] = true;
    CLOVER_Shield_Presence[4] = true;
    CLOVER_Distance[4] = 21.*cm;
    CLOVER_phi[4] = 225*deg;
    CLOVER_theta[4] = 90*deg;
    CLOVER_rotm[4].rotateX(-45.0*deg);
    CLOVER_rotm[4].rotateY(90.0 *deg);
    
    //  CLOVER 6
    CLOVER_Presence[5] = true;
    CLOVER_Shield_Presence[5] = true;
    CLOVER_Distance[5] = 21.*cm;
    CLOVER_phi[5] = 0*deg;
    CLOVER_theta[5] = 45*deg;
    CLOVER_rotm[5].rotateY(-135.0*deg);
    
    //  CLOVER 7
    CLOVER_Presence[6] = true;
    CLOVER_Shield_Presence[6] = true;
    CLOVER_Distance[6] = 21.*cm;
    CLOVER_phi[6] = 90*deg;
    CLOVER_theta[6] = 45*deg;
    CLOVER_rotm[6].rotateX(135.0*deg);

    //  CLOVER 8
    CLOVER_Presence[7] = true;
    CLOVER_Shield_Presence[7] = true;
    CLOVER_Distance[7] = 21.*cm;
    CLOVER_phi[7] = -90*deg;
    CLOVER_theta[7] = 45*deg;
    CLOVER_rotm[7].rotateX(225*deg);

    
    //  CLOVER 9
    // CLOVER_Presence[8] = true;
    // CLOVER_Shield_Presence[8] = false;
    // CLOVER_Distance[8] = 21.*cm;
    // CLOVER_phi[8] = -45*deg;
    // CLOVER_theta[8] = 90*deg;
    // CLOVER_rotm[8].rotateY(-90.0 *deg);
    // CLOVER_rotm[8].rotateZ(-45.0 *deg);
    
    
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
    OCLLaBr3_rotm[0].rotateX(270*deg);
    OCLLaBr3_rotm[0].rotateZ(OCLLaBr3_theta[0]);
    OCLLaBr3_rotm[0].rotateY(OCLLaBr3_phi[0]);
    
    //  LaBr3 Detector 2
    OCLLaBr3_Presence[1] = true;
    OCLLaBr3_Distance[1] = 10.*cm+14.*cm;
    OCLLaBr3_phi[1] = -(90.)*deg;
    OCLLaBr3_theta[1] = 45.*deg;
    OCLLaBr3_rotm[1].rotateX(270*deg);
    OCLLaBr3_rotm[1].rotateZ(OCLLaBr3_theta[1]);
    OCLLaBr3_rotm[1].rotateY(OCLLaBr3_phi[1]);

    
    for (G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
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
    //          Get Elements            //
    //////////////////////////////////////
    
    G4Element* Hydrogen = G4Element::GetElement("H");
    G4Element* Carbon = G4Element::GetElement("C");
    G4Element* Nitrogen = G4Element::GetElement("N");
    G4Element* Oxygen = G4Element::GetElement("O");
    G4Element* Iron = G4Element::GetElement("Fe");
    G4Element* Cobalt = G4Element::GetElement("Co");
    G4Element* Nickel = G4Element::GetElement("Ni");
    G4Element* Copper = G4Element::GetElement("Cu");
    G4Element* Lead = G4Element::GetElement("Pb");
    G4Element* Tungsten = G4Element::GetElement("W");
    G4Element* Lithium = G4Element::GetElement("Li");
    
    
    //////////////////////////////////////
    //          Get Materials           //
    //////////////////////////////////////
    
    ////    NIST Defined Elemental Material
    G4Material* G4_Ge_Material = G4Material::GetMaterial("G4_Ge");
    G4Material* G4_Al_Material  = G4Material::GetMaterial("G4_Al");
    G4Material* G4_Si_Material = G4Material::GetMaterial("G4_Si");
    G4Material* G4_W_Material = G4Material::GetMaterial("G4_W");
    G4Material* G4_Ar_Material = G4Material::GetMaterial("G4_Ar");
    G4Material* G4_Be_Material = G4Material::GetMaterial("G4_Be");
    
    ////    NIST Defined Materials and Compounds
    G4Material* G4_Galactic_Material = G4Material::GetMaterial("G4_Galactic");
    G4Material* G4_AIR_Material = G4Material::GetMaterial("G4_AIR");
    G4Material* G4_BGO_Material = G4Material::GetMaterial("G4_BGO");
    G4Material* G4_PLASTIC_SC_VINYLTOLUENE_Material = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Material* G4_Mylar_Material = G4Material::GetMaterial("G4_MYLAR");
    G4Material* G4_CARBON_DIOXIDE_Material = G4Material::GetMaterial("G4_CARBON_DIOXIDE");
    G4Material* G4_SODIUM_IODIDE_Material = G4Material::GetMaterial("G4_SODIUM_IODIDE");
    G4Material* G4_LITHIUM_CARBONATE_Material = G4Material::GetMaterial("G4_LITHIUM_CARBONATE");
    
    ////    CLOVER Detector Shield, HEAVIMET Material
    G4Material* Heavimet_Material = new G4Material("Heavimet_Material",19.25*g/cm3, 5);
    Heavimet_Material->AddElement( Tungsten, 94.20*perCent);
    Heavimet_Material->AddElement( Nickel, 4.35*perCent);
    Heavimet_Material->AddElement( Iron, 0.85*perCent);
    Heavimet_Material->AddElement( Cobalt, 0.50*perCent);
    Heavimet_Material->AddElement( Copper, 0.10*perCent);
    
    
    ////    PlasticScint Detector Materials
    ////    This Plastic is identical in composition to NE102, which is the plastic utilized in the Neutron Wall plastic Scintillators which have been employed within the AFRODITE vault for numerous (3He,n) experiments
    G4Material* BC408_Material = new G4Material("BC408_Material",1.032*g/cm3, 2);
    BC408_Material->AddElement( Hydrogen, 8.4748*perCent);
    BC408_Material->AddElement( Carbon, 91.5252*perCent);
    
    ////    VDC Detector Materials
    G4Material* VDC_SR_Gas_Material = new G4Material("VDC_SR_Gas_Material", 0.00184212*g/cm3, 2);
    VDC_SR_Gas_Material->AddMaterial( G4_Ar_Material, 90.*perCent);
    VDC_SR_Gas_Material->AddMaterial( G4_CARBON_DIOXIDE_Material, 10.*perCent);
    
    
    
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
    
    G4Box* SolidVacuumChamber = new G4Box("VacuumChamber", (100./2)*cm, (100./2)*cm, (100./2)*cm);
    
    G4LogicalVolume* LogicVacuumChamber = new G4LogicalVolume(SolidVacuumChamber, G4_Galactic_Material,"VacuumChamber",0,0,0);
    
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
    
    
    
    
    /////////////////////////////////////////////
    //             CLOVER DEFINITION           //
    /////////////////////////////////////////////
    
    G4double CLOVERtoShield_displacement = 0.;  // cm
    
    G4ThreeVector offset_CLOVERInternalVacuum = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    G4ThreeVector offset_CLOVEREncasement = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    G4ThreeVector offset_CLOVERHPGeCrystal1 = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    G4ThreeVector offset_CLOVERHPGeCrystal2 = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    G4ThreeVector offset_CLOVERHPGeCrystal3 = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    G4ThreeVector offset_CLOVERHPGeCrystal4 = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
    
    G4LogicalVolume * Logic_CLOVER_InternalVacuum[numberOf_CLOVER];
    G4LogicalVolume * Logic_CLOVER_Encasement;
    G4LogicalVolume * Logic_CLOVER_HPGeCrystal[4];

    if( CLOVER_Presence[0] || CLOVER_Presence[1] || CLOVER_Presence[2] || CLOVER_Presence[3] || CLOVER_Presence[4] || CLOVER_Presence[5] || CLOVER_Presence[6] || CLOVER_Presence[7] )
    {
        //////////////////////////////////////////////////////////
        //              CLOVER Internal Vacuum - CADMesh
        G4cout << "Reading CLOVER internal vacuum from " << PLY_PATH"/DETECTORS/CLOVER/CLOVER-InternalVacuum/CloverInternalVacuum.ply" << G4endl;
        CADMesh * mesh_CLOVERInternalVacuum = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/CLOVER-InternalVacuum/CloverInternalVacuum.ply", "PLY", mm, offset_CLOVERInternalVacuum, false);
        
        G4VSolid * Solid_CLOVERInternalVacuum = mesh_CLOVERInternalVacuum->TessellatedMesh();
        
        for(G4int i=0; i<numberOf_CLOVER; i++)
        {
            Logic_CLOVER_InternalVacuum[i] = new G4LogicalVolume(Solid_CLOVERInternalVacuum, G4_Galactic_Material, "LogicCLOVERInternalVacuum", 0, 0, 0);
        }
        
        
        ///////////////////////////////////////////////////////
        //              CLOVER Encasement - CADMesh
        G4cout << "Reading CLOVER encasement from " << PLY_PATH"/DETECTORS/CLOVER/CloverEncasement/CloverEncasement.ply" << G4endl;
        CADMesh * mesh_CLOVEREncasement = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/CloverEncasement/CloverEncasement.ply", "PLY", mm, offset_CLOVEREncasement, false);
        
        G4VSolid * Solid_CLOVEREncasement = mesh_CLOVEREncasement->TessellatedMesh();
        
        Logic_CLOVER_Encasement = new G4LogicalVolume(Solid_CLOVEREncasement, G4_Al_Material, "LogicCLOVERCloverEncasement", 0, 0, 0);
        
        
        //////////////////////////////////////////////////////////
        //              CLOVER HPGeCrystals - CADMesh

        G4cout << "Reading CLOVER crystals from " << PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGeCrystal[1-4].ply" << G4endl;
        CADMesh * mesh_CLOVERHPGeCrystal1 = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGeCrystal1.ply", "PLY", mm, offset_CLOVERHPGeCrystal1, false);
        CADMesh * mesh_CLOVERHPGeCrystal2 = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGeCrystal2.ply", "PLY", mm, offset_CLOVERHPGeCrystal2, false);
        CADMesh * mesh_CLOVERHPGeCrystal3 = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGeCrystal3.ply", "PLY", mm, offset_CLOVERHPGeCrystal3, false);
        CADMesh * mesh_CLOVERHPGeCrystal4 = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGeCrystal4.ply", "PLY", mm, offset_CLOVERHPGeCrystal4, false);
        
        G4VSolid * Solid_HPGeCrystal1 = mesh_CLOVERHPGeCrystal1->TessellatedMesh();
        G4VSolid * Solid_HPGeCrystal2 = mesh_CLOVERHPGeCrystal2->TessellatedMesh();
        G4VSolid * Solid_HPGeCrystal3 = mesh_CLOVERHPGeCrystal3->TessellatedMesh();
        G4VSolid * Solid_HPGeCrystal4 = mesh_CLOVERHPGeCrystal4->TessellatedMesh();
        
        Logic_CLOVER_HPGeCrystal[0] = new G4LogicalVolume(Solid_HPGeCrystal1, G4_Ge_Material,"LogicCLOVERHPGeCrystal",0,0,0);
        Logic_CLOVER_HPGeCrystal[1] = new G4LogicalVolume(Solid_HPGeCrystal2, G4_Ge_Material,"LogicCLOVERHPGeCrystal",0,0,0);
        Logic_CLOVER_HPGeCrystal[2] = new G4LogicalVolume(Solid_HPGeCrystal3, G4_Ge_Material,"LogicCLOVERHPGeCrystal",0,0,0);
        Logic_CLOVER_HPGeCrystal[3] = new G4LogicalVolume(Solid_HPGeCrystal4, G4_Ge_Material,"LogicCLOVERHPGeCrystal",0,0,0);
        
    }
    
    
    ////////////////////////////////////////////
    //              CLOVER SHIELD
    ////////////////////////////////////////////
    
    G4ThreeVector offset_CLOVER_Shield_Body = G4ThreeVector(0*cm, 0*cm, 0*cm);
    G4ThreeVector offset_CLOVER_Shield_Heavimet = G4ThreeVector(0*cm, 0*cm, 0*cm);
    G4ThreeVector offset_CLOVER_Shield_BGOCrystals = G4ThreeVector(0*cm, 0*cm, 0*cm);
    G4ThreeVector offset_CLOVER_Shield_PMT = G4ThreeVector(0*cm, 0*cm, 0*cm);
    G4ThreeVector offset_CLOVER_Shield_PMTConArray = G4ThreeVector(0*cm, 0*cm, 0*cm);
    
    G4LogicalVolume* Logic_CLOVER_Shield_Body;
    G4LogicalVolume* Logic_CLOVER_Shield_Heavimet;
    G4LogicalVolume* Logic_CLOVER_Shield_BGOCrystal[16];
    G4LogicalVolume* Logic_CLOVER_Shield_PMT[16];

    if( CLOVER_Shield_Presence[0] || CLOVER_Shield_Presence[1] || CLOVER_Shield_Presence[2] || CLOVER_Shield_Presence[3] || CLOVER_Shield_Presence[4] || CLOVER_Shield_Presence[5] || CLOVER_Shield_Presence[6] || CLOVER_Shield_Presence[7] )
    {
        
        ///////////////////////////////////////////////////////
        //              CLOVER Shield Body - CADMesh
        ///////////////////////////////////////////////////////

        G4cout << "Reading CLOVER shield body from " << PLY_PATH"/DETECTORS/CLOVER/Shield/Body/Body.ply" << G4endl;
        G4cout << "This WILL take approx. 6 minutes, depending on CPU speed" << G4endl;
        CADMesh * mesh_CLOVER_Shield_Body = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/Shield/Body/Body.ply", "PLY", mm, offset_CLOVER_Shield_Body, false);
        
        G4VSolid * Solid_CLOVER_Shield_Body = mesh_CLOVER_Shield_Body->TessellatedMesh();
        
        Logic_CLOVER_Shield_Body = new G4LogicalVolume(Solid_CLOVER_Shield_Body, G4_Al_Material, "LogicCLOVERShieldBody", 0, 0, 0);
        
        
        ///////////////////////////////////////////////////////
        //              CLOVER Shield Heavimet - CADMesh
        ///////////////////////////////////////////////////////

        G4cout << "Reading CLOVER shield body from " << PLY_PATH"/DETECTORS/CLOVER/Shield/Heavimet/Heavimet.ply" << G4endl;
        CADMesh * mesh_CLOVER_Shield_Heavimet = new CADMesh(PLY_PATH"/DETECTORS/CLOVER/Shield/Heavimet/Heavimet.ply", "PLY", mm, offset_CLOVER_Shield_Heavimet, false);
        
        G4VSolid * Solid_CLOVER_Shield_Heavimet = mesh_CLOVER_Shield_Heavimet->TessellatedMesh();
        
        Logic_CLOVER_Shield_Heavimet = new G4LogicalVolume(Solid_CLOVER_Shield_Heavimet, Heavimet_Material, "LogicCLOVERShieldHeavimet", 0, 0, 0);
        
        
        
        ///////////////////////////////////////////////////////
        //      CLOVER Shield BGO Crystals - CADMesh
        ///////////////////////////////////////////////////////
        
        G4Box* Solid_BGOCrystal_template = new G4Box("BGOCrystal_template", (200./2)*mm, (200./2)*mm, (600./2)*mm);
        G4Box* Solid_BGOCrystal_razor = new G4Box("BGOCrystal_razor", (500./2)*mm, (500./2)*mm, (500./2)*mm);
        G4Tubs* Solid_BGOCrystal_PMTrazor = new G4Tubs("BGOCrystal_PMTrazor", 0.*mm, 500.*mm, 57.*mm, 0.*deg, 360.*deg);
        
        G4VSolid* Solid_BGOCrystal_shear[16][7];
        G4ThreeVector   position_BGOCrystal_razor[16][7];
        G4RotationMatrix* rm_BGOCrystal_razor[16][7];
        
        for(G4int i=0; i<16; i++)
        {
            for(G4int j=0; j<7; j++)
            {
                rm_BGOCrystal_razor[i][j] = new G4RotationMatrix();
            }
        }
        
        ////    The components of the positions used by the razor to shear each BGOCrystal shape
        ////    There are two "unique crystals" (up to rotations/reflections), therefore only two sets of positions are needed.
        ////    There are seven shears needed per BGO Crystal
        G4double    cP_BGOCrystal_razor[2][7][3];
        G4double    sign_BGOCrystal_razor[8][3];
        G4double    crm_BGOCrystal_razor[2][7];
        G4int       signrm_BGOCrystal_quad[4][3];
        
        ////    The components of positions for the razor
        ////    cP_BGOCrystal1_razor
        cP_BGOCrystal_razor[0][0][0] = 0., cP_BGOCrystal_razor[0][0][1] = 0., cP_BGOCrystal_razor[0][0][2] = (250.-42.);
        cP_BGOCrystal_razor[0][1][0] = -0.2000000, cP_BGOCrystal_razor[0][1][1] = -189.5684723, cP_BGOCrystal_razor[0][1][2] = -213.7959824;
        cP_BGOCrystal_razor[0][2][0] = -0.2000000, cP_BGOCrystal_razor[0][2][1] = 326.0592535, cP_BGOCrystal_razor[0][2][2] = -150.4848585;
        cP_BGOCrystal_razor[0][3][0] = -0.2000000, cP_BGOCrystal_razor[0][3][1] = 312.5280186, cP_BGOCrystal_razor[0][3][2] = -74.2400764;
        cP_BGOCrystal_razor[0][4][0] = (-0.20 + 250.), cP_BGOCrystal_razor[0][4][1] = 74.3269833, cP_BGOCrystal_razor[0][4][2] = -151.6673049;
        cP_BGOCrystal_razor[0][5][0] = -287.9700263, cP_BGOCrystal_razor[0][5][1] = 61.7954410, cP_BGOCrystal_razor[0][5][2] = -209.6135091;
        cP_BGOCrystal_razor[0][6][0] = 0., cP_BGOCrystal_razor[0][6][1] = 83.3486382, cP_BGOCrystal_razor[0][6][2] = -310.4623689;
        
        ////    cP_BGOCrystal2_razor
        cP_BGOCrystal_razor[1][0][0] = 0., cP_BGOCrystal_razor[1][0][1] = 0., cP_BGOCrystal_razor[1][0][2] = (250.-42.);
        cP_BGOCrystal_razor[1][1][0] = -0.2000000, cP_BGOCrystal_razor[1][1][1] = -189.5684723, cP_BGOCrystal_razor[1][1][2] = -213.7959824;
        cP_BGOCrystal_razor[1][2][0] = -0.2000000, cP_BGOCrystal_razor[1][2][1] = 326.0592535, cP_BGOCrystal_razor[1][2][2] = -150.4848585;
        cP_BGOCrystal_razor[1][3][0] = -0.2000000, cP_BGOCrystal_razor[1][3][1] = 312.5280186, cP_BGOCrystal_razor[1][3][2] = -74.2400764;
        cP_BGOCrystal_razor[1][4][0] = 208.0290511, cP_BGOCrystal_razor[1][4][1] = 69.0956904, cP_BGOCrystal_razor[1][4][2] = -269.0694506;
        cP_BGOCrystal_razor[1][5][0] = -252.0980447, cP_BGOCrystal_razor[1][5][1] = -101.2517474, cP_BGOCrystal_razor[1][5][2] = -172.2364983;
        cP_BGOCrystal_razor[1][6][0] = 0., cP_BGOCrystal_razor[1][6][1] = 83.3486382, cP_BGOCrystal_razor[1][6][2] = -310.4623689;
        
        ////    Reflections (for a total of 8 versions of the aforementioned two "unique" BGO Crystals
        sign_BGOCrystal_razor[0][0] = 1, sign_BGOCrystal_razor[0][1] = 1, sign_BGOCrystal_razor[0][2] = 1;
        sign_BGOCrystal_razor[1][0] = -1, sign_BGOCrystal_razor[1][1] = -1, sign_BGOCrystal_razor[1][2] = 1;
        sign_BGOCrystal_razor[2][0] = -1, sign_BGOCrystal_razor[2][1] = 1, sign_BGOCrystal_razor[2][2] = 1;
        sign_BGOCrystal_razor[3][0] = 1, sign_BGOCrystal_razor[3][1] = -1, sign_BGOCrystal_razor[3][2] = 1;
        sign_BGOCrystal_razor[4][0] = -1, sign_BGOCrystal_razor[4][1] = -1, sign_BGOCrystal_razor[4][2] = 1;
        sign_BGOCrystal_razor[5][0] = 1, sign_BGOCrystal_razor[5][1] = 1, sign_BGOCrystal_razor[5][2] = 1;
        sign_BGOCrystal_razor[6][0] = 1, sign_BGOCrystal_razor[6][1] = -1, sign_BGOCrystal_razor[6][2] = 1;
        sign_BGOCrystal_razor[7][0] = -1, sign_BGOCrystal_razor[7][1] = 1, sign_BGOCrystal_razor[7][2] = 1;
        
        
        ////    A sign per rotation (about 3 axes), per quadrant
        ////    0->X, 1->Y, 2->Z
        signrm_BGOCrystal_quad[0][0] = 1, signrm_BGOCrystal_quad[0][1] = 1, signrm_BGOCrystal_quad[0][2] = 1;
        signrm_BGOCrystal_quad[1][0] = -1, signrm_BGOCrystal_quad[1][1] = 1, signrm_BGOCrystal_quad[1][2] = 1;
        signrm_BGOCrystal_quad[2][0] = -1, signrm_BGOCrystal_quad[2][1] = -1, signrm_BGOCrystal_quad[2][2] = 1;
        signrm_BGOCrystal_quad[3][0] = 1, signrm_BGOCrystal_quad[3][1] = -1, signrm_BGOCrystal_quad[3][2] = 1;
        
        ////    The two "unique" sets of rotations;
        crm_BGOCrystal_razor[0][0] = 0.;
        crm_BGOCrystal_razor[0][1] = -7.;
        crm_BGOCrystal_razor[0][2] = -7.;
        crm_BGOCrystal_razor[0][3] = -14.5;
        crm_BGOCrystal_razor[0][4] = 0.;
        crm_BGOCrystal_razor[0][5] = -7.;
        crm_BGOCrystal_razor[0][6] = -3.;
        
        crm_BGOCrystal_razor[1][0] = 0.;
        crm_BGOCrystal_razor[1][1] = -7.;
        crm_BGOCrystal_razor[1][2] = -7.;
        crm_BGOCrystal_razor[1][3] = -14.5;
        crm_BGOCrystal_razor[1][4] = -7.;
        crm_BGOCrystal_razor[1][5] = -45.;
        crm_BGOCrystal_razor[1][6] = -3.;
        
        
        //  ph suffix-> place holder
        G4double    cP_BGOCrystal_ph[3];
        G4double    signP_BGOCrystal_ph[3];
        
        G4int       index1, index2;
        G4double    swap[2];
        
        for(G4int i=0; i<16; i++)
        {
            ////    Determining which copy number (0->7) of pair of "two unique" BGO Crystals we are working with
            if( ((i/2)%2)==0 )
            {
                ////    Depending upon the copy number, the order of the crystals shall change
                if((i%2)==0)
                    index1 = 0;
                else
                    index1 = 1;
            }
            else
            {
                ////    Depending upon the copy number, the order of the crystals shall change
                if((i%2)==0)
                    index1 = 1;
                else
                    index1 = 0;
            }
            
            ////    Positions of the shearing
            for(G4int j=0; j<7; j++)
            {
                cP_BGOCrystal_ph[0] = cP_BGOCrystal_razor[index1][j][0];
                cP_BGOCrystal_ph[1] = cP_BGOCrystal_razor[index1][j][1];
                cP_BGOCrystal_ph[2] = cP_BGOCrystal_razor[index1][j][2];
                
                ////    Reflection
                if((i>1 && i<6) || (i>9 && i<14))
                {
                    swap[0] = cP_BGOCrystal_ph[0];
                    swap[1] = cP_BGOCrystal_ph[1];
                    
                    cP_BGOCrystal_ph[0] = swap[1];
                    cP_BGOCrystal_ph[1] = swap[0];
                }
                
                signP_BGOCrystal_ph[0] = sign_BGOCrystal_razor[i/2][0];
                signP_BGOCrystal_ph[1] = sign_BGOCrystal_razor[i/2][1];
                signP_BGOCrystal_ph[2] = sign_BGOCrystal_razor[i/2][2];
                
                position_BGOCrystal_razor[i][j] = G4ThreeVector(signP_BGOCrystal_ph[0]*cP_BGOCrystal_ph[0]*mm,
                                                                signP_BGOCrystal_ph[1]*cP_BGOCrystal_ph[1]*mm,
                                                                signP_BGOCrystal_ph[2]*cP_BGOCrystal_ph[2]*mm);
                
            }
            
            ////    Rotations of the shearing
            ////    Determining which quadrant (eg. (X-, Y+)) of BGO Crystals we are working with
            ////    This determines between the two "unique" planes of rotation for the shearing
            ////    The "index1" is also neccesary to determine which of the two crystals we are working with
            
            
            ////    Determining the quadrant
            index2 = i - ((i/4)*4);
            
            if((i/4)==0 || (i/4)==2) index2 = index2;
            if((i/4)==1 || (i/4)==3) index2 = 3 - index2;
            
            if(index2==0)
            {
                rm_BGOCrystal_razor[i][0]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[0][0]*deg);
                rm_BGOCrystal_razor[i][1]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][1]*deg);
                rm_BGOCrystal_razor[i][2]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][2]*deg);
                rm_BGOCrystal_razor[i][3]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][3]*deg);
                rm_BGOCrystal_razor[i][4]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][4]*deg);
                rm_BGOCrystal_razor[i][5]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][5]*deg);
                rm_BGOCrystal_razor[i][6]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][6]*deg);
            }
            if(index2==1)
            {
                rm_BGOCrystal_razor[i][0]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[1][0]*deg);
                rm_BGOCrystal_razor[i][1]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[1][1]*deg);
                rm_BGOCrystal_razor[i][2]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[1][2]*deg);
                rm_BGOCrystal_razor[i][3]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[1][3]*deg);
                rm_BGOCrystal_razor[i][4]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[1][4]*deg);
                rm_BGOCrystal_razor[i][5]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[1][5]*deg);
                rm_BGOCrystal_razor[i][6]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[1][6]*deg);
            }
            if(index2==2)
            {
                rm_BGOCrystal_razor[i][0]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[1][0]*deg);
                rm_BGOCrystal_razor[i][1]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[1][1]*deg);
                rm_BGOCrystal_razor[i][2]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[1][2]*deg);
                rm_BGOCrystal_razor[i][3]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[1][3]*deg);
                rm_BGOCrystal_razor[i][4]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[1][4]*deg);
                rm_BGOCrystal_razor[i][5]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[1][5]*deg);
                rm_BGOCrystal_razor[i][6]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[1][6]*deg);
            }
            if(index2==3)
            {
                rm_BGOCrystal_razor[i][0]->rotateZ(signrm_BGOCrystal_quad[(i/4)][2]*crm_BGOCrystal_razor[0][0]*deg);
                rm_BGOCrystal_razor[i][1]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][1]*deg);
                rm_BGOCrystal_razor[i][2]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][2]*deg);
                rm_BGOCrystal_razor[i][3]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][3]*deg);
                rm_BGOCrystal_razor[i][4]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][4]*deg);
                rm_BGOCrystal_razor[i][5]->rotateX(signrm_BGOCrystal_quad[(i/4)][0]*crm_BGOCrystal_razor[0][5]*deg);
                rm_BGOCrystal_razor[i][6]->rotateY(signrm_BGOCrystal_quad[(i/4)][1]*crm_BGOCrystal_razor[0][6]*deg);
            }
            
            
        }
        
        
        char nameChar[512];
        G4String nameG4String;
        
        for(G4int i=0; i<16; i++)
        {
            for(G4int j=0; j<6; j++)
            {
                sprintf(nameChar,"Solid_BGOCrystal%d_shear%d", i, j);
                nameG4String = string(nameChar);
                
                if(j==0)
                {
                    Solid_BGOCrystal_shear[i][j] = new G4SubtractionSolid(nameG4String, Solid_BGOCrystal_template, Solid_BGOCrystal_razor, rm_BGOCrystal_razor[i][j], position_BGOCrystal_razor[i][j]);
                }
                else
                {
                    Solid_BGOCrystal_shear[i][j] = new G4SubtractionSolid(nameG4String, Solid_BGOCrystal_shear[i][j-1], Solid_BGOCrystal_razor, rm_BGOCrystal_razor[i][j], position_BGOCrystal_razor[i][j]);
                }
                
            }
            
            ////    The final shear for each crystal (corresponding to the surface that the PMT attaches) is executed in such a way that the resulting surface is well defined for simple PMT coupling
            ////    This scintillation still needs to be fully implemented
            
            sprintf(nameChar,"Solid_BGOCrystal%d", i);
            nameG4String = string(nameChar);
            
            Solid_BGOCrystal_shear[i][6] = new G4SubtractionSolid(nameChar, Solid_BGOCrystal_shear[i][5], Solid_BGOCrystal_PMTrazor, rm_BGOCrystal_razor[i][6], position_BGOCrystal_razor[i][6]);
            
        }
        
        for(G4int i=0; i<16; i++)
        {
            Logic_CLOVER_Shield_BGOCrystal[i] = new G4LogicalVolume(Solid_BGOCrystal_shear[i][6], G4_BGO_Material, "LogicCLOVERShieldBGOCrystal", 0, 0, 0);
        }
        
        
        
        ////////////////////////////////////
        //      CLOVER Shield PMT's
        ////////////////////////////////////
        
        G4VSolid* Solid_CLOVER_Shield_PMT[16];
        
        ////    Positioning of the centre of the PMT's
        ////    Once again, there are only two "unique" positions (the rest can be obtained through reflections about the cartesian axes)
        G4ThreeVector   position_Shield_PMT[16];
        G4double        cP_Shield_PMT[2][3];
        
        cP_Shield_PMT[0][0] = -22., cP_Shield_PMT[0][1] = 83.3486382, cP_Shield_PMT[0][2] = -310.4623689;
        cP_Shield_PMT[1][0] = -58.5, cP_Shield_PMT[1][1] = 83.3486382, cP_Shield_PMT[1][2] = -310.4623689;
        
        
        ////    A unique rotation of the PMT's within each quadrant (X+, X-, Y+, Y-)
        G4RotationMatrix* rm_Shield_PMT[4];
        
        for(G4int i=0; i<4; i++)
        {
            rm_Shield_PMT[i] = new G4RotationMatrix();
        }
        
        rm_Shield_PMT[0]->rotateX(-3.*deg);
        rm_Shield_PMT[1]->rotateY(-3.*deg);
        rm_Shield_PMT[2]->rotateX(3.*deg);
        rm_Shield_PMT[3]->rotateY(3.*deg);
        
        G4Tubs* Solid_Shield_PMTtemplate = new G4Tubs("Shield_PMT", 0.*mm, 14.3*mm, 57.*mm, 0.*deg, 360.*deg);
        G4Box* Solid_Shield_PMTRaw = new G4Box("Shield_PMTtemplate", (1000./2)*mm, (1000./2)*mm, (1000./2)*mm);
        
        
        //  placeholders
        G4double    cP_Shield_PMT_ph[3];
        G4double    signP_Shield_PMT_ph[3];
        
        for(G4int i=0; i<16; i++)
        {
            ////    Determining which copy number (0->7) of pair of "two unique" PMT positions we are working with
            if( ((i/2)%2)==0 )
            {
                ////    Depending upon the copy number, the order of the PMT's shall change
                if((i%2)==0)
                    index1 = 0;
                else
                    index1 = 1;
            }
            else
            {
                ////    Depending upon the copy number, the order of the PMT's shall change
                if((i%2)==0)
                    index1 = 1;
                else
                    index1 = 0;
            }
            
            ////    Positions of the Intersection
            cP_Shield_PMT_ph[0] = cP_Shield_PMT[index1][0];
            cP_Shield_PMT_ph[1] = cP_Shield_PMT[index1][1];
            cP_Shield_PMT_ph[2] = cP_Shield_PMT[index1][2];
            
            if((i>1 && i<6) || (i>9 && i<14))
            {
                swap[0] = cP_Shield_PMT_ph[0];
                swap[1] = cP_Shield_PMT_ph[1];
                
                cP_Shield_PMT_ph[0] = swap[1];
                cP_Shield_PMT_ph[1] = swap[0];
            }
            
            signP_Shield_PMT_ph[0] = sign_BGOCrystal_razor[i/2][0];
            signP_Shield_PMT_ph[1] = sign_BGOCrystal_razor[i/2][1];
            signP_Shield_PMT_ph[2] = sign_BGOCrystal_razor[i/2][2];
            
            position_Shield_PMT[i] = G4ThreeVector(signP_Shield_PMT_ph[0]*cP_Shield_PMT_ph[0]*mm,
                                                   signP_Shield_PMT_ph[1]*cP_Shield_PMT_ph[1]*mm,
                                                   signP_Shield_PMT_ph[2]*cP_Shield_PMT_ph[2]*mm);
            
            ////////////////////////////////////////////////////////////////////
            
            sprintf(nameChar,"Solid_CLOVER_Shield_PMT%d", i);
            nameG4String = string(nameChar);
            
            ////    Selecting which quadrant of PMT's (X+, X-, Y+, Y-) that we are working with
            if(i==0 || i==1 || i==14 || i==15)  index2 = 0;
            if(i>=2 && i<=5)  index2 = 1;
            if(i>=6 && i<=9)  index2 = 2;
            if(i>=10 && i<=13)  index2 = 3;
            
            Solid_CLOVER_Shield_PMT[i] = new G4IntersectionSolid(nameG4String, Solid_Shield_PMTRaw, Solid_Shield_PMTtemplate, rm_Shield_PMT[index2], position_Shield_PMT[i]);
            
            Logic_CLOVER_Shield_PMT[i] = new G4LogicalVolume(Solid_CLOVER_Shield_PMT[i], G4_AIR_Material,"LogicCLOVERShieldPMT",0,0,0);
            
        }
    }
    
    
    ////////////////////////////////////////////////////
    //               CLOVER INITIALIZATION            //
    ////////////////////////////////////////////////////
    
    
    for(G4int i=0; i<numberOf_CLOVER; i++)
    {
        CLOVER_position[i] = (CLOVER_Distance[i])*G4ThreeVector( sin(CLOVER_theta[i]) * cos(CLOVER_phi[i]), sin(CLOVER_theta[i]) * sin(CLOVER_phi[i]), cos(CLOVER_theta[i]));
        
        CLOVER_transform[i] = G4Transform3D(CLOVER_rotm[i],CLOVER_position[i]);
        
        CLOVER_Shield_position[i] = (CLOVER_Distance[i])*G4ThreeVector( sin(CLOVER_theta[i]) * cos(CLOVER_phi[i]), sin(CLOVER_theta[i]) * sin(CLOVER_phi[i]), cos(CLOVER_theta[i]));
        
        CLOVER_Shield_transform[i] = CLOVER_transform[i];
        
        /////////////////////////////
        //          CLOVER
        if(CLOVER_Presence[i])
        {
            new G4PVPlacement(CLOVER_transform[i],   // transformation matrix
                              Logic_CLOVER_Encasement,       // its logical volume
                              "CLOVER_Encasement",       // its name
                              LogicVacuumChamber,         // its mother  volume
                              false,           // no boolean operations
                              i,               // copy number
                              fCheckOverlaps); // checking overlaps
            
            
            
            
            
            for (int j=0; j<4; j++)
            {
                PhysiCLOVER_HPGeCrystal = new G4PVPlacement(0,               // no rotation
                                                            G4ThreeVector(0,0,0), // at (x,y,z)
                                                            Logic_CLOVER_HPGeCrystal[j],
                                                            "CLOVER_HPGeCrystal", // its name
                                                            Logic_CLOVER_InternalVacuum[i],
                                                            false,           // no boolean operations
                                                            i*4 + j,               // copy number
                                                            fCheckOverlaps); // checking overlaps
                
            }
            
            
            new G4PVPlacement(CLOVER_transform[i],
                              Logic_CLOVER_InternalVacuum[i],
                              "CLOVER_InternalVacuum",       // its name
                              LogicVacuumChamber,         // its mother  volume
                              false,           // no boolean operations
                              i,               // copy number
                              fCheckOverlaps); // checking overlaps
            
        }
        
        
        /////////////////////////////
        //      CLOVER SHIELD
        if(CLOVER_Shield_Presence[i])
        {
            
            new G4PVPlacement(CLOVER_Shield_transform[i],
                              Logic_CLOVER_Shield_Body,
                              "CLOVER_Shield_Body",       // its name
                              LogicVacuumChamber,         // its mother  volume
                              false,           // no boolean operations
                              i,               // copy number
                              fCheckOverlaps); // checking overlaps
            
            for (int j=0; j<16; j++)
            {
                PhysiCLOVER_Shield_BGOCrystal = new G4PVPlacement(CLOVER_Shield_transform[i],
                                                                  Logic_CLOVER_Shield_BGOCrystal[j],
                                                                  "CLOVER_Shield_BGOCrystal",
                                                                  LogicVacuumChamber,
                                                                  false,
                                                                  i*16 + j,
                                                                  fCheckOverlaps);
                
                PhysiCLOVER_Shield_PMT = new G4PVPlacement(CLOVER_Shield_transform[i],
                                                           Logic_CLOVER_Shield_PMT[j],
                                                           "CLOVER_Shield_PMT", // its name
                                                           LogicVacuumChamber,
                                                           false, // no boolean operations
                                                           i*16 + j,  // copy number
                                                           fCheckOverlaps); // checking overlaps
                
            }
            
            
            new G4PVPlacement(CLOVER_Shield_transform[i],
                              Logic_CLOVER_Shield_Heavimet,
                              "CLOVER_Shield_Heavimet",       // its name
                              LogicVacuumChamber,         // its mother  volume
                              false,           // no boolean operations
                              i,               // copy number
                              fCheckOverlaps); // checking overlaps
            
        }
    }

    ////////////////////////////////////////////////////
    //            OCL LaBr3 INITIALIZATION            //
    ////////////////////////////////////////////////////
    
    G4int copynumber;

    for(G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
        OCLLaBr3_position[i] = OCLLaBr3_Distance[i]
                                *G4ThreeVector( -sin(OCLLaBr3_theta[i]) * cos(OCLLaBr3_phi[i]), 
                                                cos(OCLLaBr3_theta[i]),
                                                sin(OCLLaBr3_theta[i]) * sin(OCLLaBr3_phi[i]));
        
        // OCLLaBr3_transform[i] = G4Transform3D(OCLLaBr3_rotm[i],OCLLaBr3_position[i]);
                
        /////////////////////////////
        //          LaBr3 Detectors
        if(OCLLaBr3_Presence[i])
        {
           labr3[i] = new OCLLaBr3();
           labr3[i]->SetRotation(OCLLaBr3_rotm[i]);
           labr3[i]->SetPosition(OCLLaBr3_position[i]);
           labr3[i]->Placement(copynumber = i,  PhysiVacuumChamber, fCheckOverlaps);           
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

        // OCLLaBr3_transform[i] = G4Transform3D(OCLLaBr3_rotm[i],OCLLaBr3_position[i]);

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
