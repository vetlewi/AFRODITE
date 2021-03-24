#include "FTALaBr3.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <G4Polycone.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>

using namespace CLHEP;

constexpr G4double startPhi_LaBr3_Housing = 0.0*deg;
constexpr G4double endPhi_LaBr3_Housing = 360.0*deg;
constexpr G4int nrRZ_LaBr3_Housing = 6;
constexpr G4double zPlane_LaBr3_Housing[]={0, 1.53*25.4*mm, 2.29*25.4*mm, 4.11*25.4*mm, 5.21*25.4*mm, 8.82*25.4*mm};
constexpr G4double rInner_LaBr3_Housing[]={0, 0, 0, 0, 0, 0};
constexpr G4double rOuter_LaBr3_Housing[]={2.2*25.4/2*mm, 2.2*25.4/2*mm, 3.15*25.4/2*mm, 3.15*25.4/2*mm, 2.31*25.4/2*mm, 2.31*25.4/2*mm};

constexpr G4double startPhi_LaBr3_Interior = 0.0*deg;
constexpr G4double endPhi_LaBr3_Interior = 360.0*deg;
constexpr G4int nrRZ_LaBr3_Interior = 6;
constexpr G4double zPlane_LaBr3_Interior[]={0.5*mm, 1.53*25.4*mm, 2.29*25.4*mm, 4.11*25.4*mm, 5.21*25.4*mm, 8.82*25.4*mm - 0.5*mm};
constexpr G4double rInner_LaBr3_Interior[]={0, 0, 0, 0, 0, 0};
constexpr G4double rOuter_LaBr3_Interior[]={2.2*25.4/2*mm - 0.5*mm, 2.2*25.4/2*mm - 0.5*mm, 3.15*25.4/2*mm - 0.5*mm, 3.15*25.4/2*mm - 0.5*mm, 2.31*25.4/2*mm - 0.5*mm, 2.31*25.4/2*mm - 0.5*mm};

constexpr  G4double startPhi_LaBr3_PMT = 0.0*deg;
constexpr G4double endPhi_LaBr3_PMT = 360.0*deg;
constexpr G4int nrRZ_LaBr3_PMT = 4;
constexpr G4double zPlane_LaBr3_PMT[]={2.29*25.4*mm, 4.11*25.4*mm, 5.21*25.4*mm, 6.06*25.4*mm - 0.5*mm};
constexpr G4double rInner_LaBr3_PMT[]={0, 0, 0, 0};
constexpr G4double rOuter_LaBr3_PMT[]={3.0*25.4/2*mm - 0.5*mm, 3.0*25.4/2*mm - 0.5*mm, 2.31*25.4/2*mm - 0.5*mm, 2.31*25.4/2*mm - 0.5*mm};


constexpr G4double startPhi_LaBr3_PMTInterior = 0.0*deg;
constexpr G4double endPhi_LaBr3_PMTInterior = 360.0*deg;
constexpr G4int nrRZ_LaBr3_PMTInterior = 4;
constexpr G4double zPlane_LaBr3_PMTInterior[]={2.29*25.4*mm + 1*mm, 4.11*25.4*mm, 5.21*25.4*mm, 6.06*25.4*mm - 0.5*mm - 1*mm};
constexpr G4double rInner_LaBr3_PMTInterior[]={0, 0, 0, 0};
constexpr G4double rOuter_LaBr3_PMTInterior[]={3.0*25.4/2*mm - 0.5*mm - 1*mm, 3.0*25.4/2*mm - 0.5*mm - 1*mm, 2.31*25.4/2*mm - 0.5*mm - 1*mm, 2.31*25.4/2*mm - 0.5*mm - 1*mm};

extern G4Element *MakeIfNotFound(const G4String &name, const G4String &symbol, const G4double &Zeff, const G4double &Aeff);


