//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#include "detector/CloverFactory.hh"

#define INCBIN_PREFIX g_
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <meshreader/incbin.h>
#include <meshreader/MeshReader.hh>

#include <CADMesh/CADMesh.hh>

#include <G4Types.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4MultiUnion.hh>

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

#define DATA_NAME(NAME) \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
        INCBIN_STYLE_IDENT(DATA))

#define SIZE_NAME(NAME) \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
        INCBIN_STYLE_IDENT(SIZE))
#define CONVERT_TO_STRUCT(NAME) \
        { \
        DATA_NAME(NAME), \
        SIZE_NAME(NAME)}

#define INCNS(NAMESPACE, NAME, PATH) \
    INCBIN(NAME, PATH); \
    namespace NAMESPACE { static const MeshReader::MemBuf_t NAME = CONVERT_TO_STRUCT(NAME); }

// Including all the CLOVER HPGe mesh models
INCNS(HPGe_Components, vaccum, CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply");
INCNS(HPGe_Components, encasement, CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply");
INCBIN(HPGeCrystalA, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply");
INCBIN(HPGeCrystalB, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal2_10umTolerance.ply");
INCBIN(HPGeCrystalC, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal3_10umTolerance.ply");
INCBIN(HPGeCrystalD, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal4_10umTolerance.ply");
INCBIN(HPGeContactA, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact1_10um.ply");
INCBIN(HPGeContactB, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact2_10um.ply");
INCBIN(HPGeContactC, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact3_10um.ply");
INCBIN(HPGeContactD, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact4_10um.ply");


namespace HPGe_Components {

    static const MeshReader::MemBuf_t crystals[] = {
            CONVERT_TO_STRUCT(HPGeCrystalA), CONVERT_TO_STRUCT(HPGeCrystalB),
            CONVERT_TO_STRUCT(HPGeCrystalC), CONVERT_TO_STRUCT(HPGeCrystalD)};

    static const MeshReader::MemBuf_t contacts[] = {
            CONVERT_TO_STRUCT(HPGeContactA), CONVERT_TO_STRUCT(HPGeContactB),
            CONVERT_TO_STRUCT(HPGeContactC), CONVERT_TO_STRUCT(HPGeContactD)
    };
}

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

// Including all the shield components
INCNS(Shield_parts, body, CLOVER_PATH"/Shield/Body/Body_Modified2_tol_10um.ply");
INCNS(Shield_parts, heavymet, CLOVER_PATH"/Shield/Heavimet/HEAVIMET_30mm.ply");
INCNS(Shield_parts, PMTConArray, CLOVER_PATH"/Shield/PMT-Connectors/PMT-ConnecterArray.ply");
INCBIN(BGOCrystal0, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_1.ply");
INCBIN(BGOCrystal1, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_2.ply");
INCBIN(BGOCrystal2, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_3.ply");
INCBIN(BGOCrystal3, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_4.ply");
INCBIN(BGOCrystal4, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_5.ply");
INCBIN(BGOCrystal5, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_6.ply");
INCBIN(BGOCrystal6, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_7.ply");
INCBIN(BGOCrystal7, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_8.ply");
INCBIN(BGOCrystal8, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_9.ply");
INCBIN(BGOCrystal9, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_10.ply");
INCBIN(BGOCrystal10, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_11.ply");
INCBIN(BGOCrystal11, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_12.ply");
INCBIN(BGOCrystal12, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_13.ply");
INCBIN(BGOCrystal13, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_14.ply");
INCBIN(BGOCrystal14, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_15.ply");
INCBIN(BGOCrystal15, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_16.ply");
INCBIN(BGOPMT0, CLOVER_PATH"/Shield/PMTs/PMT1.ply"); INCBIN(BGOPMT1, CLOVER_PATH"/Shield/PMTs/PMT2.ply");
INCBIN(BGOPMT2, CLOVER_PATH"/Shield/PMTs/PMT3.ply"); INCBIN(BGOPMT3, CLOVER_PATH"/Shield/PMTs/PMT4.ply");
INCBIN(BGOPMT4, CLOVER_PATH"/Shield/PMTs/PMT5.ply"); INCBIN(BGOPMT5, CLOVER_PATH"/Shield/PMTs/PMT6.ply");
INCBIN(BGOPMT6, CLOVER_PATH"/Shield/PMTs/PMT7.ply"); INCBIN(BGOPMT7, CLOVER_PATH"/Shield/PMTs/PMT8.ply");
INCBIN(BGOPMT8, CLOVER_PATH"/Shield/PMTs/PMT9.ply"); INCBIN(BGOPMT9, CLOVER_PATH"/Shield/PMTs/PMT10.ply");
INCBIN(BGOPMT10, CLOVER_PATH"/Shield/PMTs/PMT11.ply"); INCBIN(BGOPMT11, CLOVER_PATH"/Shield/PMTs/PMT12.ply");
INCBIN(BGOPMT12, CLOVER_PATH"/Shield/PMTs/PMT13.ply"); INCBIN(BGOPMT13, CLOVER_PATH"/Shield/PMTs/PMT14.ply");
INCBIN(BGOPMT14, CLOVER_PATH"/Shield/PMTs/PMT15.ply"); INCBIN(BGOPMT15, CLOVER_PATH"/Shield/PMTs/PMT16.ply");

namespace Shield_parts {
    static const MeshReader::MemBuf_t BGOCrystals[] = {
            CONVERT_TO_STRUCT(BGOCrystal0), CONVERT_TO_STRUCT(BGOCrystal1),
            CONVERT_TO_STRUCT(BGOCrystal2), CONVERT_TO_STRUCT(BGOCrystal3),
            CONVERT_TO_STRUCT(BGOCrystal4), CONVERT_TO_STRUCT(BGOCrystal5),
            CONVERT_TO_STRUCT(BGOCrystal6), CONVERT_TO_STRUCT(BGOCrystal7),
            CONVERT_TO_STRUCT(BGOCrystal8), CONVERT_TO_STRUCT(BGOCrystal9),
            CONVERT_TO_STRUCT(BGOCrystal10), CONVERT_TO_STRUCT(BGOCrystal11),
            CONVERT_TO_STRUCT(BGOCrystal12), CONVERT_TO_STRUCT(BGOCrystal13),
            CONVERT_TO_STRUCT(BGOCrystal14), CONVERT_TO_STRUCT(BGOCrystal15)
    };

    static const MeshReader::MemBuf_t BGOPMTs[] = {
            CONVERT_TO_STRUCT(BGOPMT0), CONVERT_TO_STRUCT(BGOPMT1),
            CONVERT_TO_STRUCT(BGOPMT2), CONVERT_TO_STRUCT(BGOPMT3),
            CONVERT_TO_STRUCT(BGOPMT4), CONVERT_TO_STRUCT(BGOPMT5),
            CONVERT_TO_STRUCT(BGOPMT6), CONVERT_TO_STRUCT(BGOPMT7),
            CONVERT_TO_STRUCT(BGOPMT8), CONVERT_TO_STRUCT(BGOPMT9),
            CONVERT_TO_STRUCT(BGOPMT10), CONVERT_TO_STRUCT(BGOPMT11),
            CONVERT_TO_STRUCT(BGOPMT12), CONVERT_TO_STRUCT(BGOPMT13),
            CONVERT_TO_STRUCT(BGOPMT14), CONVERT_TO_STRUCT(BGOPMT15)
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

static const char *encasement_path = CLOVER_PATH"/CloverEncasement/CloverEncasement.ply";

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
    , solid_future( std::async(std::launch::async, [this](){ return this->mesh->TessellatedMesh(); }) )
    , solid( nullptr ){}

void Solids_t::await()
{
    solid_future.wait();
    solid = solid_future.get();
}

HPGeFactory::HPGeFactory(const char *vacuum_path, const char *encasement_path, const char *crystals_path[],
                         const char *contacts_path[])
    : Detector::DetectorFactory( Detector::Type::clover )
    , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
    , fMatAluminium( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
    , fMatGe( G4NistManager::Instance()->FindOrBuildMaterial("G4_Ge") )
    , internal_vacuum(HPGe_Components::vaccum, "vaccum", offset_Vacuum)
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
    if ( !fMatVaccum || !fMatAluminium || !fMatGe )
        throw std::runtime_error("Could not find materials needed.");
}

G4AssemblyVolume *HPGeFactory::GetAssembly(const int &copy_no, const bool &overlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., 0.);
    G4RotationMatrix rot0;

    auto* CLOVER_InternalVacuum_VisAtt = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7));
    CLOVER_InternalVacuum_VisAtt->SetVisibility(false);
    auto *internal_volume_logical = new G4LogicalVolume(internal_vacuum.GetSolid(), fMatVaccum, "Internal_Volume_Logic");
    internal_volume_logical->SetVisAttributes(CLOVER_InternalVacuum_VisAtt);
    assembly->AddPlacedVolume(internal_volume_logical, pos0, &rot0);

    auto *CLOVER_Encasement_VisAtt = new G4VisAttributes(G4Colour(1, 0., 0.));
    CLOVER_Encasement_VisAtt->SetVisibility(false);
    auto *encasement_volume_logical = new G4LogicalVolume(encasement.GetSolid(), fMatAluminium, "Encasement_Volume_Logic");
    encasement_volume_logical->SetVisAttributes(CLOVER_Encasement_VisAtt);
    assembly->AddPlacedVolume(encasement_volume_logical, pos0, &rot0);



    auto* CLOVER_DeadLayer_Lithium_VisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
    auto* CLOVER_HPGeCrystals_VisAtt = new G4VisAttributes(G4Colour(0.9, 0.9, 0.0));

    CLOVER_DeadLayer_Lithium_VisAtt->SetForceSolid(true);

    CLOVER_DeadLayer_Lithium_VisAtt->SetVisibility(true);
    CLOVER_HPGeCrystals_VisAtt->SetVisibility(true);

    // Place the HPGe inside the internal volume
    for ( int i = 0 ; i < numberOf_CLOVER_Crystals ; ++i ) {
        auto *crystal_logical = new G4LogicalVolume(crystals[i].GetSolid(), fMatGe, "LogicCLOVERHPGeCrystal");
        crystal_logical->SetVisAttributes(CLOVER_HPGeCrystals_VisAtt);
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
                          crystal_logical,
                          "CLOVER_HPGeCrystal",
                          internal_volume_logical,
                          false,
                          copy_no * numberOf_CLOVER_Crystals + i,
                          overlap);
        auto *contact_logical = new G4LogicalVolume(contacts[i].GetSolid(), fMatGe, "LogicCLOVERHPGeCrystal_LithiumContact");
        contact_logical->SetVisAttributes(CLOVER_DeadLayer_Lithium_VisAtt);
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
                          contact_logical,
                          "CLOVER_HPGeLithiumDopedDeadlayer",
                          crystal_logical,
                          false,
                          copy_no * numberOf_CLOVER_Crystals + i,
                          overlap);
    }

    return assembly;
}

ShieldFactory::ShieldFactory(const char *body_path, const char *heavimet_path, const char *PMTConArray_path,
                             const char *BGOCrystal_path[], const char *PMT_path[])
    : Detector::DetectorFactory( Detector::Type::clover )
    , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
    , fMatAluminium( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
    , fMatHeavimet( G4NistManager::Instance()->FindOrBuildMaterial("Heavimet_Material") )
    , fMatBGO( G4NistManager::Instance()->FindOrBuildMaterial("G4_BGO") )
    , body(body_path, offset_body)
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
    , BGOVolume( new G4MultiUnion() )
{
    if ( !fMatHeavimet ){
        fMatHeavimet = new G4Material("Heavimet_Material",19.25*g/cm3, 5);
        fMatHeavimet->AddElement(G4NistManager::Instance()->FindOrBuildElement("W"), 94.20*perCent);
        fMatHeavimet->AddElement(G4NistManager::Instance()->FindOrBuildElement("Ni"), 4.35*perCent);
        fMatHeavimet->AddElement(G4NistManager::Instance()->FindOrBuildElement("Fe"), 0.85*perCent);
        fMatHeavimet->AddElement(G4NistManager::Instance()->FindOrBuildElement("Co"), 0.50*perCent);
        fMatHeavimet->AddElement(G4NistManager::Instance()->FindOrBuildElement("Cu"), 0.10*perCent);
    }

    {
        auto rotm = G4RotationMatrix();
        auto pos = G4ThreeVector(0,0,0);
        auto tr = G4Transform3D(rotm, pos);
        for ( auto &crystal : BGOCrystal ){
            BGOVolume->AddNode(*crystal.GetSolid(), tr);
        }
        BGOVolume->Voxelize();
    }

}

G4AssemblyVolume *ShieldFactory::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., 0.);
    G4RotationMatrix rot0;

    auto* CLOVER_ShieldBody_VisAtt = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6));
    CLOVER_ShieldBody_VisAtt->SetForceSolid(true);
    CLOVER_ShieldBody_VisAtt->SetVisibility(true);
    auto *body_logical = new G4LogicalVolume(body.GetSolid(), fMatAluminium, "LogicCLOVERShieldBody");
    body_logical->SetVisAttributes(CLOVER_ShieldBody_VisAtt);
    assembly->AddPlacedVolume(body_logical, pos0, &rot0);

    auto* CLOVER_Shield_HEAVIMET_VisAtt = new G4VisAttributes(G4Colour(0.4, 0.2, 0.2));
    CLOVER_Shield_HEAVIMET_VisAtt->SetForceSolid(true);
    CLOVER_Shield_HEAVIMET_VisAtt->SetVisibility(true);
    auto *heavimet_logical = new G4LogicalVolume(heavimet.GetSolid(), fMatHeavimet, "LogicCLOVERShieldHeavimet");
    heavimet_logical->SetVisAttributes(CLOVER_Shield_HEAVIMET_VisAtt);
    assembly->AddPlacedVolume(heavimet_logical, pos0, &rot0);

    /*auto *CLOVER_Shield_PMTConArray_VisAtt = new G4VisAttributes(G4Colour::White());
    CLOVER_Shield_PMTConArray_VisAtt->SetVisibility(true);
    auto *Logic_CLOVER_Shield_PMTConArray = new G4LogicalVolume(PMTConArray.GetSolid(), fMatAluminium, "LogicCLOVERShieldPMTConArray");
    Logic_CLOVER_Shield_PMTConArray->SetVisAttributes(CLOVER_Shield_PMTConArray_VisAtt);
    assembly->AddPlacedVolume(Logic_CLOVER_Shield_PMTConArray, pos0, &rot0);*/

    auto *Shield_BGOCrystal_VisAtt = new G4VisAttributes(G4Colour(0., 0., 1.0));
    auto *Shield_PMT_VisAtt = new G4VisAttributes(G4Colour(0., 1., 0.0));
    Shield_BGOCrystal_VisAtt->SetVisibility(false);
    Shield_PMT_VisAtt->SetVisibility(false);

    for ( int i = 0 ; i < numberOf_BGO_Crystals ; ++i ){
        auto *bgo_logical = new G4LogicalVolume(BGOCrystal[i].GetSolid(), fMatBGO, "LogicCLOVERShieldBGOCrystal");
        bgo_logical->SetVisAttributes(Shield_BGOCrystal_VisAtt);
        assembly->AddPlacedVolume(bgo_logical, pos0, &rot0);
        auto *pmt_logical = new G4LogicalVolume(PMT[i].GetSolid(), fMatAluminium, "LogicCLOVERShieldPMT");
        pmt_logical->SetVisAttributes(Shield_PMT_VisAtt);
        assembly->AddPlacedVolume(pmt_logical, pos0, &rot0);
    }

    return assembly;
}

CloverFactory::CloverFactory(const bool &have_HPGe, const bool &have_Shield)
    : Detector::DetectorFactory( Detector::Type::clover )
    , crystalFactory( have_HPGe ? new HPGeFactory(HPGe_Paths::vacuum_path, HPGe_Paths::encasement_path,
                                                  HPGe_Paths::crystals_path, HPGe_Paths::contacts_path) : nullptr)
    , shieldFactory( have_Shield ? new ShieldFactory(Shields_Paths::body_path, Shields_Paths::heavimet_path,
                                                     Shields_Paths::PMTConArray_path, Shields_Paths::bgo_path,
                                                     Shields_Paths::pmt_path) : nullptr )
    , encasement( encasement_path, G4ThreeVector() )
    , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
{}

CloverFactory::~CloverFactory()
{
    delete crystalFactory;
    delete shieldFactory;
}

G4AssemblyVolume *CloverFactory::GetAssembly(const int &copy_no, const bool &checkOverlap)
{
    auto *assembly = new G4AssemblyVolume();
    G4ThreeVector pos0(0.,0., 0.);
    G4RotationMatrix rot0;
    if ( crystalFactory )
        assembly->AddPlacedAssembly(crystalFactory->GetAssembly(copy_no, checkOverlap), pos0, &rot0);
    if ( shieldFactory )
        assembly->AddPlacedAssembly(shieldFactory->GetAssembly(copy_no, checkOverlap), pos0, &rot0);


    return assembly;
}