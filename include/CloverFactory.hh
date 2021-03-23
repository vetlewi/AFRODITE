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

struct Volumes_t {
    G4LogicalVolume *logicalVolume;
    G4VPhysicalVolume *physicalVolume;

    Volumes_t() : logicalVolume( nullptr ), physicalVolume( nullptr ){}
};

struct HPGeDetector {
    Volumes_t InternalVacuum;
    Volumes_t Encasement;
    Volumes_t HPGeCrystals[numberOf_CLOVER_Crystals];
    Volumes_t HPGeContacts[numberOf_CLOVER_Crystals];
    HPGeDetector() = default;
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

    void SetupLogical(HPGeDetector *detector) const;
    void SetupVis(HPGeDetector *detector) const;
    void SetupPhysical(HPGeDetector *detector, G4LogicalVolume *parent, const G4ThreeVector &pos,
                       const G4RotationMatrix &rot, const int &copy_no, const bool &overlap) const;

public:

    HPGeFactory(const char *vacuum_path, const char *encasement_path,
                const char *crystals_path[], const char *contacts_path[]);

    HPGeDetector Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                           const int &copy_no, const bool &overlap) const;


};

struct ShieldDetector {
    Volumes_t Body;
    Volumes_t Heavimet;
    Volumes_t PMTConArray;
    Volumes_t BGOCrystals[numberOf_BGO_Crystals];
    Volumes_t PMT[numberOf_BGO_Crystals];

    ShieldDetector() = default;
};

class ShieldFactory {

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

    void SetupLogical(ShieldDetector *detector) const;
    void SetupVis(ShieldDetector *detector) const;
    void SetupPhysical(ShieldDetector *detector, G4LogicalVolume *parent, const G4ThreeVector &pos,
                       const G4RotationMatrix &rot, const int &copy_no, const bool &overlap) const;

public:

    ShieldFactory(const char *body_path, const char *heavimet_path, const char *PMTConArray_path,
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
    ShieldFactory *shieldFactory;

public:

    CloverFactory(const bool &shield = false);

    ~CloverFactory();


    CloverDetector Construct(G4LogicalVolume *parent, const G4ThreeVector &pos, const G4RotationMatrix &rot,
                             const int &copy_no, const bool &overlap, const bool &HPGe_present,
                             const bool &shield_present) const;


};

#endif // CLOVERFACTORY_HH
