#ifndef FTALABR3_HH
#define FTALABR3_HH

/*!
 * Sizes and measurements of the FTA LaBr detector is based on https://github.com/AndreiHanu/G4McMasterTDS
 */

class G4OpticalSurface;
class G4Element;
class G4Material;
class G4MaterialPropertiesTable;
class G4VPhysicalVolume;
class G4Tubs;
class G4LogicalVolume;
class G4LogicalBorderSurface;
class G4VisAttributes;
class G4Cons;
class G4VSolid;
class G4UnionSolid;


#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4Types.hh>



class FTALaBr3
{
public:
    FTALaBr3(const G4int &detectorID, const bool &checkOverlaps = false);
    ~FTALaBr3();

public:
    void SetPosition( G4ThreeVector );
    void SetRotation( G4RotationMatrix );
    void Placement(G4int, G4VPhysicalVolume*, G4bool);

    G4LogicalVolume *GetLogical(){ return LaBr3_Housing_Logical; }
    G4LogicalVolume *GetSensitive(){ return LaBr3_Crystal_Logical; }

private:
    G4ThreeVector        translatePos;
    G4RotationMatrix     rotation;

    G4int detectorID;

    G4VSolid *LaBr3_Housing_Solid;

    G4LogicalVolume* LaBr3_Housing_Logical;
    G4LogicalVolume* LaBr3_Interior_Logical;
    G4VPhysicalVolume *LaBr3_Interior_Physical;
    G4LogicalVolume* LaBr3_Reflector_Logical;
    G4VPhysicalVolume *LaBr3_Reflector_Physical;
    G4LogicalVolume* LaBr3_Crystal_Logical;
    G4VPhysicalVolume *LaBr3_Crystal_Physical;
    G4LogicalVolume* LaBr3_LightGuide_Logical;
    G4VPhysicalVolume *LaBr3_LightGuide_Physical;
    G4LogicalVolume* LaBr3_PMT_Logical;
    G4VPhysicalVolume *LaBr3_PMT_Physical;
    G4LogicalVolume* LaBr3_PMTInterior_Logical;
    G4VPhysicalVolume *LaBr3_PMTInterior_Physical;

    G4Material* fMatLaBr3Housing;
    G4Material* fMatLaBr3Interior;
    G4Material* fMatLaBr3Reflector;
    G4Material* fMatLaBr3Crystal;
    G4Material* fMatLaBr3LightGuide;
    G4Material* fMatLaBr3PMT;
    G4Material* fMatLaBr3PMTInterior;

    //
    // Elements & Materials
    //

    G4Element* Br;
    G4Element* La;
    G4Element* Ce;

    G4Material* LaBr3;
    G4Material* LaBr3_Ce;
    G4Material* vacuum;


private:
    void CreateSolids(const bool &checkOverlaps);
    // void MakeMaterials();

};

#endif // FTALABR3_HH