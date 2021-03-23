//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#include <CADMesh/CADMesh.hh>

#include <G4Types.hh>
#include <G4SystemOfUnits.hh>
#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>

#include "CloverFactory.hh"

#ifndef SRC_PATH
    #define SRC_PATH "../"
#endif // SRC_PATH

#ifndef PLY_PATH
    #define PLY_PATH SRC_PATH"Mesh-Models"
#endif // PLY_PATH

#ifndef DETECTOR_PATH
    #define DETECTOR_PATH PLY_PATH"/DETECTORS"
#endif // DETECTOR_PATH

#ifndef CLOVER_PATH
    #define CLOVER_PATH DETECTOR_PATH"/CLOVER"
#endif // CLOVER_PATH

namespace HPGe_Paths {
    static const char *vacuum_path = CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply";
    static const char *encasement_path = CLOVER_PATH"/CloverEncasement/CloverEncasement_new_approx.ply";
    static const char *crystals_path[] = {
            CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal2_10umTolerance.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal3_10umTolerance.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal4_10umTolerance.ply"
    };
    static const char *contacts_path[] = {
            CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact1_10um.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact2_10um.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact3_10um.ply",
            CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact4_10um.ply"
    };
}

namespace Shields_Paths {
    static const char *body_path = CLOVER_PATH"/Shield/Body/Body_Modified2_tol_10um.ply";
    static const char *heavimet_path = CLOVER_PATH"/Shield/Heavimet/HEAVIMET_30mm.ply";
    static const char *PMTConArray_path = CLOVER_PATH"/Shield/PMT-Connectors/PMT-ConnecterArray.ply";
    static const char *bgo_path[] = {
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_1.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_2.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_3.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_4.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_5.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_6.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_7.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_8.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_9.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_10.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_11.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_12.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_13.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_14.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_15.ply",
            CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_16.ply"};
    static const char *pmt_path[] = {
            CLOVER_PATH"/Shield/PMTs/PMT1.ply", CLOVER_PATH"/Shield/PMTs/PMT2.ply", CLOVER_PATH"/Shield/PMTs/PMT3.ply",
            CLOVER_PATH"/Shield/PMTs/PMT4.ply", CLOVER_PATH"/Shield/PMTs/PMT5.ply", CLOVER_PATH"/Shield/PMTs/PMT6.ply",
            CLOVER_PATH"/Shield/PMTs/PMT7.ply", CLOVER_PATH"/Shield/PMTs/PMT8.ply", CLOVER_PATH"/Shield/PMTs/PMT9.ply",
            CLOVER_PATH"/Shield/PMTs/PMT10.ply", CLOVER_PATH"/Shield/PMTs/PMT11.ply", CLOVER_PATH"/Shield/PMTs/PMT12.ply",
            CLOVER_PATH"/Shield/PMTs/PMT13.ply", CLOVER_PATH"/Shield/PMTs/PMT14.ply", CLOVER_PATH"/Shield/PMTs/PMT15.ply",
            CLOVER_PATH"/Shield/PMTs/PMT16.ply"};
}



G4Material *GetHeavimet()
{
    G4Material* Heavimet_Material = G4Material::GetMaterial("Heavimet_Material", false);
    if ( Heavimet_Material ){
        return Heavimet_Material;
    } else {
        Heavimet_Material = new G4Material("Heavimet_Material",19.25*g/cm3, 5);
        Heavimet_Material->AddElement(G4Element::GetElement("W"), 94.20*perCent);
        Heavimet_Material->AddElement(G4Element::GetElement("Ni"), 4.35*perCent);
        Heavimet_Material->AddElement(G4Element::GetElement("Fe"), 0.85*perCent);
        Heavimet_Material->AddElement(G4Element::GetElement("Co"), 0.50*perCent);
        Heavimet_Material->AddElement(G4Element::GetElement("Cu"), 0.10*perCent);
        return Heavimet_Material;
    }
}

constexpr G4double CLOVERtoShield_displacement = 7.3;  // cm

G4ThreeVector HPGeFactory::offset_Vacuum = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
G4ThreeVector HPGeFactory::offset_Encasement = G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm);
G4ThreeVector HPGeFactory::offset_Crystals[] = {
        G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm),
        G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm),
        G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm),
        G4ThreeVector(0*cm, 0*cm, -CLOVERtoShield_displacement*cm),
};

