//
// Created by Vetle Wegner Ingeberg on 26/03/2021.
//

#ifndef S2FACTORY_HH
#define S2FACTORY_HH

#include "detector/DetectorFactory.hh"
#include <G4Types.hh>

class G4Tubs;
class G4Box;
class G4Material;
class G4VSolid;
class G4VPhysicalVolume;
class G4LogicalVolume;

namespace Detector {

    class S2Factory : public Detector::DetectorFactory
    {
    public:
        explicit S2Factory(const G4double &thickness = 0.5);

        ~S2Factory() override = default;

        G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;

        S2Factory GetFactory(const G4double &_thickness)
        {
            if ( thickness == _thickness )
                return *this;
            else
                return S2Factory(_thickness);
        }

    private:

        G4double thickness;

        G4Material *fMatPCB;
        G4Material *fMatSi;
        G4Material *fMatVaccum;

        G4VSolid *PCB_Solid;
        G4VSolid *PCB_vaccum;

        G4Tubs *Inactive_area_inner_Solid;
        G4Tubs *Inactive_area_outer_Solid;
        G4Tubs *Active_area_solid;

    };

}

#endif // S2FACTORY_HH
