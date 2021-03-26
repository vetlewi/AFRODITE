//
// Created by Vetle Wegner Ingeberg on 26/03/2021.
//

#include "S2Factory.hh"

#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4NistManager.hh>
#include <G4AssemblyVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4SubtractionSolid.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4PVPlacement.hh>

S2Factory::S2Factory(const G4double &thickness)
    : fMatPCB( G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE") )
    , fMatSi( G4NistManager::Instance()->FindOrBuildMaterial("G4_Si") )
    , PCB_Solid( new G4SubtractionSolid("PCB_Solid",
                                        new G4Box("PCB_before_cutout", 127./2*mm, 127./2*mm, 1.6e+3*um/2),
                                        new G4Tubs("Wafer_cutout", 0*mm, 76*mm/2, 1.61e+3*um/2, 0*deg, 360*deg)) )
    , Inactive_area_inner_Solid( new G4Tubs("Inactive_area_inner_Solid", 10.0*mm, 23.06*mm/2, thickness/2, 0*deg, 360*deg) )
    , Inactive_area_outer_Solid( new G4Tubs("Inactive_area_outer_Solid", 23.06*mm/2 + numberOfRings*0.491*mm, 76*mm/2, thickness/2, 0*deg, 360*deg ) )
    , Active_area_solid( new G4Tubs("Active_area_Solid", 23.06*mm/2, 23.06*mm/2 + numberOfRings*0.491*mm, thickness/2, 0*deg, 360*deg) )
{
}

G4AssemblyVolume * S2Factory::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., 0.);
    G4RotationMatrix rot0;

    auto *PCB_VisAtt = new G4VisAttributes(G4Colour::Yellow());
    PCB_VisAtt->SetForceSolid(true);
    auto *PCB_Logical = new G4LogicalVolume(PCB_Solid, fMatPCB, "PCB_logical");
    PCB_Logical->SetVisAttributes(PCB_VisAtt);
    assembly->AddPlacedVolume(PCB_Logical, pos0, &rot0);

    auto Inactive_VisAtt = new G4VisAttributes(G4Colour::Green());
    Inactive_VisAtt->SetForceSolid(true);
    auto *Inactive_area_inner_logical = new G4LogicalVolume(Inactive_area_inner_Solid, fMatSi, "Inactive_area_inner_logical");
    auto *Inactive_area_outer_logical = new G4LogicalVolume(Inactive_area_outer_Solid, fMatSi, "Inactive_area_outer_logical");
    Inactive_area_inner_logical->SetVisAttributes(Inactive_VisAtt);
    Inactive_area_outer_logical->SetVisAttributes(Inactive_VisAtt);

    assembly->AddPlacedVolume(Inactive_area_inner_logical, pos0, &rot0);
    assembly->AddPlacedVolume(Inactive_area_outer_logical, pos0, &rot0);

    /*new G4PVPlacement(0, pos0, Inactive_area_inner_logical,
                      "Inactive_area_inner_phys",
                      PCB_Logical,
                      false,
                      copy_no,
                      checkOverlap);

    new G4PVPlacement(0, pos0, Inactive_area_outer_logical,
                      "Inactive_area_outer_phys",
                      PCB_Logical,
                      false,
                      copy_no,
                      checkOverlap);*/

    auto *active_VisAtt = new G4VisAttributes(G4Colour::Red());
    auto *Active_area_logical = new G4LogicalVolume(Active_area_solid, fMatSi, "Active_area_logical");
    Active_area_logical->SetVisAttributes(active_VisAtt);
    assembly->AddPlacedVolume(Active_area_logical, pos0, &rot0);

    return assembly;
}