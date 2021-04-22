#include "Detectors/FTALaBr3.hh"

#include <G4SystemOfUnits.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Tubs.hh>
#include <G4AssemblyVolume.hh>
#include <G4VisAttributes.hh>
#include <G4SubtractionSolid.hh>

constexpr G4double thMin = 0.*deg;
constexpr G4double thMax = 360.*deg;

constexpr G4double HousingHalfHeight = 233.*mm/2.;
constexpr G4double HousingRadius = 62.*mm/2.;

constexpr G4double CrystalHousingRadius = 57.5*mm/2.;
constexpr G4double CrystalHousingHalfThickness = 0.5*mm/2.;
constexpr G4double CrystalHousingHalfHeight = 49.*mm/2.;

constexpr G4double CrystalRadius = 50.8*mm/2.;
constexpr G4double CrystalHalfHeight = 50.8*mm/2.;

constexpr G4double ReflectorThickness = CrystalHousingRadius - 2*CrystalHousingHalfThickness - CrystalRadius;
constexpr G4double ReflectorHalfThickness = ReflectorThickness/2.;
constexpr G4double ReflectorRadius = CrystalRadius + ReflectorThickness;
constexpr G4double ReflectorHalfHeight = CrystalHalfHeight + ReflectorHalfThickness;

constexpr G4double PlexiWindowRadius = ReflectorRadius;
constexpr G4double PlexiWindowHalfHeight = 1.0*mm/2.;

constexpr G4double PMTHouseThickness = 0.5*mm/2.; // Assumed to be 0.5 mm thick.
constexpr G4double PMTHouseOuterRadius = HousingRadius;
constexpr G4double PMTHouseInnerRadius = PMTHouseOuterRadius - PMTHouseThickness;
constexpr G4double PMTHouseHalfHeight = HousingHalfHeight - CrystalHousingHalfHeight;

constexpr G4double PMTWindowThickness = 1.0*mm; // Assumed to be 1 mm thick walls.
constexpr G4double PMTHalfHeight = 121.*mm/2.;
constexpr G4double PMTRadius = 52.0*mm/2.;
constexpr G4double PMTVacuumRadius = PMTRadius - PMTWindowThickness;
constexpr G4double PMTVacuumHalfHeight = PMTHalfHeight - PMTWindowThickness;

constexpr G4double PMTCathodeRadius = 46.*mm/2;
constexpr G4double PMTCathodeHalfHeight = 0.005*mm;




extern G4Element *MakeIfNotFound(const G4String &name, const G4String &symbol, const G4double &Zeff, const G4double &Aeff);