G4ThreeVector ShieldFactory::offset_body = G4ThreeVector(0*cm, 0*cm, 0*cm);
G4ThreeVector ShieldFactory::offset_heavimet = G4ThreeVector(0*cm, 0*cm, 0*cm);
G4ThreeVector ShieldFactory::offset_PMTConArray = G4ThreeVector(0*cm, 0*cm, 0*cm);
G4ThreeVector ShieldFactory::offset_BGOCrystals = G4ThreeVector(0*cm, 0*cm, 0*cm);
G4ThreeVector ShieldFactory::offset_PMT = G4ThreeVector(0*cm, 0*cm, 0*cm);

Solids_t::Solids_t(const char *path, const G4ThreeVector &offset)
    : mesh( new CADMesh(path, "PLY", mm, offset, false) )
    , solid( mesh->TessellatedMesh() ){}

HPGeFactory::HPGeFactory(const char *vacuum_path, const char *encasement_path, const char *crystals_path[],
                         const char *contacts_path[])
    : internal_vacuum(vacuum_path, offset_Vacuum)
    , encasement(encasement_path, offset_Encasement)
    , crystals{ Solids_t(crystals_path[0], offset_Crystals[0]),
                Solids_t(crystals_path[1], offset_Crystals[1]),
                Solids_t(crystals_path[2], offset_Crystals[2]),
                Solids_t(crystals_path[3], offset_Crystals[3])}
    , contacts{ Solids_t(contacts_path[0], offset_Crystals[0]),
                Solids_t(contacts_path[1], offset_Crystals[1]),
                Solids_t(contacts_path[2], offset_Crystals[2]),
                Solids_t(contacts_path[3], offset_Crystals[3])}
{
}

void HPGeFactory::SetupLogical(HPGeDetector *detector) const
{
    G4Material* G4_Galactic_Material = G4Material::GetMaterial("G4_Galactic");
    G4Material* G4_Ge_Material = G4Material::GetMaterial("G4_Ge");
    G4Material* G4_Al_Material = G4Material::GetMaterial("G4_Al");

    detector->InternalVacuum.logicalVolume =
            new G4LogicalVolume(internal_vacuum.solid, G4_Galactic_Material, "LogicCLOVERInternalVacuum");


    detector->Encasement.logicalVolume =
            new G4LogicalVolume(encasement.solid, G4_Al_Material, "LogicCLOVERCloverEncasement");


    auto* CLOVER_DeadLayer_Lithium_VisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
    CLOVER_DeadLayer_Lithium_VisAtt->SetForceSolid(true);
    CLOVER_DeadLayer_Lithium_VisAtt->SetVisibility(true);

    auto* CLOVER_HPGeCrystals_VisAtt = new G4VisAttributes(G4Colour(0.9, 0.9, 0.0));
    CLOVER_HPGeCrystals_VisAtt->SetVisibility(true);

    for ( int i = 0 ; i < numberOf_CLOVER_Crystals ; ++i ){
        std::string name = "LogicCLOVERHPGeCrystal";
        name.push_back('A'+i);
        detector->HPGeCrystals[i].logicalVolume =
                new G4LogicalVolume(crystals[i].solid, G4_Ge_Material, "LogicCLOVERHPGeCrystal");
        name = "LogicCLOVERHPGeCrystal_LithiumContact";
        name.push_back('A'+i);
        detector->HPGeContacts[i].logicalVolume =
                new G4LogicalVolume(contacts[i].solid, G4_Ge_Material, "LogicCLOVERHPGeCrystal_LithiumContact");
    }
}

