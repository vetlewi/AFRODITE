#ifndef FTALABR3_HH
#define FTALABR3_HH

/*!
 * Sizes and measurements of the FTA LaBr detector is based on https://github.com/AndreiHanu/G4McMasterTDS
 */

#include "detector/DetectorFactory.hh"

class G4Element;
class G4Material;
class G4Tubs;
class G4VSolid;
class G4AssemblyVolume;

namespace Detector {

    class FTALaBr3 : public Detector::DetectorFactory
    {
    public:

        FTALaBr3();

        ~FTALaBr3() override = default;

        G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;

    private:
        G4Material *fMatLaBr3Vacuum;
        G4Material *fMatLaBr3Housing;
        G4Material *fMatLaBr3Reflector;
        G4Material *fMatLaBr3Crystal;
        G4Material *fMatLaBr3LightGuide;
        G4Material *fMatLaBr3PMTWindow;
        G4Material *fMatLaBr3PMTCathode;

        G4VSolid *Housing_Solid;
        G4VSolid *CrystalHouse_Solid;
        G4Tubs *Reflector_Solid;
        G4Tubs *Crystal_Solid;
        G4Tubs *PlexiWindow_Solid;
        G4VSolid *PMTHouse_Solid;
        G4Tubs *PMT_Solid;
        G4Tubs *PMT_Vaccum_Solid;
        G4Tubs *PMTCathode_Solid;
    };

}

#endif // FTALABR3_HH