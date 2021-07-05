//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#include "detector/CloverFactory.hh"
#include "CloverData.hpp"


#include <meshreader/MeshReader.hh>

#include <G4Types.hh>
#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4MultiUnion.hh>
#include <G4AssemblyVolume.hh>

using namespace Detector;



class HPGeFactory : public Detector::DetectorFactory {

public:

    HPGeFactory()
        : Detector::DetectorFactory( Detector::Type::clover )
            , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
            , fMatAluminium( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
            , fMatGe( G4NistManager::Instance()->FindOrBuildMaterial("G4_Ge") )
            , internal_vacuum(HPGe_Components::vaccum, "vaccum", offset_Vacuum)
            , encasement(HPGe_Components::encasement, "encasement", offset_Encasement)
            , crystals{ MeshReader(HPGe_Components::crystals[0], "HPGeCrystalA", offset_Crystals[0]),
                        MeshReader(HPGe_Components::crystals[1], "HPGeCrystalB", offset_Crystals[1]),
                        MeshReader(HPGe_Components::crystals[2], "HPGeCrystalC", offset_Crystals[2]),
                        MeshReader(HPGe_Components::crystals[3], "HPGeCrystalD", offset_Crystals[3])}
            , contacts{ MeshReader(HPGe_Components::contacts[0], "HPGeContactsA", offset_Crystals[0]),
                        MeshReader(HPGe_Components::contacts[1], "HPGeContactsB", offset_Crystals[1]),
                        MeshReader(HPGe_Components::contacts[2], "HPGeContactsC", offset_Crystals[2]),
                        MeshReader(HPGe_Components::contacts[3], "HPGeContactsD", offset_Crystals[3])}
    {
        if ( !fMatVaccum || !fMatAluminium || !fMatGe )
            throw std::runtime_error("Could not find materials needed.");
    }

    void SetParameters(const Parameters *param) override {}

    ~HPGeFactory() override = default;

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &overlap) override {
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

private:

    G4Material *fMatVaccum;
    G4Material *fMatAluminium;
    G4Material *fMatGe;

    MeshReader internal_vacuum;
    MeshReader encasement;
    MeshReader crystals[numberOf_CLOVER_Crystals];
    MeshReader contacts[numberOf_CLOVER_Crystals];

    static G4ThreeVector offset_Vacuum;
    static G4ThreeVector offset_Encasement;
    static G4ThreeVector offset_Crystals[numberOf_CLOVER_Crystals];
};

class ShieldFactory : public Detector::DetectorFactory {

public:

    ShieldFactory()
        : Detector::DetectorFactory( Detector::Type::clover )
        , fMatVaccum( G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic") )
        , fMatAluminium( G4NistManager::Instance()->FindOrBuildMaterial("G4_Al") )
        , fMatHeavimet( G4NistManager::Instance()->FindOrBuildMaterial("Heavimet_Material") )
        , fMatBGO( G4NistManager::Instance()->FindOrBuildMaterial("G4_BGO") )
        , body(Shield_parts::body, "ShieldBody", offset_body)
        , heavimet(Shield_parts::heavimet, "ShieldHeavimet", offset_heavimet)
        , PMTConArray(Shield_parts::PMTConArray, "ShieldPMTConArray", offset_PMTConArray)
        , BGOCrystal{ MeshReader(Shield_parts::BGOCrystals[0], "ShieldBGOCrystal0", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[1], "ShieldBGOCrystal1", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[2], "ShieldBGOCrystal2", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[3], "ShieldBGOCrystal3", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[4], "ShieldBGOCrystal4", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[5], "ShieldBGOCrystal5", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[6], "ShieldBGOCrystal6", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[7], "ShieldBGOCrystal7", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[8], "ShieldBGOCrystal8", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[9], "ShieldBGOCrystal9", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[10], "ShieldBGOCrystal10", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[11], "ShieldBGOCrystal11", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[12], "ShieldBGOCrystal12", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[13], "ShieldBGOCrystal13", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[14], "ShieldBGOCrystal14", offset_BGOCrystals),
                    MeshReader(Shield_parts::BGOCrystals[15], "ShieldBGOCrystal15", offset_BGOCrystals)}
        , PMT{ MeshReader(Shield_parts::BGOPMTs[0], "ShieldBGOPMT0", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[1], "ShieldBGOPMT1", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[2], "ShieldBGOPMT2", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[3], "ShieldBGOPMT3", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[4], "ShieldBGOPMT4", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[5], "ShieldBGOPMT5", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[6], "ShieldBGOPMT6", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[7], "ShieldBGOPMT7", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[8], "ShieldBGOPMT8", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[9], "ShieldBGOPMT9", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[10], "ShieldBGOPMT10", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[11], "ShieldBGOPMT11", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[12], "ShieldBGOPMT12", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[13], "ShieldBGOPMT13", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[14], "ShieldBGOPMT14", offset_PMT),
                    MeshReader(Shield_parts::BGOPMTs[15], "ShieldBGOPMT15", offset_PMT)}
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

    void SetParameters(const Parameters *param) override {}

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &overlap) override
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

private:

    G4Material *fMatVaccum;
    G4Material *fMatAluminium;
    G4Material *fMatHeavimet;
    G4Material *fMatBGO;

    MeshReader body;
    MeshReader heavimet;
    MeshReader PMTConArray;
    MeshReader BGOCrystal[numberOf_BGO_Crystals];
    MeshReader PMT[numberOf_BGO_Crystals];
    G4MultiUnion *BGOVolume;

    static G4ThreeVector offset_body;
    static G4ThreeVector offset_heavimet;
    static G4ThreeVector offset_PMTConArray;
    static G4ThreeVector offset_BGOCrystals;
    static G4ThreeVector offset_PMT;
};

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

CloverFactory::CloverFactory(const bool &have_HPGe, const bool &have_Shield)
    : Detector::DetectorFactory( Detector::Type::clover )
    , crystalFactory( have_HPGe ? new HPGeFactory : nullptr )
    , shieldFactory( have_Shield ? new ShieldFactory : nullptr )
{}

void CloverFactory::SetParameters(const Parameters *param)
{
    auto params = reinterpret_cast<const CloverParameters *>(param);

    if ( params->have_HPGe ){
        if ( !crystalFactory )
            crystalFactory = new HPGeFactory;
    }

    if ( params->have_Shield ){
        if ( !shieldFactory )
            shieldFactory = new ShieldFactory;
    }
}

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