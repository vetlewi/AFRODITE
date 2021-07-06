//
// Created by Vetle Wegner Ingeberg on 19/03/2021.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <G4Types.hh>
#include <G4RotationMatrix.hh>
#include <G4Transform3D.hh>
#include <G4ThreeVector.hh>

//////////////////////////////////////////////////////////
///                 DETECTOR ARRAY SETUP               ///
//////////////////////////////////////////////////////////

///////////////     CLOVER DETECTORS     ///////////////////
constexpr G4int     numberOf_CLOVER = 8;
constexpr G4int     numberOf_CLOVER_Crystals = 4;
constexpr G4int     numberOf_CLOVER_Shields = numberOf_CLOVER;
constexpr G4int     numberOf_BGO_Crystals = 16;

constexpr G4int     numberOfSi = 2;

///////////////     OCL LaBr3            ///////////////////
constexpr G4int     numberOf_OCLLaBr3 = 6;

///////////////     FTA LaBr3            ///////////////////
constexpr G4int     numberOf_FTALaBr3 = 6;

///////////////     Si detectrs            ///////////////////
constexpr G4int     numberOf_SiRings = 48;
constexpr G4int     numberOf_SiSectors = 16;

constexpr G4double  ring_constant = 0.5*23.06/0.491;

#endif // CONSTANTS_H
