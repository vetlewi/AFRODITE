//
// Created by Vetle Wegner Ingeberg on 30/04/2021.
//

#include "meshreader/incbin.h"

#ifndef SRC_PATH
#define SRC_PATH "../"
#endif // SRC_PATH

#ifndef PLY_PATH
#define PLY_PATH SRC_PATH"Mesh-Models"
#endif // PLY_PATH

#ifndef DETECTOR_PATH
#define DETECTOR_PATH PLY_PATH"/DETECTORS"
#endif // DETECTOR_PATH

#ifndef CLOVER_PATH
#define CLOVER_PATH DETECTOR_PATH"/CLOVER"
#endif // CLOVER_PATH

// Including all the CLOVER HPGe mesh models
INCBIN(CLOVER_VACCUM, CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply");
INCBIN(CLOVER_ENCASEMENT, CLOVER_PATH"/CloverEncasement/CloverEncasement_new_approx.ply");
INCBIN(HPGeCrystalA, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply");
INCBIN(HPGeCrystalB, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal2_10umTolerance.ply");
INCBIN(HPGeCrystalC, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal3_10umTolerance.ply");
INCBIN(HPGeCrystalD, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal4_10umTolerance.ply");
INCBIN(HPGeContactA, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact1_10um.ply");
INCBIN(HPGeContactB, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact2_10um.ply");
INCBIN(HPGeContactC, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact3_10um.ply");
INCBIN(HPGeContactD, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact4_10um.ply");

