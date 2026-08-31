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
#include <G4Tubs.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4GeometryTolerance.hh>

#include <G4UserLimits.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4SystemOfUnits.hh>
#include <G4String.hh>
#include <globals.hh>


#include <G4PhysicalVolumeStore.hh>
#include <G4SolidStore.hh>


#include <G4MagneticField.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

#include "detector/CloverFactory.hh"
#include "detector/OCLLaBr3.hh"
#include "detector/FTALaBr3.hh"
#include "detector/S2Factory.hh"

#include "DetectorSetupMessenger.hh"
#include "Frame/DetectorFrame.hh"

#include <meshreader/MeshReader.hh>

// Generated at build time by bin2cpp (see cmake/EmbedBinary.cmake) from the
// corresponding .ply files under Mesh-Models/.
#include "TargetChamber.mesh.hh"
#include "DetectorFrame.mesh.hh"

#include <cmath>
#include <memory>
#include <fstream>
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
    , fCheckOverlaps( false )
    , WorldSize( 5. * m )
    , fMessenger( new DetectorSetupMessenger( fFrame ) )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
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

    // Detector factories are created lazily below, only for the detector types
    // that actually occupy a frame slot.

    G4ThreeVector offset_MathisTC = G4ThreeVector(0*cm, 0*cm, 0*cm);
    MeshReader targetChamber({TargetChamber_data, TargetChamber_size}, "TargetChamber", offset_MathisTC);
    MeshReader frameMesh({DetectorFrame_data, DetectorFrame_size}, "DetectorFrame", offset_MathisTC);
    
    
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
    //              Scattering Chamber
    //////////////////////////////////////////////////////////

    if(fFrame.place_chamber)
    {
        G4VSolid * SolidMathisTC = targetChamber.GetSolid();
        
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
    //              Detector frame
    //////////////////////////////////////////////////////////

    if(fFrame.place_frame)
    {
        G4VSolid * SolidFrame = frameMesh.GetSolid();

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
    
    if(fFrame.place_target)
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
    //               FRAME SLOT DETECTORS             //
    ////////////////////////////////////////////////////
    //
    // Every frame-mounted detector is placed from its slot. The slot direction
    // (theta, phi) is fixed by the frame; the occupant type and distance come
    // from the runtime configuration (see DetectorSetupMessenger). Factories are
    // built lazily, only for the detector types that are actually used.

    std::unique_ptr<CloverFactory> cloverWithShield;
    std::unique_ptr<CloverFactory> cloverNoShield;
    std::unique_ptr<OCLLaBr3>      oclFactory;
    std::unique_ptr<FTALaBr3>      ftaFactory;

    auto placeSlot = [&](const Slot &slot, int copy_no)
    {
        if ( !slot.occupied() )
            return;

        // The detector centre lies along the (fixed) slot direction.
        // (non-const: G4AssemblyVolume::MakeImprint takes it by reference)
        G4ThreeVector position = slot.distance * G4ThreeVector(
                std::sin(slot.theta) * std::cos(slot.phi),
                std::sin(slot.theta) * std::sin(slot.phi),
                std::cos(slot.theta));

        G4RotationMatrix rotm;
        G4AssemblyVolume *assembly = nullptr;
        bool is_clover = false;

        switch ( slot.type )
        {
            case Detector::Type::clover:
            {
                rotm.rotateX(180.*deg);
                rotm.rotateY(slot.theta);
                rotm.rotateZ(slot.phi);
                auto &factory = slot.shield ? cloverWithShield : cloverNoShield;
                if ( !factory )
                    factory = std::make_unique<CloverFactory>(true, slot.shield);
                assembly = factory->GetAssembly(copy_no, fCheckOverlaps);
                is_clover = true;
                break;
            }
            case Detector::Type::ocl_labr:
            {
                rotm.rotateY(slot.theta);
                rotm.rotateZ(slot.phi);
                if ( !oclFactory )
                    oclFactory = std::make_unique<OCLLaBr3>(false);
                assembly = oclFactory->GetAssembly(copy_no, fCheckOverlaps);
                break;
            }
            case Detector::Type::fta_labr:
            {
                // The FTA orientation is historically expressed with the polar
                // axis along +y; recover those native angles from the slot
                // direction so the detector is oriented as it always has been.
                const double theta_n = std::acos(std::sin(slot.theta) * std::sin(slot.phi));
                const double phi_n = std::atan2(std::cos(slot.theta),
                                                -std::sin(slot.theta) * std::cos(slot.phi));
                rotm.rotateX(270.*deg);
                rotm.rotateZ(theta_n);
                rotm.rotateY(phi_n);
                if ( !ftaFactory )
                    ftaFactory = std::make_unique<FTALaBr3>();
                assembly = ftaFactory->GetAssembly(copy_no, fCheckOverlaps);
                break;
            }
            default:
                return;
        }

        if ( is_clover )
        {
            G4Transform3D transform(rotm, position);
            assembly->MakeImprint(LogicVacuumChamber, transform, copy_no);

            // The assembly machinery creates the physical volumes for us; relabel
            // the BGO shield crystals so the readout can identify them, and check
            // for overlaps here since we do not check during construction.
            auto nVolumes = assembly->TotalImprintedVolumes();
            int bgo_copy = 0;
            for ( auto vol = assembly->GetVolumesIterator() ;
                  vol < assembly->GetVolumesIterator() + nVolumes ; ++vol){
                if ( fCheckOverlaps )
                    (*vol)->CheckOverlaps();
#if G4VERSION_NUMBER >= 1100
                if ( G4StrUtil::contains((*vol)->GetName(), "LogicCLOVERShieldBGOCrystal") ) {
#else
                if ( G4StrUtil::contains((*vol)->GetName(), "LogicCLOVERShieldBGOCrystal") ) {
#endif // G4VERSION_NUMBER < 1100
                    (*vol)->SetCopyNo(copy_no * numberOf_BGO_Crystals + bgo_copy++);
                    (*vol)->SetName("CLOVER_Shield_BGOCrystal");
                }
            }
        }
        else
        {
            assembly->MakeImprint(LogicVacuumChamber, position, &rotm, copy_no);
            if ( fCheckOverlaps ){
                auto nVolumes = assembly->TotalImprintedVolumes();
                for ( auto vol = assembly->GetVolumesIterator() ;
                      vol < assembly->GetVolumesIterator() + nVolumes ; ++vol){
                    (*vol)->CheckOverlaps();
                }
            }
        }
    };

    for ( int i = 0 ; i < DetectorFrame::num_large ; ++i )
        placeSlot(fFrame.large(i), i);
    for ( int i = 0 ; i < DetectorFrame::num_small ; ++i )
        placeSlot(fFrame.small(i), i);
    
    
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
