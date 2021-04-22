//
// Created by Vetle Wegner Ingeberg on 26/03/2021.
//

#ifndef S2FACTORY_HH
#define S2FACTORY_HH

#include <G4Types.hh>

class G4Tubs;
class G4Box;
class G4AssemblyVolume;
class G4Material;
class G4VSolid;
class G4VPhysicalVolume;
class G4LogicalVolume;

constexpr int numberOfSectors = 16;
constexpr int numberOfRings = 48;

class S2Factory
{
public:

    S2Factory(const G4double &thickness);
    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap);

private:

    G4Material *fMatPCB;
    G4Material *fMatSi;
    G4Material *fMatVaccum;

    G4VSolid *PCB_Solid;
    G4VSolid *PCB_vaccum;

    G4Tubs *Inactive_area_inner_Solid;
    G4Tubs *Inactive_area_outer_Solid;
    G4Tubs *Active_area_solid;
//    G4Tubs *Active_area_Solid[numberOfSectors][numberOfRings];

};

#endif // S2FACTORY_HH