FTALaBr3::FTALaBr3(const G4int &dID, const bool &checkOverlaps)
    : detectorID( dID )
{
    G4NistManager *nistManager = G4NistManager::Instance();
    fMatLaBr3Housing = nistManager->FindOrBuildMaterial("G4_Al");
    fMatLaBr3Interior = nistManager->FindOrBuildMaterial("G4_AIR");
    fMatLaBr3Reflector = nistManager->FindOrBuildMaterial("G4_TEFLON");
    fMatLaBr3LightGuide = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
    fMatLaBr3PMT = nistManager->FindOrBuildMaterial("G4_Pyrex_Glass");
    fMatLaBr3PMTInterior = nistManager->FindOrBuildMaterial("G4_Galactic");

    // vacuum
    vacuum = G4Material::GetMaterial("Vacuum");
    if ( !vacuum ) {
        vacuum = new G4Material("Vacuum", 1,1.008 * g / mole,1.e-25 * g / cm3,
                                kStateGas,2.73 * kelvin, 1.e-25 * g / cm3);
    }

    G4double a, z;                    //a=mass of a mole;
    G4double density;                 //z=mean number of protons;
    G4int ncomponents, natoms;
    G4double abundance, fractionmass;

    La = MakeIfNotFound("Lanthanum", "La", z=57.,  a=138.90547*g/mole);
    Br = MakeIfNotFound("Bromium",    "Br",   z=35.,  a=79.904*g/mole);
    Ce = MakeIfNotFound("Cerium",     "Cl",   z=58.,  a=140.116*g/mole);

    //LaBr3
    LaBr3 = G4Material::GetMaterial("LaBr3", false);
    if ( !LaBr3 ){
        LaBr3 =   new G4Material("LaBr3", density = 5.07*g/cm3, ncomponents=2);
        LaBr3->AddElement(La, natoms=1);
        LaBr3->AddElement(Br, natoms=3);
    }

    //LaBr3_Ce
    //with 5% dopping, see technical note "BrilLanCe Scintillators Performance Summary"
    LaBr3_Ce = G4Material::GetMaterial("LaBr3", false);
    if ( !LaBr3_Ce ){
        LaBr3_Ce = new G4Material("LaBr3_Ce", density = 5.08 * g / cm3, ncomponents = 2);
        LaBr3_Ce->AddMaterial(LaBr3, fractionmass = 95. * perCent);
        LaBr3_Ce->AddElement(Ce, fractionmass = 5. * perCent);
    }

    CreateSolids(checkOverlaps);
}