void HPGeFactory::SetupVis(HPGeDetector *detector) const
{
    auto* CLOVER_InternalVacuum_VisAtt = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7));
    auto *CLOVER_Encasement_VisAtt = new G4VisAttributes(G4Colour(1, 0., 0.));
    auto* CLOVER_HPGeCrystals_VisAtt = new G4VisAttributes(G4Colour(0.9, 0.9, 0.0));
    auto* CLOVER_DeadLayer_Lithium_VisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));


    CLOVER_InternalVacuum_VisAtt->SetVisibility(false);
    CLOVER_Encasement_VisAtt->SetVisibility(true);
    CLOVER_HPGeCrystals_VisAtt->SetVisibility(true);
    CLOVER_DeadLayer_Lithium_VisAtt->SetVisibility(true);

    CLOVER_DeadLayer_Lithium_VisAtt->SetForceSolid(true);

    detector->InternalVacuum.logicalVolume->SetVisAttributes(CLOVER_InternalVacuum_VisAtt);
    detector->Encasement.logicalVolume->SetVisAttributes(CLOVER_Encasement_VisAtt);

    for ( int i = 0 ; i < numberOf_CLOVER_Crystals ; ++i){
        detector->HPGeCrystals[i].logicalVolume->SetVisAttributes(CLOVER_HPGeCrystals_VisAtt);
        detector->HPGeContacts[i].logicalVolume->SetVisAttributes(CLOVER_DeadLayer_Lithium_VisAtt);
    }
}

void HPGeFactory::SetupPhysical(HPGeDetector *detector, G4LogicalVolume *parent, const G4ThreeVector &pos,
                                const G4RotationMatrix &rot, const int &copy_no, const bool &overlap) const
{
    G4Transform3D transform = G4Transform3D(rot, pos);
    detector->InternalVacuum.physicalVolume = new G4PVPlacement(transform,
                                                                detector->InternalVacuum.logicalVolume,
                                                                "CLOVER_InternalVacuum",
                                                                 parent,
                                                                false,
                                                                copy_no,
                                                                overlap);
    detector->Encasement.physicalVolume = new G4PVPlacement(transform,
                                                            detector->Encasement.logicalVolume,
                                                            "CLOVER_Encasement",
                                                            parent,
                                                            false,
                                                            copy_no,
                                                            overlap);

    for ( int i = 0 ; i < numberOf_CLOVER_Crystals ; ++i ){
        detector->HPGeCrystals[i].physicalVolume =
                new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                  detector->HPGeCrystals[i].logicalVolume,
                                  "CLOVER_HPGeCrystal",
                                  detector->InternalVacuum.logicalVolume,
                                  false,
                                  copy_no*4 + i,
                                  overlap);
        detector->HPGeContacts[i].physicalVolume =
                new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                  detector->HPGeContacts[i].logicalVolume,
                                  "CLOVER_HPGeLithiumDopedDeadlayer",
                                  detector->HPGeCrystals[i].logicalVolume,
                                  false,
                                  copy_no*4 + i,
                                  overlap);
    }
}

HPGeDetector HPGeFactory::Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                                        const int &copy_no, const bool &overlap) const
{
    HPGeDetector detector;
    SetupLogical(&detector);
    SetupVis(&detector);
    SetupPhysical(&detector, parent, pos, rot, copy_no, overlap);
    return detector;
}

