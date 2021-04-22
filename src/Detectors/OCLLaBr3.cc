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


#include "Detectors/OCLLaBr3.hh"
#include "SingleScintParameters.hh"

#include <G4NistManager.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Polycone.hh>
#include <G4MultiUnion.hh>
#include <G4AssemblyVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace OCL;

G4Element *MakeIfNotFound(const G4String &name, const G4String &symbol, const G4double &Zeff, const G4double &Aeff)
{
    G4Element *element = G4Element::GetElement(symbol, false);
    if ( element )
        return element;
    return new G4Element(name, symbol, Zeff, Aeff);
}

G4VSolid *GetShielding_Solid(const bool &tapered)
{
    if ( tapered )
        return new G4Cons("Shielding_Solid", shieldingInnerR, shieldingConeOuterRFront, shieldingInnerR, shieldingConeOuterRBack, shieldingConeHalfLength, startPhi, deltaPhi);
    else
        return new G4Tubs("Shielding_Solid", shieldingInnerR, shieldingConeOuterRBack, shieldingConeHalfLength, startPhi, deltaPhi);
}


OCLLaBr3::OCLLaBr3(const bool &new_shield_design)
    : fMatAir( G4NistManager::Instance()->FindOrBuildMaterial("Air") )
    , fMatAluminium( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
    , fMatPlexiGlass( G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS") )
    , fMatMgO( G4NistManager::Instance()->FindOrBuildMaterial("MgO") )
    , fMatLaBr3_Ce( G4NistManager::Instance()->FindOrBuildMaterial("LaBr3_Ce") )
    , fMatSiO2( G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE") )
    , fMatBialkali( G4NistManager::Instance()->FindOrBuildMaterial("Bialkali") )
    , Detector_Solid( new G4Tubs("Detector_Solid", 0.*mm, coatingOuterR, PMTandAirHalfLength, startPhi, deltaPhi) )
    , Shielding_Solid( GetShielding_Solid(new_shield_design) )
    , Shields_Tube_Solids{ new G4Tubs("Shielding_Tube_Solid_0", shieldingInnerR, ShieldrOuters[0], dShieldTubs[0]/2, startPhi, deltaPhi),
                           new G4Tubs("Shielding_Tube_Solid_1", shieldingInnerR, ShieldrOuters[1], dShieldTubs[1]/2, startPhi, deltaPhi),
                           new G4Tubs("Shielding_Tube_Solid_2", shieldingInnerR, ShieldrOuters[2], dShieldTubs[2]/2, startPhi, deltaPhi)}
    , Union_Shielding_Solid( new G4MultiUnion("Shield_Union_Solid") )
    , Coating_Solid( new G4Tubs("Coating_Solid", 0.*mm, coatingOuterR, coatingHalfLength, startPhi, deltaPhi) )
    , PlexiGlas_Coating_Solid( new G4Tubs("PlexiGlas_Coating_Solid", 0.*mm, coatingOuterR-coatingAlThickness, coatingHalfLength - 0.5 * coatingAlThicknessFront, startPhi, deltaPhi) )
    , Reflector_Solid( new G4Tubs("Reflector_Solid", reflectorInnerR, reflectorOuterR, reflectorHalfLength, startPhi, deltaPhi) )
    , Crystal_Solid( new G4Tubs("Crystal_Solid", crystalInnerR, crystalOuterR, crystalHalfLength, startPhi, deltaPhi) )
    , Crystal_Window_Solid( new G4Tubs("Crystal_Window_Solid", 0. * mm, plexiGlasWindowOuterR, plexiGlasWindowHalfLength, startPhi, deltaPhi) )
    , PMT_Window_Solid( new G4Tubs("PMT_Window_Solid", 0.*cm, PMTWindowRadius, PMTWindowHalfLength, startPhi, deltaPhi) )
    , Cathode_Solid( new G4Tubs("Cathode_Solid", 0.*cm, cathodeRadius, cathodeHalfLength, startPhi, deltaPhi) )
    , PMT_Body_Solid( new G4Polycone("PMT_Body_Solid", startPhi, deltaPhi, 3, zPlane, rInners, rOuters) )
{

    G4NistManager *nist = G4NistManager::Instance();

    {
        G4RotationMatrix rotm = G4RotationMatrix();
        G4ThreeVector positionC = G4ThreeVector(0., 0., shieldingConeHalfLength - detectorHalfinclPMT);
        G4ThreeVector position0 = positionC + G4ThreeVector(0., 0., dShieldTubs[0] / 2. + shieldingConeHalfLength);
        G4ThreeVector position1 = position0 + G4ThreeVector(0., 0., dShieldTubs[1] / 2. + dShieldTubs[0] / 2.);
        G4ThreeVector position2 = position1 + G4ThreeVector(0., 0., dShieldTubs[2] / 2. + dShieldTubs[1] / 2.);
        G4Transform3D trC = G4Transform3D(rotm, positionC);
        G4Transform3D tr0 = G4Transform3D(rotm, position0);
        G4Transform3D tr1 = G4Transform3D(rotm, position1);
        G4Transform3D tr2 = G4Transform3D(rotm, position2);
        Union_Shielding_Solid->AddNode(*Shielding_Solid, trC);
        Union_Shielding_Solid->AddNode(*Shields_Tube_Solids[0], tr0);
        Union_Shielding_Solid->AddNode(*Shields_Tube_Solids[1], tr1);
        Union_Shielding_Solid->AddNode(*Shields_Tube_Solids[2], tr2);
        Union_Shielding_Solid->Voxelize();
    }

    if (!fMatAir) {

        auto *Nitrogen = new G4Material("N2", 1.25053 * mg / cm3, 1);
        Nitrogen->AddElement(nist->FindOrBuildElement("N"), 2);

        auto *Oxygen = new G4Material("O2", 1.4289 * mg / cm3, 1);
        Oxygen->AddElement(nist->FindOrBuildElement("O"), 2);

        auto *Argon = new G4Material("Argon", 1.7836 * mg / cm3, 1);
        Argon->AddElement(nist->FindOrBuildElement("Ar"), 1);

        fMatAir = new G4Material("Air", 1.2928 * mg / cm3, 3);
        fMatAir->AddMaterial(Nitrogen, 0.7557);
        fMatAir->AddMaterial(Oxygen, 0.2315);
        fMatAir->AddMaterial(Argon, 0.0128);
    }

    if ( !fMatMgO ) {
        fMatMgO = new G4Material("MgO", 3.6*g/cm3, 2);
        fMatMgO->AddElement(nist->FindOrBuildElement("Mg"), 1);
        fMatMgO->AddElement(nist->FindOrBuildElement("O"), 1);
    }

    if (!fMatLaBr3_Ce) {
        auto *LaBr3 = new G4Material("LaBr3", 5.07 * g / cm3, 2);
        LaBr3->AddElement(MakeIfNotFound("Lanthanum", "La", 57., 138.90547 * g / mole), 1);
        LaBr3->AddElement(MakeIfNotFound("Bromium", "Br", 35., 79.904 * g / mole), 3);
        fMatLaBr3_Ce = new G4Material("LaBr3_Ce", 5.08 * g / cm3, 2);
        fMatLaBr3_Ce->AddMaterial(LaBr3, 95. * perCent);
        fMatLaBr3_Ce->AddElement(MakeIfNotFound("Cerium", "Cl", 58., 140.116 * g / mole), 5. * perCent);
    }

    if (!fMatBialkali) {
        fMatBialkali = new G4Material("Bialkali", 2. * g / cm3, 3);
        fMatBialkali->AddElement(nist->FindOrBuildElement("K"), 2);
        fMatBialkali->AddElement(nist->FindOrBuildElement("Cs"), 1);
        fMatBialkali->AddElement(nist->FindOrBuildElement("Sb"), 1);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4AssemblyVolume * OCLLaBr3::GetAssembly(const int &copy_no, const bool &checkOverlap)
{

    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., detectorHalfinclPMT);
    G4RotationMatrix rot0;

    // Setup attributes
    auto *hiddenVIS = new G4VisAttributes(G4Colour::Black());
    auto *shieldVIS = new G4VisAttributes(G4Colour::Gray());
    auto *crystalVIS = new G4VisAttributes(G4Colour::White());
    auto *coatingVIS = new G4VisAttributes(G4Color::Red());
    auto *reflectorVIS = new G4VisAttributes(G4Colour::Yellow());
    auto *plexiCoatingVIS = new G4VisAttributes(G4Colour::Brown());
    auto *PMT_Vol_VIS = new G4VisAttributes(G4Colour::Red());
    auto *PMT_WindowVIS = new G4VisAttributes(G4Colour::Green());
    auto *PMT_VIS = new G4VisAttributes(G4Colour::Magenta());



    shieldVIS->SetForceSolid(true);

    hiddenVIS->SetVisibility(false);
    shieldVIS->SetVisibility(true);
    crystalVIS->SetVisibility(false);
    coatingVIS->SetVisibility(false);
    reflectorVIS->SetVisibility(false);
    plexiCoatingVIS->SetVisibility(false);
    PMT_Vol_VIS->SetVisibility(false);
    PMT_WindowVIS->SetVisibility(false);
    PMT_VIS->SetVisibility(false);



    // Creating all the logical volumes
    auto *Detector_Logic = new G4LogicalVolume(Detector_Solid, fMatAir, "Detector_Logic");
    auto *Shielding_Logic = new G4LogicalVolume(Union_Shielding_Solid, fMatAluminium, "Shielding_Logic");
    assembly->AddPlacedVolume(Shielding_Logic, pos0, &rot0);

    // Set Shielding visuals
    Detector_Logic->SetVisAttributes(PMT_Vol_VIS);
    Shielding_Logic->SetVisAttributes(shieldVIS);


    auto *Coating_Logic = new G4LogicalVolume(Coating_Solid, fMatAluminium, "Coating_Logic");
    auto *PlexiGlas_Coating_Logic = new G4LogicalVolume(PlexiGlas_Coating_Solid, fMatPlexiGlass, "PlexiGlas_Coating_Logic");
    auto *Reflector_Logic = new G4LogicalVolume(Reflector_Solid, fMatMgO, "Reflector_Logic");
    auto *Crystal_Logic = new G4LogicalVolume(Crystal_Solid, fMatLaBr3_Ce, "Crystal_Logic");
    auto *Crystal_Window_Logic = new G4LogicalVolume(Crystal_Window_Solid, fMatPlexiGlass, "Crystal_Window_Logic");

    Coating_Logic->SetVisAttributes(coatingVIS);
    PlexiGlas_Coating_Logic->SetVisAttributes(plexiCoatingVIS);
    Reflector_Logic->SetVisAttributes(reflectorVIS);
    Crystal_Logic->SetVisAttributes(crystalVIS);
    Crystal_Window_Logic->SetVisAttributes(plexiCoatingVIS);


    auto *PMT_Window_Logic = new G4LogicalVolume(PMT_Window_Solid, fMatSiO2, "PMT_Window_Logic");
    auto *Cathode_Logic = new G4LogicalVolume(Cathode_Solid, fMatBialkali, "Cathode_Logic");
    auto *PMT_Body_Logic = new G4LogicalVolume(PMT_Body_Solid, fMatAluminium, "PMT_Body_Logic");

    PMT_Window_Logic->SetVisAttributes(PMT_WindowVIS);
    Cathode_Logic->SetVisAttributes(coatingVIS);
    PMT_Body_Logic->SetVisAttributes(PMT_VIS);

    // Place objects inside each other
    auto positionCoating = G4ThreeVector(0.*cm,0.*cm,2*shieldingHalfThicknessLid + coatingHalfLength - detectorHalfinclPMT);
    new G4PVPlacement(0, positionCoating,
                      Coating_Logic, "Coating_Phys", Shielding_Logic, false, copy_no, checkOverlap);

    auto positionCoatingPlexi = G4ThreeVector(0.*cm, 0.*cm, 0.5*coatingAlThicknessFront);
    new G4PVPlacement(0, positionCoatingPlexi,
                      PlexiGlas_Coating_Logic, "PlexiGlas_Coating_Phys", Coating_Logic, false, copy_no, checkOverlap);

    auto positionReflector = G4ThreeVector(0.*cm, 0.*cm, 0.5 * coatingPlasticThickness);
    new G4PVPlacement(0, positionReflector,
                      Reflector_Logic, "Reflector_Phys", PlexiGlas_Coating_Logic, false, copy_no, checkOverlap);

    auto positionCrystal = G4ThreeVector(0.*cm, 0.*cm, 0.5*reflectorThickness);
    new G4PVPlacement(0, positionCrystal,
                      Crystal_Logic, /*"Crystal_Phys"*/"OCL_Crystal", Reflector_Logic, false, copy_no, checkOverlap);

    auto positionPlexiWindow = positionCoating + G4ThreeVector(0.*cm, 0.*cm, coatingHalfLength + plexiGlasWindowHalfLength);
    new G4PVPlacement(0, positionPlexiWindow,
                      Crystal_Window_Logic, "Crystal_Window_Phys", Shielding_Logic, false, copy_no, checkOverlap);

    auto positionPMTandAir = positionPlexiWindow + G4ThreeVector(0.*cm, 0.*cm, plexiGlasWindowHalfLength + PMTandAirHalfLength);
    new G4PVPlacement(0, positionPMTandAir,
                      Detector_Logic, "Detector_Phys", Shielding_Logic, false, copy_no, checkOverlap);

    auto positionPMTWindow = G4ThreeVector(0.*cm, 0.*cm, PMTWindowHalfLength - PMTandAirHalfLength);
    new G4PVPlacement(0, positionPMTWindow,
                      PMT_Window_Logic, "PMT_Window_Phys", Detector_Logic, false, copy_no, checkOverlap);

    auto positionCathode = positionPMTWindow + G4ThreeVector(0.*cm,0.*cm,PMTWindowHalfLength + cathodeHalfLength);
    new G4PVPlacement(0, positionCathode,
                      Cathode_Logic, "Cathode_Phys", Detector_Logic, false, copy_no, checkOverlap);

    auto positionPMT = positionCathode + G4ThreeVector(0.*cm, 0.*cm, cathodeHalfLength + PMTHalfLength);
    new G4PVPlacement(0, positionPMT,
                      PMT_Body_Logic, "PMT_Body_Phys", Detector_Logic, false, copy_no, checkOverlap);
    return assembly;
}
