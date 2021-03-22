//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#ifndef CLOVERFACTORY_HH
#define CLOVERFACTORY_HH

#include <string>

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

#include "Constants.hh"

class CADMesh;
class G4VSolid;
class G4LogicalVolume;

struct Solids_t {
    CADMesh *mesh;
    G4VSolid *solid;

    Solids_t(const char *path, const G4ThreeVector &offset);
};

struct HPGeDetector {
    G4LogicalVolume *Logic_InternalVacuum;
    G4PVPlacement *PhysInternalVacuum;
    G4PVPlacement *PhysEncasement;
    G4PVPlacement *PhysHPGeCrystal[numberOf_CLOVER_Crystals];
    G4PVPlacement *PhysHPGEContact[numberOf_CLOVER_Crystals];

    HPGeDetector()
        : Logic_InternalVacuum( nullptr )
        , PhysInternalVacuum( nullptr )
        , PhysEncasement( nullptr )
        , PhysHPGeCrystal{ nullptr }
        , PhysHPGEContact{ nullptr }{}
};

class HPGeFactory {

private:

    Solids_t internal_vacuum;
    Solids_t encasement;
    Solids_t crystals[numberOf_CLOVER_Crystals];
    Solids_t contacts[numberOf_CLOVER_Crystals];

    static G4ThreeVector offset_Vacuum;
    static G4ThreeVector offset_Encasement;
    static G4ThreeVector offset_Crystals[numberOf_CLOVER_Crystals];

    G4LogicalVolume * Logic_CLOVER_Encasement;
    G4LogicalVolume * Logic_CLOVER_HPGeCrystal[4];
    G4LogicalVolume * Logic_CLOVER_HPGeCrystal_LithiumContact[4];

public:

    HPGeFactory(const char *vacuum_path, const char *encasement_path,
                const char *crystals_path[], const char *contacts_path[]);

    HPGeDetector Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                           const int &copy_no, const bool &overlap) const;


};

struct ShieldDetector {
    G4PVPlacement *PhysBody;
    G4PVPlacement *PhysHeavimet;
    G4PVPlacement *PhysPMTConArray;
    G4PVPlacement *PhysBGOCrystal[numberOf_BGO_Crystals];
    G4PVPlacement *PhysPMT[numberOf_BGO_Crystals];

    static ShieldDetector getZero();
};

class CloverShieldFactory {

private:

    Solids_t body;
    Solids_t heavimet;
    Solids_t PMTConArray;
    Solids_t BGOCrystal[numberOf_BGO_Crystals];
    Solids_t PMT[numberOf_BGO_Crystals];

    static G4ThreeVector offset_body;
    static G4ThreeVector offset_heavimet;
    static G4ThreeVector offset_PMTConArray;
    static G4ThreeVector offset_BGOCrystals;
    static G4ThreeVector offset_PMT;

    G4LogicalVolume* Logic_Shield_Body;
    G4LogicalVolume* Logic_Shield_Heavimet;
    G4LogicalVolume* Logic_Shield_PMTConArray;
    G4LogicalVolume* Logic_Shield_BGOCrystal[numberOf_BGO_Crystals];
    G4LogicalVolume* Logic_Shield_PMT[numberOf_BGO_Crystals];

public:

    CloverShieldFactory(const char *body_path, const char *heavimet_path, const char *PMTConArray_path,
                        const char *BGOCrystal_path[], const char *PMT_path[]);

    ShieldDetector Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                             const int &copy_no, const bool &overlap) const;

};

struct CloverDetector {
    HPGeDetector HPGe;
    ShieldDetector Shield;
};

class CloverFactory {

private:
    bool shieldConstruct;

    HPGeFactory *crystalFactory;
    CloverShieldFactory *shieldFactory;

public:

    CloverFactory(const bool &shield = false);

    ~CloverFactory();


    CloverDetector Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                             const int &copy_no, const bool &overlap, const bool &HPGe_present,
                             const bool &shield_present) const;


};

#endif // CLOVERFACTORY_HH
