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

///////////////     OCL LaBr3            ///////////////////
constexpr G4int     numberOf_OCLLaBr3 = 2;

///////////////     FTA LaBr3            ///////////////////
constexpr G4int     numberOf_FTALaBr3 = 6;


//////////////////////////////////////////////////////////////////////////
//                          OPERATION MODES
//////////////////////////////////////////////////////////////////////////

///////////////     CLOVER Detectors - PIXIE16 Sampling     ///////////////////
const G4double      CLOVER_SamplingTime = 10; // ns
const G4int         CLOVER_TotalTimeSamples = 10; //
const G4double      CLOVER_TotalSampledTime = CLOVER_SamplingTime * CLOVER_TotalTimeSamples; // ns
const G4int         CLOVER_ComptonSupression_TimeWindow = 3; // Amount of CLOVER Time Samples


///////////////     CLOVER BGO Anti-Compton Shield - PIXIE16 Sampling    ///////////////////
const G4double      CLOVER_Shield_BGO_SamplingTime = CLOVER_SamplingTime; // ns
const G4int         CLOVER_Shield_BGO_TotalTimeSamples = CLOVER_TotalTimeSamples + CLOVER_ComptonSupression_TimeWindow; //
const G4double      CLOVER_Shield_BGO_TotalSampledTime = CLOVER_Shield_BGO_SamplingTime * CLOVER_Shield_BGO_TotalTimeSamples; // ns

///////////////     OCL LaBr3 Detectors - PIXIE16 Sampling     ///////////////////
const G4double      OCLLABR_SamplingTime = 10; // ns
const G4int         OCLLABR_TotalTimeSamples = 10; //
const G4double      OCLLABR_TotalSampledTime = OCLLABR_SamplingTime * OCLLABR_TotalTimeSamples; // ns

///////////////     LaBr3 Detectors - PIXIE16 Sampling     ///////////////////
const G4double      FTALABR_SamplingTime = 10; // ns
const G4int         FTALABR_TotalTimeSamples = 10; //
const G4double      FTALABR_TotalSampledTime = FTALABR_SamplingTime * FTALABR_TotalTimeSamples; // ns





#endif // CONSTANTS_H