void FTALaBr3::CreateSolids(const bool &checkOverlaps)
{
    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector Housing
    //
    // Material: Aluminium
    LaBr3_Housing_Solid = new G4Polycone("LaBr3_Housing_Solid",
                                         startPhi_LaBr3_Housing,
                                         endPhi_LaBr3_Housing,
                                         nrRZ_LaBr3_Housing,
                                         zPlane_LaBr3_Housing,
                                         rInner_LaBr3_Housing,
                                         rOuter_LaBr3_Housing);

    LaBr3_Housing_Logical = new G4LogicalVolume(LaBr3_Housing_Solid, fMatLaBr3Housing, "LaBr3_Housing_Logical");

    auto* Vis_LaBr3_Housing = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.4));
    Vis_LaBr3_Housing->SetForceWireframe(true);
    LaBr3_Housing_Logical->SetVisAttributes(Vis_LaBr3_Housing);

    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector Interior
    //
    // Material: Air

    G4VSolid* LaBr3_Interior_Solid = new G4Polycone("LaBr3_Interior_Solid",
                                                    startPhi_LaBr3_Interior,
                                                    endPhi_LaBr3_Interior,
                                                    nrRZ_LaBr3_Interior,
                                                    zPlane_LaBr3_Interior,
                                                    rInner_LaBr3_Interior,
                                                    rOuter_LaBr3_Interior);

    LaBr3_Interior_Logical = new G4LogicalVolume(LaBr3_Interior_Solid, fMatLaBr3Interior, "LaBr3_Interior_Logical");
    auto* Vis_LaBr3_Interior = new G4VisAttributes(G4Colour(0.,1.0,0.,0.1));
    Vis_LaBr3_Interior->SetForceWireframe(false);
    LaBr3_Interior_Logical->SetVisAttributes(Vis_LaBr3_Interior);

    LaBr3_Interior_Physical = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), LaBr3_Interior_Logical, "LaBr3_Interior_Physical",
                                                LaBr3_Housing_Logical, false, detectorID, checkOverlaps);


    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector Reflector
    //
    // Material: Teflon
    // The LaBr3(Ce) crystal is wrapped in a material acting as a Lambertian reflector surface
    // which is designed to increase light collection efficiency

    G4VSolid* LaBr3_Reflector_Solid = new G4Tubs("LaBr3_Reflector_Solid",
                                                 0.,
                                                 50.8*mm/2 + 1.5*mm,
                                                 (50.8*mm + 1.5*mm + 5.0*mm)/2,
                                                 0.,
                                                 360.*deg);

    LaBr3_Reflector_Logical = new G4LogicalVolume(LaBr3_Reflector_Solid, fMatLaBr3Reflector, "LaBr3_Reflector_Logical");
    auto* Vis_LaBr3_Reflector = new G4VisAttributes(G4Colour(1.,1.,1.,0.5));
    Vis_LaBr3_Reflector->SetForceWireframe(false);
    LaBr3_Reflector_Logical->SetVisAttributes(Vis_LaBr3_Reflector);

    LaBr3_Interior_Physical = new G4PVPlacement(0,
                                                G4ThreeVector(0, 0, 0.5*mm + (50.8*mm + 1.5*mm + 5.0*mm)/2),
                                                LaBr3_Reflector_Logical, "LaBr3_Interior_Physical",
                                                LaBr3_Interior_Logical, false, detectorID, checkOverlaps);

    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector Crystal
    //
    // Material: LaBr3(Ce)

    G4VSolid* LaBr3_Crystal_Solid = new G4Tubs("LaBr3_Crystal_Solid",
                                               0.,
                                               50.8*mm/2,
                                               50.8*mm/2,
                                               0.,
                                               360.*deg);

    /*std::string crystal_name = "LaBr3_Crystal_Logical_" + std::to_string(detectorID);*/
    LaBr3_Crystal_Logical = new G4LogicalVolume(LaBr3_Crystal_Solid, LaBr3_Ce, "LaBr3_Crystal_Logical");
    auto* Vis_LaBr3_Crystal = new G4VisAttributes(G4Colour(0.,0.,1.,0.5));
    Vis_LaBr3_Crystal->SetForceWireframe(false);
    LaBr3_Crystal_Logical->SetVisAttributes(Vis_LaBr3_Crystal);

    LaBr3_Crystal_Physical = new G4PVPlacement(0,
                                               G4ThreeVector(0, 0, -(5.0*mm-1.5*mm)/2),
                                                LaBr3_Crystal_Logical, "LaBr3_Crystal_Physical",
                                                LaBr3_Reflector_Logical, false, detectorID, checkOverlaps);

    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector Light Guide
    //
    // Material: Glass

    G4VSolid* LaBr3_LightGuide_Solid = new G4Tubs("LaBr3_LightGuide_Solid",
                                                  0.,
                                                  50.8*mm/2,
                                                  5.0*mm/2,
                                                  0.,
                                                  360.*deg);

    LaBr3_LightGuide_Logical = new G4LogicalVolume(LaBr3_LightGuide_Solid, fMatLaBr3LightGuide, "LaBr3_LightGuide_Logical");
    auto* Vis_LaBr3_LightGuide = new G4VisAttributes(G4Colour(0.,1.0,1.0,0.4));
    Vis_LaBr3_LightGuide->SetForceWireframe(false);
    LaBr3_LightGuide_Logical->SetVisAttributes(Vis_LaBr3_LightGuide);

    LaBr3_LightGuide_Physical = new G4PVPlacement(0,
                                                  G4ThreeVector(0, 0, +(50.8*mm+1.5*mm)/2),
                                                  LaBr3_LightGuide_Logical, "LaBr3_LightGuide_Physical",
                                                  LaBr3_Reflector_Logical, false, detectorID, checkOverlaps);

    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector PMT
    //
    // Material: Glass (0.8 mm)

    G4VSolid* LaBr3_PMT_Solid = new G4Polycone("LaBr3_PMT_Solid",
                                               startPhi_LaBr3_PMT,
                                               endPhi_LaBr3_PMT,
                                               nrRZ_LaBr3_PMT,
                                               zPlane_LaBr3_PMT,
                                               rInner_LaBr3_PMT,
                                               rOuter_LaBr3_PMT);

    LaBr3_PMT_Logical = new G4LogicalVolume(LaBr3_PMT_Solid, fMatLaBr3PMT, "LaBr3_PMT_Logical");
    auto* Vis_LaBr3_PMT = new G4VisAttributes(G4Colour(1.,0.,1.,0.5));
    Vis_LaBr3_PMT->SetForceWireframe(false);
    LaBr3_PMT_Logical->SetVisAttributes(Vis_LaBr3_PMT);

    LaBr3_PMT_Physical = new G4PVPlacement(0,
                                           G4ThreeVector(0, 0, 0),
                                           LaBr3_PMT_Logical, "LaBr3_PMT_Physical",
                                           LaBr3_Interior_Logical, false, detectorID, checkOverlaps);

    ////////////////////////////////////////////////////////////////////////
    // LaBr3 - Detector PMT Interior
    //
    // Material: Vacuum

    G4VSolid* LaBr3_PMTInterior_Solid = new G4Polycone("LaBr3_PMTInterior_Solid",
                                                       startPhi_LaBr3_PMTInterior,
                                                       endPhi_LaBr3_PMTInterior,
                                                       nrRZ_LaBr3_PMTInterior,
                                                       zPlane_LaBr3_PMTInterior,
                                                       rInner_LaBr3_PMTInterior,
                                                       rOuter_LaBr3_PMTInterior);

    LaBr3_PMTInterior_Logical = new G4LogicalVolume(LaBr3_PMTInterior_Solid, fMatLaBr3PMTInterior, "LaBr3_PMTInterior_Logical");
    auto* Vis_LaBr3_PMTInterior = new G4VisAttributes(G4Colour(1.,1.,1.,0.5));
    Vis_LaBr3_PMTInterior->SetForceWireframe(false);
    LaBr3_PMTInterior_Logical->SetVisAttributes(Vis_LaBr3_PMTInterior);

    LaBr3_PMTInterior_Physical = new G4PVPlacement(0,
                                                   G4ThreeVector(0, 0, 0),
                                                   LaBr3_PMTInterior_Logical, "LaBr3_PMTInterior_Physical",
                                                   LaBr3_PMT_Logical, false, detectorID, checkOverlaps);

}

void FTALaBr3::SetPosition(G4ThreeVector thisPos) {
    translatePos = thisPos*mm;
    G4cout << " ----> A OCLLaBr3 will be placed at distance: " << translatePos/mm << " mm" << G4endl;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void FTALaBr3::SetRotation(G4RotationMatrix thisRot) {
    rotation = thisRot;
}

void FTALaBr3::Placement(G4int copyNo, G4VPhysicalVolume *physiMother, G4bool checkOverlaps)
{
    G4Transform3D transDetector = G4Transform3D(rotation,translatePos);
    G4int copyNoSub = copyNo; // copy number for the sub elements (could also be copyNo)

    auto posHousing = G4ThreeVector(0,-1.5*25.4*mm,-2.5*25.4*mm);

    new G4PVPlacement(transDetector,
                      "LaBr3_Housing_Physical",	// Name
                      LaBr3_Housing_Logical,			// Logical volume
                      physiMother,						// Mother volume
                      false,						// Unused boolean parameter
                      copyNo,							// Copy number
                      checkOverlaps);					// Overlap Check
}