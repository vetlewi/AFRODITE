#include "FTALaBr3.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Tubs.hh>
#include <G4AssemblyVolume.hh>
#include <G4VisAttributes.hh>

using namespace CLHEP;

extern G4Element *MakeIfNotFound(const G4String &name, const G4String &symbol, const G4double &Zeff, const G4double &Aeff);


FTALaBr3::FTALaBr3()
    : fMatLaBr3Housing( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
    , fMatLaBr3Reflector( G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON") )
    , fMatLaBr3Crystal( G4NistManager::Instance()->FindOrBuildMaterial("Labr3_Ce") )
    , fMatLaBr3LightGuide( G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS") )
    , fMatLaBr3PMTWindow( G4NistManager::Instance()->FindOrBuildMaterial("Borosilicate glass") )
    , CrystalHouse_Solid( new G4Tubs("CrystalHouse_Solid", 0.,57.5/2.,54.15/2.,0.*deg,360*deg) )
    , PMTHouse_Solid( new G4Tubs("PMTHouse_Solid", 30.,62./2.,(233.-45.)/2.,0.*deg,360*deg) )
    , PMTTop_Solid( new G4Tubs("PMTTop_Solid", 0.,30.,0.25,0.*deg,360*deg) )
    , Reflector_Solid( new G4Tubs("Reflector_Solid", 0.,57.5/2.-0.5,50.8/2.+2.85/2.,0.*deg,360*deg) )
    , Crystal_Solid( new G4Tubs("Crystal_Solid", 0.,50.8/2.,50.8/2.,0.*deg,360*deg) )
    , PlexiWindow_Solid( new G4Tubs("PlexiWindow_Solid", 0.,30.,0.5,0.*deg,360*deg) )
    , PMTWindow_Solid( new G4Tubs("PMTWindow_solid", 0.,30.,1.0,0.*deg,360*deg) )
{

    if ( !fMatLaBr3Crystal ){
        auto *LaBr3 = new G4Material("LaBr3", 5.07*g/cm3, 2);
        LaBr3->AddElement(MakeIfNotFound("Lanthanum", "La", 57., 138.90547*g/mole), 1);
        LaBr3->AddElement(MakeIfNotFound("Bromium", "Br", 35., 79.904*g/mole), 3);
        auto *LaBr3_Ce = new G4Material("LaBr3_Ce", 5.08 * g / cm3, 2);
        LaBr3_Ce->AddMaterial(LaBr3, 95. * perCent);
        LaBr3_Ce->AddElement(MakeIfNotFound("Cerium", "Cl", 58., 140.116*g/mole), 5. * perCent);
        fMatLaBr3Crystal = LaBr3_Ce;
    }

    if ( !fMatLaBr3PMTWindow ){
        fMatLaBr3PMTWindow = new G4Material("Borosilicate glass", 2.23 * g / cm3, 5);
        fMatLaBr3PMTWindow->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 80.6 * perCent);
        fMatLaBr3PMTWindow->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_BORON_OXIDE"), 13.0 * perCent);
        fMatLaBr3PMTWindow->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_SODIUM_MONOXIDE"), 2. * perCent); // 1/2 of wt% for (Na20+K20)
        fMatLaBr3PMTWindow->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_POTASSIUM_OXIDE"), 2. * perCent); // 1/2 of wt% for (Na20+K20)
        fMatLaBr3PMTWindow->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_ALUMINUM_OXIDE"), 2.31 * perCent);
    }
}


G4AssemblyVolume *FTALaBr3::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0.,-54.15/2.);
    G4RotationMatrix rot0;

    auto *CrystalHouseLV = new G4LogicalVolume(CrystalHouse_Solid, fMatLaBr3Housing, "CrystalHouse_Logic");
    auto *PMTHouseLV = new G4LogicalVolume(PMTHouse_Solid, fMatLaBr3Housing, "PMTHouse_Logic");
    auto *PMTTopLV = new G4LogicalVolume(PMTTop_Solid, fMatLaBr3Housing, "PMTTop_Logic");

    auto *housingVIS = new G4VisAttributes(G4Colour::Grey());
    housingVIS->SetForceSolid(true);
    housingVIS->SetVisibility(true);
    CrystalHouseLV->SetVisAttributes(housingVIS);
    PMTHouseLV->SetVisAttributes(housingVIS);
    PMTTopLV->SetVisAttributes(housingVIS);

    assembly->AddPlacedVolume(CrystalHouseLV, pos0, &rot0);
    G4ThreeVector pos1 = pos0+G4ThreeVector(0.,0.,-54.15/2.-(233.-45.)/2.);
    assembly->AddPlacedVolume(PMTHouseLV,pos1, &rot0);
    pos1 = pos0+G4ThreeVector(0.,0.,-54.15/2.-(233.-45.)+0.25);
    assembly->AddPlacedVolume(PMTTopLV,pos1, &rot0);

    auto *ReflectorLV = new G4LogicalVolume(Reflector_Solid, fMatLaBr3Reflector, "Reflector_Logic");
    auto *CrystalLV = new G4LogicalVolume(Crystal_Solid, fMatLaBr3Crystal, "Crystal_Logic");


    // Place crystal inside reflector
    new G4PVPlacement(0, G4ThreeVector(0.,0.,-2.85/2.), CrystalLV,
                      "LaBr3_Crystal_Physical", ReflectorLV, 0, copy_no, checkOverlap);

    // Place reflector inside housing
    new G4PVPlacement(0, G4ThreeVector(0.,0.,-0.25), ReflectorLV,
                      "Reflector_Physical", CrystalHouseLV, 0, copy_no, checkOverlap);

    auto *PlexiWindowLV = new G4LogicalVolume(PlexiWindow_Solid, fMatLaBr3LightGuide, "PlexiWindow_Logic");
    pos1 = pos0+G4ThreeVector(0.,0.,-0.5-54.15/2.);
    assembly->AddPlacedVolume(PlexiWindowLV, pos1, &rot0);

    auto *PMTWindowLV = new G4LogicalVolume(PMTWindow_Solid, fMatLaBr3PMTWindow, "PMTWindow_Logic");
    pos1 = pos0+G4ThreeVector(0.,0.,-0.5-54.15/2.0-0.5-1.);
    assembly->AddPlacedVolume(PMTWindowLV,pos1, &rot0);

    return assembly;
}