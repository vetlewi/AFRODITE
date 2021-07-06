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

#include "detector/DetectorFactories.hh"
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

INCBIN(TargetChamber, PLY_PATH"/STRUCTURES/MathisTC/target_chamber_new_sealed_fused_100umTolerance.ply");
INCBIN(DetectorFrame, PLY_PATH"/STRUCTURES/Frame/OuterFrame_100umTolerance.ply");

constexpr double std_theta[] = {135.*deg, 135.*deg, 135.*deg, 135.*deg,
                                   90.*deg, 90.*deg, 90.*deg, 90.*deg, 90.*deg, 90.*deg, 90.*deg, 90.*deg,
                                   45.*deg, 45.*deg, 45.*deg, 45.*deg};

constexpr double std_phi[] = {90.*deg, 180.*deg, 270.*deg, 0.*deg,
                              90.*deg, 135.*deg, 180.*deg, 215.*deg, 270.*deg, 315.*deg, 0.*deg, 45.*deg,
                              90.*deg, 180.*deg, 270.*deg, 0.*deg};

constexpr double small_theta[] = {125.*deg, 125.*deg, 125.*deg, 125.*deg,
                                  55.*deg, 55.*deg, 55.*deg, 55.*deg};

constexpr double small_phi[] = {135.*deg, 225.*deg, 315.*deg, 45.*deg,
                                135.*deg, 225.*deg, 315.*deg, 45.*deg};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
    , fCheckOverlaps( false )
    , messenger( new DetectorSetupMessenger )
    , WorldSize( 5. * m )
    , num_particle_arrays( 0 )
    , num_clover( 0 )
    , num_ocl( 0 )
    , num_fta( 0 )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
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

    Detector::DetectorFactories factories;

    std::unique_ptr<MeshReader> targetChamber;
    std::unique_ptr<MeshReader> detectorFrame;

    if ( messenger->TargetChamberPresent() ) {
        targetChamber.reset(new MeshReader({gTargetChamberData, gTargetChamberSize},
                                           "TargetChamber",
                                           G4ThreeVector(0 * cm, 0 * cm, 0 * cm)));
    }
    if ( messenger->FramePresent() ){
        detectorFrame.reset(new MeshReader({gDetectorFrameData, gDetectorFrameSize},
                                           "DetectorFrame",
                                           G4ThreeVector(0 * cm, 0 * cm, 0 * cm)));
    }

    //////////////////////////////////////////////////////////
    //                      WORLD                           //
    //////////////////////////////////////////////////////////

    G4Box* SolidWorld = new G4Box("World", (5*m + 50*cm)/2, 200*cm/2., (5*m + 50*cm)/2);

    G4LogicalVolume *LogicWorld = new G4LogicalVolume(SolidWorld,                //its solid
                                                      G4_Galactic_Material,      //its material
                                                "World");                  //its name

    G4VisAttributes* World_VisAtt= new G4VisAttributes(G4Colour(0., 0., 0.));
    World_VisAtt->SetVisibility(true);
    LogicWorld->SetVisAttributes(World_VisAtt);

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
    //                 Air inside concrete                  //
    //////////////////////////////////////////////////////////

    G4Box* air_solid = new G4Box("Air", (5*m)/2, 150*cm/2., (5*m)/2);
    G4LogicalVolume* LogicVacuumChamber = new G4LogicalVolume(air_solid, G4_Air_Material,"VacuumChamber");
    PhysiVacuumChamber =
            new G4PVPlacement(0, G4ThreeVector(), LogicVacuumChamber,
                              "VaccumChamber",LogicConcrete, false, 0, fCheckOverlaps);


    //////////////////////////////////////////////////////////
    //                    Detector frame                    //
    //////////////////////////////////////////////////////////

    if ( detectorFrame ){

        G4VSolid *solidDF = detectorFrame->GetSolid();
        G4LogicalVolume *logicDF = new G4LogicalVolume(solidDF, G4_Al_Material, "DetectorFrameLogic");

        G4VisAttributes* AFRODITE_DF_VisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
        AFRODITE_DF_VisAtt->SetForceSolid(true);
        logicDF->SetVisAttributes(AFRODITE_DF_VisAtt);

        new G4PVPlacement(0, G4ThreeVector(),
                          logicDF,
                          "DetectorFrame",
                          LogicVacuumChamber,
                          false,
                          0,
                          fCheckOverlaps);

    }

    //////////////////////////////////////////////////////////
    //                 Target Chamber                       //
    //////////////////////////////////////////////////////////

    if( targetChamber )
    {

        G4VSolid * SolidMathisTC = targetChamber->GetSolid();

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
    //                      Target                          //
    //////////////////////////////////////////////////////////

    if ( messenger->TargetPresent() ){
        G4Box* SolidTarget = new G4Box("Target", (20./2)*mm, (20./2)*mm, (2.42/2)*um);
        G4LogicalVolume* LogicTarget = new G4LogicalVolume(SolidTarget, G4_LITHIUM_CARBONATE_Material,"Target");

        G4VisAttributes* Target_VisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
        LogicTarget->SetVisAttributes(Target_VisAtt);

        new G4PVPlacement(0,               // no rotation
                          G4ThreeVector(0, 0, 0), // at (x,y,z)
                          LogicTarget,       // its logical volume
                          "Target",       // its name
                          LogicVacuumChamber,         // its mother  volume
                          false,           // no boolean operations
                          0,               // copy number
                          fCheckOverlaps); // checking overlaps
    }

    //////////////////////////////////////////////////////////
    //                  Particle telescope                  //
    //////////////////////////////////////////////////////////

    for ( auto &volume : messenger->GetParticleArray() ){
        if ( volume.type == Detector::none )
            continue;

        auto factory = reinterpret_cast<Detector::S2Factory *>(factories.GetFactory(volume.type))->GetFactory(volume.thickness);;
        auto assembly = factory.GetAssembly(num_particle_arrays, fCheckOverlaps);

        auto pos = volume.distance*G4ThreeVector(0, 0, 1);

        auto rot = G4Transform3D(G4RotationMatrix(), pos);

        assembly->MakeImprint(LogicVacuumChamber, rot, num_particle_arrays++);
    }

    //////////////////////////////////////////////////////////
    //                  Standard detectors                  //
    //////////////////////////////////////////////////////////

    int pos_no = 0;
    std::map<Detector::Type, int*> map_num = {{Detector::clover, &num_clover}, {Detector::ocl_labr, &num_ocl}, {Detector::fta_labr, &num_fta}};
    std::map<Detector::Type, double> map_rot = {{Detector::clover, 180.*deg}, {Detector::ocl_labr, 0.*deg}, {Detector::fta_labr, 0*deg}};

    for ( auto &volume : messenger->GetStdSlot() ){
        if ( volume.type == Detector::none ) {
            ++pos_no;
            continue;
        }

        auto factory = factories.GetFactory(volume.type);
        int *num = map_num.find(volume.type)->second;
        auto assembly = factory->GetAssembly(*num, fCheckOverlaps);

        auto pos = volume.distance * G4ThreeVector(sin(std_theta[pos_no]) * cos(std_phi[pos_no]),
                                                   sin(std_theta[pos_no]) * sin(std_phi[pos_no]),
                                                   cos(std_theta[pos_no]));
        G4RotationMatrix rotm;
        rotm.rotateX(map_rot[volume.type]);
        rotm.rotateY(std_theta[pos_no]);
        rotm.rotateZ(std_phi[pos_no]);

        auto transform = G4Transform3D(rotm, pos);

        assembly->MakeImprint(LogicVacuumChamber, transform, *num);

        if ( volume.type == Detector::clover ){
            auto nVolumes = assembly->TotalImprintedVolumes();
            int copy_no = 0;
            for ( auto vol = assembly->GetVolumesIterator() ;
                  vol < assembly->GetVolumesIterator() + nVolumes ; ++vol){
                if ( fCheckOverlaps )
                    (*vol)->CheckOverlaps();
                if ( (*vol)->GetName().contains("LogicCLOVERShieldBGOCrystal") ) {
                    (*vol)->SetCopyNo(num_clover * numberOf_BGO_Crystals + copy_no++);
                    (*vol)->SetName("CLOVER_Shield_BGOCrystal");
                }
            }
        }
        ++(*num);
        ++pos_no;
    }

    pos_no = 0;
    for ( auto &volume : messenger->GetSmallSlot() ){
        if ( volume.type == Detector::none ) {
            ++pos_no;
            continue;
        }

        auto factory = factories.GetFactory(volume.type);
        int *num = map_num.find(volume.type)->second;
        auto assembly = factory->GetAssembly(*num, fCheckOverlaps);

        auto pos = volume.distance * G4ThreeVector(sin(small_theta[pos_no]) * cos(small_phi[pos_no]),
                                                   sin(small_theta[pos_no]) * sin(small_phi[pos_no]),
                                                   cos(small_theta[pos_no]));
        G4RotationMatrix rotm;
        rotm.rotateX(map_rot[volume.type]);
        rotm.rotateY(small_theta[pos_no]);
        rotm.rotateZ(small_phi[pos_no]);

        auto transform = G4Transform3D(rotm, pos);

        assembly->MakeImprint(LogicVacuumChamber, transform, *num);

        ++(*num);
        ++pos_no;

    }



    return PhysiWorld;
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
    Detector::CloverFactory cloverFactory(have_HPGe, have_shield);
    Detector::OCLLaBr3 ocl_factory(false);
    Detector::FTALaBr3 labr_factory;

    G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
    MeshReader targetChamber({gTargetChamberData, gTargetChamberSize}, "TargetChamber", offset_MathisTC);
    MeshReader detectorFrame({gDetectorFrameData, gDetectorFrameSize}, "DetectorFrame", offset_MathisTC);
    
    
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



    //////////////////////////////////////////////////////////
    //              Detector frame - CADMesh
    //////////////////////////////////////////////////////////

    if ( AFRODITE_Frame_Presence ) {
        G4VSolid *solidDF = detectorFrame.GetSolid();

        G4LogicalVolume *logicDF = new G4LogicalVolume(solidDF, G4_Al_Material, "DetectorFrameLogic");

        G4VisAttributes* AFRODITE_DF_VisAtt = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
        AFRODITE_DF_VisAtt->SetForceSolid(true);
        logicDF->SetVisAttributes(AFRODITE_DF_VisAtt);

        new G4PVPlacement(0, G4ThreeVector(),
                          logicDF,
                          "DetectorFrame",
                          LogicVacuumChamber,
                          false,
                          0,
                          fCheckOverlaps);
    }
    
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

        Detector::S2Factory s2factory(S2_Silicon_Thickness[i]);
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
