//
// Created by Vetle Wegner Ingeberg on 19/03/2021.
//

#ifndef AFRODITE_LABR3_H
#define AFRODITE_LABR3_H

class G4Material;
class G4Element;

class LaBr3Base
{
protected:

    // Common definitions
    static G4Element* Br;
    static G4Element* La;
    static G4Element* Ce;
    static G4Material* LaBr3;
    static G4Material* LaBr3_Ce;
    static G4Material* vacuum;

    // Defined in FTA LaBr3 detectors
    static G4Material* fMatLaBr3Housing;
    static G4Material* fMatLaBr3Interior;
    static G4Material* fMatLaBr3Reflector;
    static G4Material* fMatLaBr3Crystal;
    static G4Material* fMatLaBr3LightGuide;
    static G4Material* fMatLaBr3PMT;
    static G4Material* fMatLaBr3PMTInterior;


    // Materials defined in OCL LaBr3 detectors
    static G4Element* O;
    static G4Element* K;
    static G4Element* Sb;
    static G4Element* Cs;
    static G4Element* Mg;

    static G4Material* Aluminium;
    static G4Material* PlexiGlass;

    static G4Material* Sili;
    static G4Material* SiO2;
    static G4Material* Na2O ;
    static G4Material* K2O ;
    static G4Material* Al2O3;
    G4Material* B2O3;

    G4Material* MgO;

    G4Material* Borosilicate;
    G4Material* Bialkali;



};

#endif //AFRODITE_LABR3_H