FTALaBr3::FTALaBr3()
    : Detector::DetectorFactory( Detector::Type::fta_labr )
    , fMatLaBr3Vacuum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
    , fMatLaBr3Housing( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
    , fMatLaBr3Reflector( G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON") )
    , fMatLaBr3Crystal( G4NistManager::Instance()->FindOrBuildMaterial("Labr3_Ce") )
    , fMatLaBr3LightGuide( G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS") )
    , fMatLaBr3PMTWindow( G4NistManager::Instance()->FindOrBuildMaterial("Borosilicate glass") )
    , fMatLaBr3PMTCathode( G4NistManager::Instance()->FindOrBuildMaterial("Bialkali") )

    , Housing_Solid( new G4SubtractionSolid("Housing_Solid",
                                            new G4Tubs("Full_Housing", 0., HousingRadius, HousingHalfHeight, thMin, thMax),
                                            new G4Tubs("Crystal_Part", CrystalHousingRadius, HousingRadius, CrystalHousingHalfHeight+0.01, thMin, thMax),
                                            0, G4ThreeVector(0, 0, CrystalHousingHalfHeight-HousingHalfHeight-0.02)) )
    , CrystalHouse_Solid( new G4SubtractionSolid("CrystalHouse_Solid",
                                                 new G4Tubs("Massive", 0, CrystalHousingRadius, CrystalHousingHalfHeight, thMin, thMax),
                                                 new G4Tubs("void", 0, CrystalHousingRadius-CrystalHousingHalfThickness, CrystalHousingHalfHeight, thMax, thMax),
                                                 0, G4ThreeVector(0, 0, 2*CrystalHousingHalfThickness)) )
    , Reflector_Solid( new G4Tubs("Reflector_Solid", 0., ReflectorRadius, ReflectorHalfHeight, thMin, thMax) )
    , Crystal_Solid( new G4Tubs("Crystal_Solid", 0, CrystalRadius, CrystalHalfHeight, thMin, thMax) )
    , PlexiWindow_Solid( new G4Tubs("PlexiWindow_Solid", 0., PlexiWindowRadius, PlexiWindowHalfHeight, thMin, thMax) )

    , PMTHouse_Solid( new G4SubtractionSolid("PMTHouse_Solid",
                                             new G4SubtractionSolid("Cut0",
                                                                    new G4Tubs("PMTHouseMass", 0, PMTHouseOuterRadius, PMTHouseHalfHeight, thMin, thMax),
                                                                    new G4Tubs("MPTHouseVoid", 0, PMTHouseInnerRadius, PMTHouseHalfHeight-PMTHouseThickness/2., thMin, thMax)),
                                             new G4Tubs("SmallCut", 0, CrystalHousingRadius, PMTHouseHalfHeight, thMin, thMax),
                                             0, G4ThreeVector(0, 0, -PMTHouseHalfHeight)) )
    , PMT_Solid( new G4Tubs("PMT_Solid", 0., PMTRadius, PMTHalfHeight, thMin, thMax) )
    , PMT_Vaccum_Solid( new G4Tubs("PMT_Vaccum_Solid", 0, PMTVacuumRadius, PMTVacuumHalfHeight, thMin, thMax) )
    , PMTCathode_Solid( new G4Tubs("PMTCathode_Solid", 0., PMTCathodeRadius, PMTCathodeHalfHeight, thMin, thMax) )
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

    if ( !fMatLaBr3PMTCathode ){
        fMatLaBr3PMTCathode = new G4Material("Bialkali", 2. * g / cm3, 3);
        fMatLaBr3PMTCathode->AddElement(G4NistManager::Instance()->FindOrBuildElement("K"), 2);
        fMatLaBr3PMTCathode->AddElement(G4NistManager::Instance()->FindOrBuildElement("Cs"), 1);
        fMatLaBr3PMTCathode->AddElement(G4NistManager::Instance()->FindOrBuildElement("Sb"), 1);
    }

}


G4AssemblyVolume *FTALaBr3::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0.,HousingHalfHeight);
    G4RotationMatrix rot0;

    auto *housing_vaccum_visAtt = new G4VisAttributes(G4Colour::Green());
    housing_vaccum_visAtt->SetVisibility(false);
    auto *HousingLV = new G4LogicalVolume(Housing_Solid, fMatLaBr3Vacuum, "Housing_Logic");
    HousingLV->SetVisAttributes(housing_vaccum_visAtt);
    assembly->AddPlacedVolume(HousingLV, pos0, &rot0);

    // Lets start by assembling the crystal inside the housing, etc.
    auto *housing_VisAtt = new G4VisAttributes(G4Colour::Gray());
    housing_VisAtt->SetForceSolid(true);
    auto *CrystalHouseLV = new G4LogicalVolume(CrystalHouse_Solid, fMatLaBr3Housing, "CrystalHouse_Logic");
    auto *PMTHouseLV = new G4LogicalVolume(PMTHouse_Solid, fMatLaBr3Housing, "PMTHouse_Logic");
    CrystalHouseLV->SetVisAttributes(housing_VisAtt);
    PMTHouseLV->SetVisAttributes(housing_VisAtt);

    auto pos_crystal_housing = G4ThreeVector(0, 0, CrystalHousingHalfHeight-HousingHalfHeight);
    new G4PVPlacement(0, pos_crystal_housing,
                      CrystalHouseLV,
                      "CrystalHousing_Physical",
                      HousingLV,
                      false,
                      copy_no,
                      checkOverlap);

    auto pos_PMT_house = pos_crystal_housing + G4ThreeVector(0., 0., CrystalHousingHalfHeight+PMTHouseHalfHeight);
    new G4PVPlacement(0, pos_PMT_house,
                      PMTHouseLV,
                      "PMTHouse_Physical",
                      HousingLV,
                      false,
                      copy_no,
                      checkOverlap);

    auto *Reflector_VisAtt = new G4VisAttributes(G4Colour::Blue());
    Reflector_VisAtt->SetVisibility(false);
    auto *ReflectorLV = new G4LogicalVolume(Reflector_Solid, fMatLaBr3Reflector, "Reflector_Logic");
    ReflectorLV->SetVisAttributes(Reflector_VisAtt);

    auto *Crystal_VisAtt = new G4VisAttributes(G4Colour::Red());
    Crystal_VisAtt->SetVisibility(false);
    auto *CrystalLV = new G4LogicalVolume(Crystal_Solid, fMatLaBr3Crystal, "Crystal_Logic");
    CrystalLV->SetVisAttributes(Crystal_VisAtt);

    // Place crystal inside reflector
    new G4PVPlacement(0, G4ThreeVector(0., 0., ReflectorHalfThickness),
                      CrystalLV,
                      "LaBr3_Crystal_Physical",
                      ReflectorLV,
                      false,
                      copy_no,
                      checkOverlap);

    // Place Reflector inside the housing
    auto pos_reflector = pos_crystal_housing+G4ThreeVector(0., 0., ReflectorThickness);
    new G4PVPlacement(0, pos_reflector,
                      ReflectorLV,
                      "Reflector_Physical",
                      HousingLV,
                      false,
                      copy_no,
                      checkOverlap);

    // Next we will create crystal window and gently place it on top of the crystal.
    auto *LightGuide_VisAtt = new G4VisAttributes(G4Colour::Green());
    LightGuide_VisAtt->SetVisibility(false);
    auto *LightGuideLV = new G4LogicalVolume(PlexiWindow_Solid, fMatLaBr3LightGuide, "LightGuide_Logic");
    LightGuideLV->SetVisAttributes(LightGuide_VisAtt);

    // Place The light guide
    auto pos_lightguide = pos_reflector + G4ThreeVector(0., 0., ReflectorHalfHeight + PlexiWindowHalfHeight);
    new G4PVPlacement(0, pos_lightguide,
                      LightGuideLV,
                      "LightGuide_Physical",
                      HousingLV,
                      false,
                      copy_no,
                      checkOverlap);

    auto *PMT_VisAtt = new G4VisAttributes(G4Colour::Yellow());
    PMT_VisAtt->SetVisibility(false);
    auto *pmtLV = new G4LogicalVolume(PMT_Solid, fMatLaBr3PMTWindow, "PMT_Logic");
    pmtLV->SetVisAttributes(PMT_VisAtt);


    auto *PMT_VacuumLV = new G4LogicalVolume(PMT_Vaccum_Solid, fMatLaBr3Vacuum, "PMT_Vaccum_Logic");
    PMT_VacuumLV->SetVisAttributes(housing_vaccum_visAtt);

    auto *PMT_Cathode_VisAtt = new G4VisAttributes(G4Colour::Brown());
    PMT_Cathode_VisAtt->SetVisibility(false);
    auto *PMT_CathodeLV = new G4LogicalVolume(PMTCathode_Solid, fMatLaBr3PMTCathode, "PMT_Cathode_Logic");
    PMT_CathodeLV->SetVisAttributes(PMT_Cathode_VisAtt);

    new G4PVPlacement(0, G4ThreeVector(0., 0., PMTCathodeHalfHeight - PMTVacuumHalfHeight),
                      PMT_CathodeLV,
                      "PMT_Cathode_Physical",
                      PMT_VacuumLV,
                      false,
                      copy_no,
                      checkOverlap);

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
                      PMT_VacuumLV,
                      "PMT_Vacuum_Logic",
                      pmtLV,
                      false,
                      copy_no,
                      checkOverlap);

    auto pos_PMT = pos_lightguide + G4ThreeVector(0., 0., PlexiWindowHalfHeight + PMTHalfHeight);
    new G4PVPlacement(0, pos_PMT,
                      pmtLV,
                      "PMT_Physical",
                      HousingLV,
                      false,
                      copy_no,
                      checkOverlap);

    return assembly;
}