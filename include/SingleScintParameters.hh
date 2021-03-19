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

#include "G4SystemOfUnits.hh"



    //
    // World
    //

namespace OCL {

    constexpr G4double world_sizeXYZ = 90 * cm;
    //
    // Detector & Shielding
    //

    constexpr G4double crystalOuterR = 8.89 * cm / 2.; // 3.5 in in cm
    constexpr G4double crystalInnerR = 0.0 * mm;
    constexpr G4double crystalHalfLength = 203.2 * 0.5 * mm; // 8 in in cm
    constexpr G4double startPhi = 0. * deg;
    constexpr G4double deltaPhi = 360. * deg;

    constexpr G4double reflectorThickness = 1. * mm; // assumption: 1 mm thick reflector on the front side
    constexpr G4double reflectorHalfLength =
            crystalHalfLength + 0.5 * reflectorThickness; // assumption: backside doesn't have a reflector
    //const G4double ReflectorInnerR = crystalOuterR;
    constexpr G4double reflectorInnerR = 0. * mm;
    constexpr G4double reflectorOuterR = crystalOuterR + reflectorThickness;

    constexpr G4double coatingThickness = 2. * mm; // thickness as in the radius part
    constexpr G4double coatingThicknessFront = 1. * mm; // we assume a smaller thickness at the front of the detector
    constexpr G4double coatingOuterR = 100. * mm / 2.;
    // in between reflector and coating, there will be some plastic
    constexpr G4double coatingPlasticThickness = coatingOuterR - coatingThickness - reflectorThickness -
                                             crystalOuterR; // assumption: 2.55 mm plexiglas coating around the reflector before the aluminium
    constexpr G4double coatingHalfLength = reflectorHalfLength + 0.5 * coatingThicknessFront +
                                       0.5 * coatingPlasticThickness; // backside doesn't have an (Aluminium) coating

    constexpr G4double shieldingThickness = 5. * mm;        // thickness of the tube
    constexpr G4double shieldingHalfThicknessLid = 2. * mm / 2.;
    constexpr G4double shieldingInnerR = 0 * mm;            // as we use it as a mother volume
    constexpr G4double shieldingOuterR = coatingOuterR + shieldingThickness;

    //in the front, the shielding tube diameter is reduces. It's later modeled by a conical section
    constexpr G4double shieldingConeHalfLength = 10. * mm;// in the front, the tube
    //const G4double shieldingConeInnerRFront = coatingOuterR;
    constexpr G4double shieldingConeOuterRFront = coatingOuterR + 2. * mm;
    //const G4double shieldingConeInnerRBack = shieldingConeInnerRFront;
    constexpr G4double shieldingConeOuterRBack = coatingOuterR + 5. * mm;

    constexpr G4double shieldingHalfLength = coatingHalfLength - shieldingConeHalfLength; // without conical Section and Lid
    //  we assume no coating at the back side

    constexpr G4double plexiGlasWindowOuterR = shieldingOuterR; // currently we just assume a flat window on the top.
    constexpr G4double plexiGlasWindowHalfLength = 0.5 * 1. * mm;


    //
    // PMT
    //

    constexpr G4double PMTWindowHalfLength = 1.0 * mm;
    constexpr G4double PMTWindowRadius = 85 * 0.5 * mm;

    constexpr G4double cathodeHalfLength = 0.005 * mm;
    constexpr G4double cathodeRadius = 85 * 0.5 * mm;


    //
    // Whole detector incl. PMT (-> Logical unit)
    //

    constexpr G4double detectorHalfinclPMT = shieldingHalfThicknessLid + shieldingConeHalfLength + shieldingHalfLength
                                         + plexiGlasWindowHalfLength + PMTWindowHalfLength + cathodeHalfLength;



    //
    // Collimator and Source
    //

    constexpr G4double collimatorHalfLength = 1. * cm; // adapt here for different collimator lengths

    // when you change the Collimator length and distance to Source, check that it's still inside the World Volume!
    //  Keeping the sum of distSourceCol and 2*collimatorHalfLength >= 20 cm.
    constexpr G4double distSourceCol = 18. * cm;        // Distance from source to Collimator (beginning)


    // Distance from collimator Half point to Crystal Half point (or fraction r in crystal length)
    constexpr G4double distHalfColHalfCry = collimatorHalfLength + 2. * shieldingHalfThicknessLid + coatingThicknessFront
                                        + coatingPlasticThickness + reflectorThickness + crystalHalfLength;
    constexpr G4double distSourceHalfCry = distSourceCol + 2 * collimatorHalfLength + distHalfColHalfCry;

    constexpr G4double ratioInCrystal = 0.5;          // range: [0..1], defines point r from where the gammas can hit the crystal
    constexpr G4double distColEndPointToRatioCrsytal =
            distHalfColHalfCry - collimatorHalfLength + (2 * ratioInCrystal - 1.) * crystalHalfLength;

    //parameters for collimator as a cone as a function of the parameters above
    // 1 is the front (towards source), 2 the backside
    constexpr G4double colRmin1 = crystalOuterR * (distSourceCol / (distSourceCol + 2. * collimatorHalfLength +
                                                                distColEndPointToRatioCrsytal));
    constexpr G4double colRmax1 = crystalOuterR * (distSourceCol / (distSourceCol + 2. * collimatorHalfLength));
    constexpr G4double colRmin2 = crystalOuterR * (distSourceCol + 2 * collimatorHalfLength)
                              / (distSourceCol + 2 * collimatorHalfLength + distColEndPointToRatioCrsytal);
    constexpr G4double colRmax2 = shieldingConeOuterRFront;

}

namespace FTA {

    constexpr G4double shieldingThickness = 0.5 * mm;        // thickness of the tube

}

#endif /* PARAMETERS__HH_ */