ShieldFactory::ShieldFactory(const char *body_path, const char *heavimet_path, const char *PMTConArray_path,
                             const char *BGOCrystal_path[], const char *PMT_path[])
    : body(body_path, offset_body)
    , heavimet(heavimet_path, offset_heavimet)
    , PMTConArray(PMTConArray_path, offset_PMTConArray)
    , BGOCrystal{ Solids_t(BGOCrystal_path[0], offset_BGOCrystals), Solids_t(BGOCrystal_path[1], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[2], offset_BGOCrystals), Solids_t(BGOCrystal_path[3], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[4], offset_BGOCrystals), Solids_t(BGOCrystal_path[5], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[6], offset_BGOCrystals), Solids_t(BGOCrystal_path[7], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[8], offset_BGOCrystals), Solids_t(BGOCrystal_path[9], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[10], offset_BGOCrystals), Solids_t(BGOCrystal_path[11], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[12], offset_BGOCrystals), Solids_t(BGOCrystal_path[13], offset_BGOCrystals),
                  Solids_t(BGOCrystal_path[14], offset_BGOCrystals), Solids_t(BGOCrystal_path[15], offset_BGOCrystals)}
    , PMT{ Solids_t(PMT_path[0], offset_PMT), Solids_t(PMT_path[1], offset_PMT),
          Solids_t(PMT_path[2], offset_PMT), Solids_t(PMT_path[3], offset_PMT),
          Solids_t(PMT_path[4], offset_PMT), Solids_t(PMT_path[5], offset_PMT),
          Solids_t(PMT_path[6], offset_PMT), Solids_t(PMT_path[7], offset_PMT),
          Solids_t(PMT_path[8], offset_PMT), Solids_t(PMT_path[9], offset_PMT),
          Solids_t(PMT_path[10], offset_PMT), Solids_t(PMT_path[11], offset_PMT),
          Solids_t(PMT_path[12], offset_PMT), Solids_t(PMT_path[13], offset_PMT),
          Solids_t(PMT_path[14], offset_PMT), Solids_t(PMT_path[15], offset_PMT)}
{
}

void ShieldFactory::SetupLogical(ShieldDetector *detector) const
{
    auto *G4_Al_Material = G4Material::GetMaterial("G4_Al");
    auto *Heavimet_Material = GetHeavimet();
    auto* G4_BGO_Material = G4Material::GetMaterial("G4_BGO");

    detector->Body.logicalVolume = new G4LogicalVolume(body.solid, G4_Al_Material, "LogicCLOVERShieldBody");
    detector->Heavimet.logicalVolume = new G4LogicalVolume(heavimet.solid, Heavimet_Material,
                                                           "LogicCLOVERShieldHeavimet");
    detector->PMTConArray.logicalVolume = new G4LogicalVolume(PMTConArray.solid, G4_Al_Material,
                                                              "LogicCLOVERShieldPMTConArray");

    char name[128];
    for (int i = 0 ; i < numberOf_BGO_Crystals ; ++i) {
        sprintf(name, "LogicCLOVERShieldBGOCrystal_%02d", i);
        detector->BGOCrystals[i].logicalVolume =
                new G4LogicalVolume(BGOCrystal[i].solid, G4_BGO_Material, name);
        sprintf(name, "LogicCLOVERShieldPMT_%02d", i);
        detector->PMT[i].logicalVolume =
                new G4LogicalVolume(PMT[i].solid, G4_Al_Material, name);
    }
}

void ShieldFactory::SetupVis(ShieldDetector *detector) const
{
    auto* CLOVER_ShieldBody_VisAtt = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6));
    auto* CLOVER_Shield_HEAVIMET_VisAtt = new G4VisAttributes(G4Colour(0.4, 0.2, 0.2));
    auto *Shield_BGOCrystal_VisAtt = new G4VisAttributes(G4Colour(0., 0., 1.0));
    auto *Shield_PMT_VisAtt = new G4VisAttributes(G4Colour(0., 1., 0.0));

    CLOVER_ShieldBody_VisAtt->SetForceSolid(true);
    CLOVER_Shield_HEAVIMET_VisAtt->SetForceSolid(true);
    Shield_PMT_VisAtt->SetForceSolid(true);

    CLOVER_ShieldBody_VisAtt->SetVisibility(true);
    CLOVER_Shield_HEAVIMET_VisAtt->SetVisibility(true);
    Shield_BGOCrystal_VisAtt->SetVisibility(true);
    Shield_PMT_VisAtt->SetVisibility(true);

    detector->Body.logicalVolume->SetVisAttributes(CLOVER_ShieldBody_VisAtt);
    detector->Heavimet.logicalVolume->SetVisAttributes(CLOVER_Shield_HEAVIMET_VisAtt);

    for ( int i = 0 ; i < numberOf_BGO_Crystals ; ++i ) {
        detector->BGOCrystals[i].logicalVolume->SetVisAttributes(Shield_BGOCrystal_VisAtt);
        detector->PMT[i].logicalVolume->SetVisAttributes(Shield_PMT_VisAtt);
    }
}

