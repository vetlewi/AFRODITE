#ifndef FTALABR3_HH
#define FTALABR3_HH

/*!
 * Sizes and measurements of the FTA LaBr detector is based on https://github.com/AndreiHanu/G4McMasterTDS
 */

class G4Element;
class G4Material;
class G4Tubs;
class G4AssemblyVolume;


class FTALaBr3
{
public:

    FTALaBr3();
    ~FTALaBr3() = default;

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap);

private:

    G4Material* fMatLaBr3Housing;
    G4Material* fMatLaBr3Reflector;
    G4Material* fMatLaBr3Crystal;
    G4Material* fMatLaBr3LightGuide;
    G4Material* fMatLaBr3PMTWindow;

    G4Tubs *CrystalHouse_Solid;
    G4Tubs *PMTHouse_Solid;
    G4Tubs *PMTTop_Solid;
    G4Tubs *Reflector_Solid;
    G4Tubs *Crystal_Solid;
    G4Tubs *PlexiWindow_Solid;
    G4Tubs *PMTWindow_Solid;

};

#endif // FTALABR3_HH