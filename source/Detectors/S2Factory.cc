//
// Created by Vetle Wegner Ingeberg on 26/03/2021.
//

#include "detector/S2Factory.hh"

#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4NistManager.hh>
#include <G4AssemblyVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4PVPlacement.hh>

constexpr int numberOfSectors = 16;
constexpr int numberOfRings = 48;

S2Factory::S2Factory(const G4double &thickness)
    : Detector::DetectorFactory( Detector::Type::s2_silicon )
    , fMatPCB( G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE") )
    , fMatSi( G4NistManager::Instance()->FindOrBuildMaterial("G4_Si") )
    , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
    , PCB_Solid( new G4SubtractionSolid("PCB_Solid",
                                        new G4Box("PCB_before_cutout", 127./2*mm, 127./2*mm, 1.9*mm/2),
                                        new G4Tubs("Wafer_cutout", 0*mm, 76*mm/2, 1.91*mm/2, 0*deg, 360*deg)) )
    , PCB_vaccum( new G4Box("PCB_Vacuum", 127./2.*mm, 127./2.*mm, 1.9/2.*mm) )
    , Inactive_area_inner_Solid( new G4Tubs("Inactive_area_inner_Solid", 10.0*mm, 23.06*mm/2, thickness/2, 0*deg, 360*deg) )
    , Inactive_area_outer_Solid( new G4Tubs("Inactive_area_outer_Solid", 23.06*mm/2 + numberOfRings*0.491*mm, 76*mm/2, thickness/2, 0*deg, 360*deg ) )
    , Active_area_solid( new G4Tubs("Active_area_Solid", 23.06*mm/2, 23.06*mm/2 + numberOfRings*0.491*mm, thickness/2, 0*deg, 360*deg) )
{
}

G4AssemblyVolume *S2Factory::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., 0.);
    G4RotationMatrix rot0;

    // First we create the logical volumes
    auto *PCB_VisAtt = new G4VisAttributes(G4Colour::Yellow());
    PCB_VisAtt->SetForceSolid(true);
    auto *PCB_Logical = new G4LogicalVolume(PCB_Solid, fMatPCB, "PCB_logical");
    PCB_Logical->SetVisAttributes(PCB_VisAtt);

    auto *PCB_vaccum_VisAtt = new G4VisAttributes();
    PCB_vaccum_VisAtt->SetVisibility(false);
    auto *PCB_vaccum_logical = new G4LogicalVolume(PCB_vaccum, fMatVaccum, "PCB_vaccum_logical");
    PCB_vaccum_logical->SetVisAttributes(PCB_vaccum_VisAtt);

    // Place vaccum within the PCB
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), PCB_Logical,
                      "PCB_vaccum_physical",
                      PCB_vaccum_logical,
                      false,
                      copy_no,
                      checkOverlap);

    auto Inactive_VisAtt = new G4VisAttributes(G4Colour::Green());
    Inactive_VisAtt->SetForceSolid(true);
    auto *Inactive_area_inner_logical = new G4LogicalVolume(Inactive_area_inner_Solid, fMatSi, "Inactive_area_inner_logical");
    auto *Inactive_area_outer_logical = new G4LogicalVolume(Inactive_area_outer_Solid, fMatSi, "Inactive_area_outer_logical");
    Inactive_area_inner_logical->SetVisAttributes(Inactive_VisAtt);
    Inactive_area_outer_logical->SetVisAttributes(Inactive_VisAtt);

    // Place the inactive areas
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Inactive_area_inner_logical,
                      "Inactive_area_inner_phys",
                      PCB_vaccum_logical,
                      false,
                      copy_no,
                      checkOverlap);

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Inactive_area_outer_logical,
                      "Inactive_area_outer_phys",
                      PCB_vaccum_logical,
                      false,
                      copy_no,
                      checkOverlap);

    auto *active_VisAtt = new G4VisAttributes(G4Colour::Red());
    auto *Active_area_logical = new G4LogicalVolume(Active_area_solid, fMatSi, "Active_area_logical");
    Active_area_logical->SetVisAttributes(active_VisAtt);

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Active_area_logical,
                      "Si_active_area_phys",
                      PCB_vaccum_logical,
                      false,
                      copy_no,
                      checkOverlap);

    assembly->AddPlacedVolume(PCB_vaccum_logical, pos0, &rot0);
    return assembly;
}