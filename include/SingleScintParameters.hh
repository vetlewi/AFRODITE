/*
 * Parameters.hh
 *
 * Includes all the constants used in the creation of the
 * DetectorGeometry class
 * and in the source position in PrimaryActionGenerator class
 *
 *  Created on: Oct 22, 2015
 *      Author: fabiobz
 */

#ifndef SingleScintPARAMETERS__HH_
#define SingleScintPARAMETERS__HH_

#include <G4SystemOfUnits.hh>
#include <G4Types.hh>



    //
    // World
    //

namespace OCL {

    // Common constants
    constexpr G4double startPhi = 0.*deg;
    constexpr G4double deltaPhi = 360.*deg;

    // Crystal measurements
    constexpr G4double crystalOuterR = 8.89*cm/2.; // 3.5 in in cm
    constexpr G4double crystalInnerR = 0.0*mm;
    constexpr G4double crystalHalfLength = 203.2*mm/2.; // 8 in in cm

    // Coating
    constexpr G4double coatingOuterR = 100.*mm/2. ;
    constexpr G4double plexiGlasWindowHalfLength = 1.*mm/2.;
    constexpr G4double coatingAlThickness = 0.8*mm;
    constexpr G4double coatingAlThicknessFront = 0.8*mm;

    // Reflector
    constexpr G4double reflectorThickness = 2.2*mm;
    constexpr G4double reflectorHalfLength = crystalHalfLength + 0.5 * reflectorThickness; // assumption: backside doesn't have a reflector
    constexpr G4double reflectorInnerR = 0.*mm;
    constexpr G4double reflectorOuterR = crystalOuterR + reflectorThickness;
    constexpr G4double coatingPlasticThickness = coatingOuterR - coatingAlThickness -reflectorThickness - crystalOuterR;
    constexpr G4double coatingHalfLength = reflectorHalfLength + 0.5 * coatingAlThicknessFront + 0.5 * coatingPlasticThickness; // backside doesn't have an (Aluminium) coating

    // Shielding
    constexpr G4double defaultshieldingHalfThicknessLid = 2.*mm/2.;
    constexpr G4double shieldingHalfThicknessLid = defaultshieldingHalfThicknessLid;
    constexpr G4double shieldingConeOuterRFront = coatingOuterR + 3.*mm;
    constexpr G4double deltaShieldingHalfThicknessLid = shieldingHalfThicknessLid - defaultshieldingHalfThicknessLid;
    constexpr G4double shieldingConeHalfLength = 27./2.*mm + deltaShieldingHalfThicknessLid;
    constexpr G4double shieldingThickness1 = 5.*mm;         // thickness of the 1. tube
    constexpr G4double shieldingThickness2 = 30*mm;
    constexpr G4double shieldingThickness3 = 10*mm;

    constexpr G4double colimatorLength = 70.*mm;

    constexpr G4double shieldingLength1 = (110.74*mm + 114.26*mm) - colimatorLength + 2*deltaShieldingHalfThicknessLid;
    constexpr G4double shieldingLength2 = 49*mm;
    constexpr G4double shieldingLength3 = 415*mm + 2*deltaShieldingHalfThicknessLid
                                -( shieldingLength1 + shieldingLength2);
    constexpr G4double dShieldTubs[] = {shieldingLength1, shieldingLength2, shieldingLength3};

    constexpr G4double shieldingInnerR = 0*mm; 			// as we use it as a mother volume
    constexpr G4double shieldingOuterR1 = coatingOuterR + shieldingThickness1;
    constexpr G4double shieldingOuterR2 = coatingOuterR + shieldingThickness2;
    constexpr G4double shieldingOuterR3 = coatingOuterR + shieldingThickness3;
    constexpr G4double ShieldrOuters[] = {shieldingOuterR1, shieldingOuterR2, shieldingOuterR3};
    constexpr G4double shieldingConeOuterRBack = coatingOuterR + shieldingThickness1;

    // PlexiGlas window
    constexpr G4double plexiGlasWindowOuterR = coatingOuterR;

    // PMT
    constexpr G4double PMTWindowHalfLength = 1.0*mm;
    constexpr G4double PMTWindowRadius = 85*0.5*mm;
    constexpr G4double cathodeHalfLength = 0.005*mm;
    constexpr G4double cathodeRadius =85*0.5*mm;
    constexpr G4double PMTStartRadius = cathodeRadius;
    constexpr G4double PMTMidtRadius = 51.5*mm/2.;
    constexpr G4double PMTEndRadius = 56.5*mm/2.;
    constexpr G4double PMTMidtZ = 7.*cm;
    constexpr G4double PMTEndZ = PMTMidtZ+3.*cm;
    constexpr G4double PMTHalfLength = PMTEndZ/2.;
    constexpr G4double detectorHalfinclPMT = shieldingConeHalfLength
                + shieldingLength1/2. +shieldingLength2/2. + shieldingLength3/2.;
    constexpr G4double PMTandAirHalfLength = detectorHalfinclPMT - shieldingHalfThicknessLid - coatingHalfLength
                                             - plexiGlasWindowHalfLength;
    constexpr G4double zPlane[3] = {0-PMTHalfLength, PMTMidtZ - PMTHalfLength, PMTEndZ - PMTHalfLength};
    constexpr G4double rInners[3] = {0};
    constexpr G4double rOuters[3] = {PMTStartRadius, PMTMidtRadius, PMTEndRadius};

}

#endif /* PARAMETERS__HH_ */
