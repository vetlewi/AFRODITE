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

    struct FTAParameters : public Parameters
    {
        // Currently we haven't implemented anything yet.
        // Everything atm are hard coded.
        // TODO: Move all parameters to this object

        double HousingHalfHeight;
        double HousingRadius;

        double CrystalHousingRadius;
        double CrystalHousingHalfThickness;
        double CrystalHousingHalfHeight;

        double CrystalRadius;
        double CrystalHalfHeight;

        [[nodiscard]] double ReflectorThickness() const
            { return CrystalHousingRadius - 2*CrystalHousingHalfThickness - CrystalRadius; }

        [[nodiscard]] double ReflectorHalfThickness() const
            { return ReflectorThickness()/2.; }

        [[nodiscard]] double ReflectorRadius() const
            { return CrystalRadius + ReflectorThickness(); }

        [[nodiscard]] double ReflectorHalfHeight() const
            { return CrystalHalfHeight + ReflectorHalfThickness(); }

        [[nodiscard]] double PlexiWindowRadius() const { return ReflectorRadius(); }
        double PlexiWindowHalfHeight;

        double PMTHouseThickness;
        [[nodiscard]] double PMTHouseOuterRadius() const { return HousingRadius; }
        [[nodiscard]] double PMTHouseInnerRadius() const { return PMTHouseOuterRadius() - PMTHouseThickness; }
        [[nodiscard]] double PMTHouseHalfHeight() const { return HousingHalfHeight - CrystalHousingHalfHeight; }

        double PMTWindowThickness;
        double PMTHalfHeight;
        double PMTRadius;
        [[nodiscard]] double PMTVacuumRadius() const { return PMTRadius - PMTWindowThickness; }
        [[nodiscard]] double PMTVacuumHalfHeight() const { return PMTHalfHeight - PMTWindowThickness; }

        double PMTCathodeRadius;
        double PMTCathodeHalfHeight;

        FTAParameters();
        FTAParameters(const FTAParameters &param);
    };

    class FTALaBr3 : public Detector::DetectorFactory
    {
    public:

        explicit FTALaBr3(const FTAParameters *params = nullptr);

        ~FTALaBr3() override = default;

        G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;

    private:
        FTAParameters parameters;

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