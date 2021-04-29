#ifndef OCLLABR3_H
#define OCLLABR3_H 1

#include "Detectors/DetectorFactory.hh"

class G4Tubs;
class G4Cons;
class G4VSolid;
class G4Polycone;
class G4MultiUnion;
class G4Material;



class OCLLaBr3 : public Detector::DetectorFactory {

public:
    explicit OCLLaBr3(const bool &new_shield_design = true);
    ~OCLLaBr3() override = default;

    G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;


private:

    // Materials for the different components
    G4Material *fMatAir;
    G4Material *fMatAluminium;
    G4Material *fMatPlexiGlass;
    G4Material *fMatMgO;
    G4Material *fMatLaBr3_Ce;
    G4Material *fMatSiO2;
    G4Material *fMatBialkali;

    // Parts for the housing of the detector
    G4Tubs *Detector_Solid;
    G4VSolid *Shielding_Solid;
    G4Tubs *Shields_Tube_Solids[3];
    G4MultiUnion *Union_Shielding_Solid;

    // Parts for the crystal
    G4Tubs *Coating_Solid;
    G4Tubs *PlexiGlas_Coating_Solid;
    G4Tubs *Reflector_Solid;
    G4Tubs *Crystal_Solid;
    G4Tubs *Crystal_Window_Solid;

    // PMT
    G4Tubs *PMT_Window_Solid;
    G4Tubs *Cathode_Solid;
    G4Polycone *PMT_Body_Solid;

};

#endif
