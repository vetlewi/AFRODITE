//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#ifndef CLOVERFACTORY_HH
#define CLOVERFACTORY_HH

#include <string>

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

#include "Constants.hh"
#include <future>

class CADMesh;
class G4VSolid;
class G4LogicalVolume;
class G4AssemblyVolume;
class G4Material;
class G4MultiUnion;

struct Solids_t {
    CADMesh *mesh;
    std::future<G4VSolid *> solid_future;
    G4VSolid *solid;
    Solids_t(const char *path, const G4ThreeVector &offset);
    void await();
    inline G4VSolid *GetSolid()
    {
        if ( !solid )
            await();
        return solid;
    };
};

class HPGeFactory {

private:

    G4Material *fMatVaccum;
    G4Material *fMatAluminium;
    G4Material *fMatGe;

    Solids_t internal_vacuum;
    Solids_t encasement;
    Solids_t crystals[numberOf_CLOVER_Crystals];
    Solids_t contacts[numberOf_CLOVER_Crystals];

    static G4ThreeVector offset_Vacuum;
    static G4ThreeVector offset_Encasement;
    static G4ThreeVector offset_Crystals[numberOf_CLOVER_Crystals];

public:

    HPGeFactory(const char *vacuum_path, const char *encasement_path,
                const char *crystals_path[], const char *contacts_path[]);

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &overlap);

};

class ShieldFactory {

public:

    ShieldFactory(const char *body_path, const char *heavimet_path, const char *PMTConArray_path,
                  const char *BGOCrystal_path[], const char *PMT_path[]);

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &overlap);

private:

    G4Material *fMatVaccum;
    G4Material *fMatAluminium;
    G4Material *fMatHeavimet;
    G4Material *fMatBGO;

    Solids_t body;
    Solids_t heavimet;
    Solids_t PMTConArray;
    Solids_t BGOCrystal[numberOf_BGO_Crystals];
    Solids_t PMT[numberOf_BGO_Crystals];
    G4MultiUnion *BGOVolume;

    static G4ThreeVector offset_body;
    static G4ThreeVector offset_heavimet;
    static G4ThreeVector offset_PMTConArray;
    static G4ThreeVector offset_BGOCrystals;
    static G4ThreeVector offset_PMT;

};

class CloverFactory {

public:

    CloverFactory(const bool &have_HPGe = true, const bool &have_Shield = true);

    ~CloverFactory();

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap);

private:
    HPGeFactory *crystalFactory;
    ShieldFactory *shieldFactory;



};

#endif // CLOVERFACTORY_HH
