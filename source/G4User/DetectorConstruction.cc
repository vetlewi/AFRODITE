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

#include "user/DetectorConstruction.hh"

#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Sphere.hh>
#include <G4LogicalVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4AssemblyVolume.hh>

#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4GeometryTolerance.hh>

#include <G4UserLimits.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4SystemOfUnits.hh>
#include <globals.hh>


#include <G4PhysicalVolumeStore.hh>
#include <G4SolidStore.hh>


#include <G4MagneticField.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

#include "detector/CloverFactory.hh"
#include "detector/S2Factory.hh"

#include <meshreader/incbin.h>
#include <meshreader/MeshReader.hh>


#include <fstream>
#include <string>

#ifndef SRC_PATH
#define SRC_PATH "../"
#endif // SRC_PATH

#ifndef PLY_PATH
#define PLY_PATH SRC_PATH"Mesh-Models"
#endif // PLY_PATH

INCBIN(TargetChamber, PLY_PATH"/STRUCTURES/MathisTC/target_chamber_new_sealed_fused_10umTolerance.ply");
INCBIN(DetectorFrame, PLY_PATH"/STRUCTURES/Frame/OuterFrame_100umTolerance.ply");

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
    , fCheckOverlaps( false )
    , WorldSize( 5. * m )
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

    /////////////////////////////////
    ////    S2 Silicon SETUP      ///
    /////////////////////////////////


    S2_Silicon_AllPresent_Override = false;
    S2_Silicon_AllAbsent_Override = false;

    S2_Silicon_Presence[0] = true;
    S2_Silicon_Thickness[0] = 330*um;
    S2_Silicon_Distance[0] = 22.5*mm;
    S2_Silicon_phi[0] = 0;
    S2_Silicon_theta[0] = 0;

    S2_Silicon_Presence[1] = true;
    S2_Silicon_Thickness[1] = 1090*um;
    S2_Silicon_Distance[1] = 25.5*mm;
    S2_Silicon_phi[1] = 0;
    S2_Silicon_theta[1] = 0;

    for (G4int i=0; i<numberOfSi; i++)
    {
        S2_Silicon_rotm[i].rotateY(S2_Silicon_theta[i]);
        S2_Silicon_rotm[i].rotateZ(S2_Silicon_phi[i]);

        if(S2_Silicon_AllPresent_Override) S2_Silicon_Presence[i] = true;
        if(S2_Silicon_AllAbsent_Override) S2_Silicon_Presence[i] = false;
        if(S2_Silicon_AllPresent_Override && S2_Silicon_AllAbsent_Override) S2_Silicon_Presence[i] = false;
    }
    
    /////////////////////////////
    ////    CLOVER SETUP      ///
    /////////////////////////////
    
    CLOVER_AllPresent_Override = false;
    CLOVER_AllAbsent_Override = false;
    
    CLOVER_Shield_AllPresent_Override = false;
    CLOVER_Shield_AllAbsent_Override = false;
    
    
    //  CLOVER 1
    CLOVER_Presence[0] = true;
    CLOVER_Shield_Presence[0] = true;
    CLOVER_Distance[0] = (21.-7.3)*cm;
    CLOVER_phi[0] = 0.*deg;
    CLOVER_theta[0] = 90.*deg;
    
    //  CLOVER 2
    CLOVER_Presence[1] = true;
    CLOVER_Shield_Presence[1] = true;
    CLOVER_Distance[1] = (21.-7.3)*cm;
    CLOVER_phi[1] = 45*deg;
    CLOVER_theta[1] = 90*deg;
    
    //  CLOVER 3
    CLOVER_Presence[2] = true;
    CLOVER_Shield_Presence[2] = true;
    CLOVER_Distance[2] = (21.-7.3)*cm;
    CLOVER_phi[2] = 135*deg;
    CLOVER_theta[2] = 90*deg;

    //  CLOVER 4
    CLOVER_Presence[3] = true;
    CLOVER_Shield_Presence[3] = true;
    CLOVER_Distance[3] = (21.-7.3)*cm;
    CLOVER_phi[3] = 180*deg;
    CLOVER_theta[3] = 90*deg;

    //  CLOVER 5
    CLOVER_Presence[4] = true;
    CLOVER_Shield_Presence[4] = true;
    CLOVER_Distance[4] = (21.-7.3)*cm;
    CLOVER_phi[4] = 315*deg;
    CLOVER_theta[4] = 90*deg;
    
    //  CLOVER 6
    CLOVER_Presence[5] = true;
    CLOVER_Shield_Presence[5] = true;
    CLOVER_Distance[5] = (21.-7.3)*cm;
    CLOVER_phi[5] = 0*deg;
    CLOVER_theta[5] = 135*deg;
    
    //  CLOVER 7
    CLOVER_Presence[6] = true;
    CLOVER_Shield_Presence[6] = true;
    CLOVER_Distance[6] = (21.-7.3)*cm;
    CLOVER_phi[6] = 90*deg;
    CLOVER_theta[6] = 135*deg;

    //  CLOVER 8
    CLOVER_Presence[7] = true;
    CLOVER_Shield_Presence[7] = true;
    CLOVER_Distance[7] = (21.-7.3)*cm;
    CLOVER_phi[7] = 180*deg;
    CLOVER_theta[7] = 135*deg;
    
    for (G4int i=0; i<numberOf_CLOVER; i++)
    {

        CLOVER_rotm[i].rotateX(180.*deg);
        CLOVER_rotm[i].rotateY(CLOVER_theta[i]);
        CLOVER_rotm[i].rotateZ(CLOVER_phi[i]);

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
    OCLLaBr3_Distance[0] = 14.*cm;
    OCLLaBr3_phi[0] = 0.*deg;
    OCLLaBr3_theta[0] = 45.*deg;
    
    //  LaBr3 Detector 2
    OCLLaBr3_Presence[1] = true;
    OCLLaBr3_Distance[1] = 14.*cm;
    OCLLaBr3_phi[1] = 90.*deg;
    OCLLaBr3_theta[1] = 45.*deg;

    // LaBr3 Detector 3
    OCLLaBr3_Presence[2] = false;
    OCLLaBr3_Distance[2] = 14.*cm;
    OCLLaBr3_phi[2] = 180.*deg;
    OCLLaBr3_theta[2] = 45.*deg;

    //  LaBr3 Detector 4
    OCLLaBr3_Presence[3] = false;
    OCLLaBr3_Distance[3] = 14.*cm;
    OCLLaBr3_phi[3] = 270.*deg;
    OCLLaBr3_theta[3] = 45.*deg;

    // LaBr3 Detector 5
    OCLLaBr3_Presence[4] = false;
    OCLLaBr3_Distance[4] = 14.*cm;
    OCLLaBr3_phi[4] = 225.*deg;
    OCLLaBr3_theta[4] = 90.*deg;

    //  LaBr3 Detector 6
    OCLLaBr3_Presence[5] = false;
    OCLLaBr3_Distance[5] = 14.*cm;
    OCLLaBr3_phi[5] = 270.*deg;
    OCLLaBr3_theta[5] = 135.*deg;

    
    for (G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
        OCLLaBr3_rotm[i].rotateY(OCLLaBr3_theta[i]);
        OCLLaBr3_rotm[i].rotateZ(OCLLaBr3_phi[i]);

        if(OCLLaBr3_AllPresent_Override) OCLLaBr3_Presence[i] = true;
        if(OCLLaBr3_AllAbsent_Override) OCLLaBr3_Presence[i] = false;
        if(OCLLaBr3_AllPresent_Override && OCLLaBr3_AllAbsent_Override) OCLLaBr3_Presence[i] = false;
    }

    ////////////////////////////
    ////    FTA LaBr3 Detectors
    ////////////////////////////

    // NOTE: Angles are defined differently for FTA detectors.

    FTALaBr3_AllPresent_Override = false;
    FTALaBr3_AllAbsent_Override = false;


    // LaBr3 Detector 1
    FTALaBr3_Presence[0] = true;
    FTALaBr3_Distance[0] = 14.*cm;
    FTALaBr3_phi[0] = 45.*deg;
    FTALaBr3_theta[0] = 55*deg;


    // LaBr3 Detector 2
    FTALaBr3_Presence[1] = true;
    FTALaBr3_Distance[1] = 14.*cm;
    FTALaBr3_phi[1] = 135.*deg;
    FTALaBr3_theta[1] = 55*deg;

    // LaBr3 Detector 3
    FTALaBr3_Presence[2] = true;
    FTALaBr3_Distance[2] = 14.*cm;
    FTALaBr3_phi[2] = 225.*deg;
    FTALaBr3_theta[2] = 55*deg;

    // LaBr3 Detector 4
    FTALaBr3_Presence[3] = true;
    FTALaBr3_Distance[3] = 14.*cm;
    FTALaBr3_phi[3] = 225.*deg;
    FTALaBr3_theta[3] = (180-55)*deg;


    // LaBr3 Detector 5
    FTALaBr3_Presence[4] = true;
    FTALaBr3_Distance[4] = 14.*cm;
    FTALaBr3_phi[4] = 315.*deg;
    FTALaBr3_theta[4] = 55.*deg;

    // LaBr3 Detector 6
    FTALaBr3_Presence[5] = true;
    FTALaBr3_Distance[5] = 14.*cm;
    FTALaBr3_phi[5] = 315.*deg;
    FTALaBr3_theta[5] = (180.-55.)*deg;


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
    AFRODITE_Target_Presence = true;
    AFRODITE_Frame_Presence = true;
    
    // Define volumes
    return DefineVolumes();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

    //////////////////////////////////////
    //          Get Materials           //
    //////////////////////////////////////
    
    ////    NIST Defined Elemental Material
    G4Material* G4_Al_Material  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    ////    NIST Defined Materials and Compounds
    G4Material* G4_Galactic_Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    G4Material* G4_Air_Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    G4Material* G4_LITHIUM_CARBONATE_Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_LITHIUM_CARBONATE");
    G4Material* G4_concrete_Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

    // Create factories
    bool have_HPGe = std::any_of(CLOVER_Presence, CLOVER_Presence+sizeof(CLOVER_Presence),
                                 [](const G4bool &p){ return p; });
    bool have_shield = std::any_of(CLOVER_Shield_Presence, CLOVER_Shield_Presence+sizeof(CLOVER_Shield_Presence),
                                   [](const G4bool &p){ return p; });
    CloverFactory cloverFactory(have_HPGe, have_shield);
    OCLLaBr3 ocl_factory(false);
    FTALaBr3 labr_factory;

    G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
    MeshReader targetChamber({gTargetChamberData, gTargetChamberSize}, "TargetChamber", offset_MathisTC);
    MeshReader DetectorFrame({gDetectorFrameData, gDetectorFrameSize}, "DetectorFrame", offset_MathisTC);
    
    
    //////////////////////////////////////////////////////////
    //                      WORLD                           //
    //////////////////////////////////////////////////////////
    
    G4Box* SolidWorld = new G4Box("World", (5*m + 50*cm)/2, 200*cm/2., (5*m + 50*cm)/2);
    
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
    //              Concrete walls                          //
    //////////////////////////////////////////////////////////

    G4Box *solidConcrete = new G4Box("Concrete_box", (5*m + 50*cm)/2, 200*cm/2., (5*m + 50*cm)/2);
    G4LogicalVolume*
    LogicConcrete = new G4LogicalVolume(solidConcrete,                //its solid
                                        G4_concrete_Material,      //its material
                                        "Concrete");                  //its name

    new G4PVPlacement(0, G4ThreeVector(),
                                     LogicConcrete,
                                     "ConcretePhysical",
                                     LogicWorld,
                                     false,
                                     0,
                                     fCheckOverlaps);

    //////////////////////////////////////////////////////////
    //          Vaccum inside the concrete (air actually)
    //////////////////////////////////////////////////////////

    G4Box* air_solid = new G4Box("Air", (5*m)/2, 150*cm/2., (5*m)/2);
    G4LogicalVolume* LogicVacuumChamber = new G4LogicalVolume(air_solid, G4_Air_Material,"VacuumChamber");
    PhysiVacuumChamber =
            new G4PVPlacement(0, G4ThreeVector(), LogicVacuumChamber,
                              "VaccumChamber",LogicConcrete, false, 0, fCheckOverlaps);

    //////////////////////////////////////////////////////////
    //                      VACUUM CHAMBER
    //////////////////////////////////////////////////////////
    
   /* G4ThreeVector positionVacuumChamber = G4ThreeVector(0,0,0);

    
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
                          fCheckOverlaps); // checking overlaps*/
    
    
    
    
    
    //////////////////////////////////////////////////////////
    //              Scattering Chamber - CADMesh
    //////////////////////////////////////////////////////////
    
    if(AFRODITE_MathisTC_Presence)
    {
        /*G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
        const char *TC_path = PLY_PATH"/STRUCTURES/MathisTC/target_chamber_new_sealed_fused_10umTolerance.ply";
        G4cout << "Reading TC from " << TC_path << G4endl;
        CADMesh * mesh_MathisTC = new CADMesh(TC_path, "PLY", mm, offset_MathisTC, false);*/
        G4VSolid * SolidMathisTC = targetChamber.GetSolid();//mesh_MathisTC->TessellatedMesh();
        
        G4LogicalVolume* LogicMathisTC = new G4LogicalVolume(SolidMathisTC, G4_Al_Material, "BACTAR", 0, 0, 0);
        G4RotationMatrix *rot = new G4RotationMatrix();
        rot->rotateY(180*deg);
        new G4PVPlacement(rot,               // no rotation
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
    //              Detector frame - CADMesh
    //////////////////////////////////////////////////////////

    if(AFRODITE_Frame_Presence)
    {
        /*G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
        const char *TC_path = PLY_PATH"/STRUCTURES/MathisTC/target_chamber_new_sealed_fused_10umTolerance.ply";
        G4cout << "Reading TC from " << TC_path << G4endl;
        CADMesh * mesh_MathisTC = new CADMesh(TC_path, "PLY", mm, offset_MathisTC, false);*/
        G4VSolid * SolidFrame = DetectorFrame.GetSolid();//mesh_MathisTC->TessellatedMesh();

        G4LogicalVolume* LogicFrame = new G4LogicalVolume(SolidFrame, G4_Al_Material, "FRAME_logic", 0, 0, 0);
        G4RotationMatrix *rot = new G4RotationMatrix();
        //rot->rotateY(180*deg);
        new G4PVPlacement(rot,               // no rotation
                          G4ThreeVector(), // at (x,y,z)
                          LogicFrame,       // its logical volume
                          "FRAME_phys",       // its name
                          LogicVacuumChamber,         // its mother  volume
                          false,           // no boolean operations
                          0,               // copy number
                          fCheckOverlaps); // checking overlaps


        //  Visualisation
        G4VisAttributes* AFRODITE_Frame_VisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
        AFRODITE_Frame_VisAtt->SetForceSolid(true);
        LogicFrame->SetVisAttributes(AFRODITE_Frame_VisAtt);
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

    //////////////////////////////////////////////////////////
    //                      S2 detectors
    //////////////////////////////////////////////////////////


   for (int i = 0 ; i < numberOfSi ; ++i){

       if ( !S2_Silicon_Presence[i] )
           continue;

        S2Factory s2factory(S2_Silicon_Thickness[i]);
        auto *assembly = s2factory.GetAssembly(0, fCheckOverlaps);

       S2_Silicon_position[i] = S2_Silicon_Distance[i]
                            *G4ThreeVector( sin(S2_Silicon_theta[i]) * cos(S2_Silicon_phi[i]),
                                            sin(S2_Silicon_theta[i]) * sin(S2_Silicon_phi[i]),
                                            cos(S2_Silicon_theta[i]));

       S2_Silicon_transform[i] = G4Transform3D(S2_Silicon_rotm[i],S2_Silicon_position[i]);

        assembly->MakeImprint(LogicVacuumChamber, S2_Silicon_transform[i], i);

    }
    
    ////////////////////////////////////////////////////
    //               CLOVER INITIALIZATION            //
    ////////////////////////////////////////////////////


    for(G4int i=0; i< numberOf_CLOVER ; i++)
    {
        if ( !CLOVER_Presence[i] )
            continue;

        /*CLOVER_position[i] = CLOVER_Distance[i]
        *G4ThreeVector( -sin(CLOVER_theta[i]) * cos(CLOVER_phi[i]),
                        cos(CLOVER_theta[i]),
                        sin(CLOVER_theta[i]) * sin(CLOVER_phi[i]));*/

        CLOVER_position[i] = CLOVER_Distance[i]
                             *G4ThreeVector( sin(CLOVER_theta[i]) * cos(CLOVER_phi[i]),
                                             sin(CLOVER_theta[i]) * sin(CLOVER_phi[i]),
                                             cos(CLOVER_theta[i]));

        CLOVER_transform[i] = G4Transform3D(CLOVER_rotm[i],CLOVER_position[i]);

        auto *assembly = cloverFactory.GetAssembly(i, fCheckOverlaps);
        assembly->MakeImprint(LogicVacuumChamber, CLOVER_transform[i], i);

        // Since assembly class will do create all the pysical volumes that
        // we haven't made yet our self we will have to hack the names
        // and copy number that we expect into the physical volumes our self.
        // It isnt a good way of doing it, but it should work:/
        // This is also the point where we will check that nothing overlaps, since
        // we dont properly check during construction.
        auto nVolumes = assembly->TotalImprintedVolumes();
        int copy_no = 0;
        for ( auto vol = assembly->GetVolumesIterator() ;
              vol < assembly->GetVolumesIterator() + nVolumes ; ++vol){
            if ( fCheckOverlaps )
                (*vol)->CheckOverlaps();
            if ( (*vol)->GetName().contains("LogicCLOVERShieldBGOCrystal") ) {
                (*vol)->SetCopyNo(i * numberOf_BGO_Crystals + copy_no++);
                (*vol)->SetName("CLOVER_Shield_BGOCrystal");
            }
        }
    }

    ////////////////////////////////////////////////////
    //            OCL LaBr3 INITIALIZATION            //
    ////////////////////////////////////////////////////
    for(G4int i=0; i<numberOf_OCLLaBr3; i++)
    {
        OCLLaBr3_position[i] = OCLLaBr3_Distance[i]
                              *G4ThreeVector( sin(OCLLaBr3_theta[i])*cos(OCLLaBr3_phi[i]),
                                              sin(OCLLaBr3_theta[i]*sin(OCLLaBr3_phi[i])),
                                              cos(OCLLaBr3_theta[i]));


        if(OCLLaBr3_Presence[i])
        {
            auto *assembly = ocl_factory.GetAssembly(i, fCheckOverlaps);
            assembly->MakeImprint(LogicVacuumChamber, OCLLaBr3_position[i], &OCLLaBr3_rotm[i], i);
            auto nVolumes = assembly->TotalImprintedVolumes();
            for ( auto vol = assembly->GetVolumesIterator() ;
                  vol < assembly->GetVolumesIterator() + nVolumes ; ++vol){
                if ( fCheckOverlaps )
                    (*vol)->CheckOverlaps();
            }
        }

    }


    ////////////////////////////////////////////////////
    //            FTA LaBr3 INITIALIZATION            //
    ////////////////////////////////////////////////////
    for(G4int i=0; i < numberOf_FTALaBr3 ; i++)
    {
        FTALaBr3_position[i] = (FTALaBr3_Distance[i])
                               *G4ThreeVector( -sin(FTALaBr3_theta[i]) * cos(FTALaBr3_phi[i]),
                                               cos(FTALaBr3_theta[i]),
                                               sin(FTALaBr3_theta[i]) * sin(FTALaBr3_phi[i]));

        /////////////////////////////
        //          LaBr3 Detectors
        if(FTALaBr3_Presence[i])
        {
            auto *assembly = labr_factory.GetAssembly(i, fCheckOverlaps);
            assembly->MakeImprint(LogicVacuumChamber, FTALaBr3_position[i], &FTALaBr3_rotm[i], i);
        }

    }
    
    
    //////////////////////////////////////////////////////////
    //                      VISUALISATION
    //////////////////////////////////////////////////////////
    
    G4VisAttributes* World_VisAtt= new G4VisAttributes(G4Colour(0., 0., 0.));
    World_VisAtt->SetVisibility(true);
    LogicWorld->SetVisAttributes(World_VisAtt);
    
    G4VisAttributes* Target_VisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    LogicTarget->SetVisAttributes(Target_VisAtt);
    
    
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

/*void DetectorConstruction::ConstructSDandField()
{

    // FTA detectors
    auto *FTALaBrSD = new EnergyDepSD("LaBr/FTA", "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(FTALaBrSD);
    SetSensitiveDetector("FTA_Crystal", FTALaBrSD, true);
}*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