void ShieldFactory::SetupPhysical(ShieldDetector *detector, G4LogicalVolume *parent, const G4ThreeVector &pos,
                                  const G4RotationMatrix &rot, const int &copy_no, const bool &overlap) const
{
    G4Transform3D transform = G4Transform3D(rot, pos);
    char volName[128];
    sprintf(volName, "CLOVER_Shield_Body_%02d", copy_no);
    detector->Body.physicalVolume =
            new G4PVPlacement(transform, detector->Body.logicalVolume, volName, parent, false, copy_no, overlap);

    sprintf(volName, "CLOVER_Shield_Heavimet_%02d", copy_no);
    detector->Heavimet.physicalVolume =
            new G4PVPlacement(transform, detector->Heavimet.logicalVolume, volName, parent, false, copy_no, overlap);

    /*sprintf(volName, "CLOVER_Shield_PMTConArray_%02d", copy_no);
    detector->PMTConArray.physicalVolume =
            new G4PVPlacement(transform, detector->PMTConArray.logicalVolume, volName, parent, false, copy_no, overlap);
    */

    for ( int i = 0 ; i < numberOf_BGO_Crystals ; ++i ) {
        sprintf(volName, "CLOVER_Shield_BGOCrystal_%02d_%02d", copy_no, i);
        detector->BGOCrystals[i].physicalVolume =
                new G4PVPlacement(transform,
                                  detector->BGOCrystals[i].logicalVolume,
                                  "CLOVER_Shield_BGOCrystal",//volName,
                                  parent,
                                  false,
                                  copy_no * numberOf_BGO_Crystals + i,
                                  overlap);
        sprintf(volName, "CLOVER_Shield_PMT_%02d_%02d", copy_no, i);
        detector->PMT[i].physicalVolume =
                new G4PVPlacement(transform,
                                  detector->PMT[i].logicalVolume,
                                  volName,
                                  parent,
                                  false,
                                  copy_no * numberOf_BGO_Crystals + i,
                                  overlap);
    }
}

ShieldDetector ShieldFactory::Construct(G4LogicalVolume *parent, const G4ThreeVector &pos,
                                              const G4RotationMatrix &rot, const int &copy_no,
                                              const bool &overlap) const
{
    ShieldDetector detector;
    SetupLogical(&detector);
    SetupVis(&detector);
    SetupPhysical(&detector, parent, pos, rot, copy_no, overlap);
    return detector;
}


CloverFactory::CloverFactory(const bool &shield)
    : shieldConstruct( shield )
    , crystalFactory(new HPGeFactory(HPGe_Paths::vacuum_path, HPGe_Paths::encasement_path,
                                     HPGe_Paths::crystals_path, HPGe_Paths::contacts_path))
    , shieldFactory( ( shieldConstruct ) ? new ShieldFactory(Shields_Paths::body_path, Shields_Paths::heavimet_path,
                                                                   Shields_Paths::PMTConArray_path,
                                                                   Shields_Paths::bgo_path, Shields_Paths::pmt_path) : nullptr )
{}

CloverFactory::~CloverFactory()
{
    delete crystalFactory;
    delete shieldFactory;
}

CloverDetector CloverFactory::Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                                        const int &copy_no, const bool &overlap,  const bool &HPGe_present,
                                        const bool &shield_present) const
{
    CloverDetector detector;
    detector.HPGe = ( HPGe_present ) ?  crystalFactory->Construct(parent, pos, rot, copy_no, overlap) : HPGeDetector();
    detector.Shield = ( shieldConstruct && shield_present ) ? shieldFactory->Construct(parent, pos, rot, copy_no, overlap) : ShieldDetector();
    return detector;
}
