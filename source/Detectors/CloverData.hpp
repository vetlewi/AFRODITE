//
// Created by Vetle Wegner Ingeberg on 03/05/2021.
//

#ifndef BUILDALL_CLOVERDATA_HPP
#define BUILDALL_CLOVERDATA_HPP

#define INCBIN_PREFIX g_
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <meshreader/incbin.h>

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

#define DATA_NAME(NAME) \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
        INCBIN_STYLE_IDENT(DATA))

#define SIZE_NAME(NAME) \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
        INCBIN_STYLE_IDENT(SIZE))
#define CONVERT_TO_STRUCT(NAME) \
        { \
        DATA_NAME(NAME), \
        SIZE_NAME(NAME)}

#define INCNS(NAMESPACE, NAME, PATH) \
    INCBIN(NAME, PATH); \
    namespace NAMESPACE { static const MeshReader::MemBuf_t NAME = CONVERT_TO_STRUCT(NAME); }

// Including all the CLOVER HPGe mesh models
INCNS(HPGe_Components, vaccum, CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply");
INCNS(HPGe_Components, encasement, CLOVER_PATH"/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply");
INCBIN(HPGeCrystalA, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply");
INCBIN(HPGeCrystalB, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal2_10umTolerance.ply");
INCBIN(HPGeCrystalC, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal3_10umTolerance.ply");
INCBIN(HPGeCrystalD, CLOVER_PATH"/HPGeCrystals/HPGe-RoundedCrystal4_10umTolerance.ply");
INCBIN(HPGeContactA, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact1_10um.ply");
INCBIN(HPGeContactB, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact2_10um.ply");
INCBIN(HPGeContactC, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact3_10um.ply");
INCBIN(HPGeContactD, CLOVER_PATH"/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact4_10um.ply");

namespace HPGe_Components {

    static const MeshReader::MemBuf_t crystals[] = {
            CONVERT_TO_STRUCT(HPGeCrystalA), CONVERT_TO_STRUCT(HPGeCrystalB),
            CONVERT_TO_STRUCT(HPGeCrystalC), CONVERT_TO_STRUCT(HPGeCrystalD)};

    static const MeshReader::MemBuf_t contacts[] = {
            CONVERT_TO_STRUCT(HPGeContactA), CONVERT_TO_STRUCT(HPGeContactB),
            CONVERT_TO_STRUCT(HPGeContactC), CONVERT_TO_STRUCT(HPGeContactD)
    };
}

// Including all the shield components
INCNS(Shield_parts, body, CLOVER_PATH"/Shield/Body/Body_Modified2_tol_10um.ply");
INCNS(Shield_parts, heavimet, CLOVER_PATH"/Shield/Heavimet/HEAVIMET_30mm.ply");
INCNS(Shield_parts, PMTConArray, CLOVER_PATH"/Shield/PMT-Connectors/PMT-ConnecterArray.ply");
INCBIN(BGOCrystal0, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_1.ply");
INCBIN(BGOCrystal1, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_2.ply");
INCBIN(BGOCrystal2, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_3.ply");
INCBIN(BGOCrystal3, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_4.ply");
INCBIN(BGOCrystal4, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_5.ply");
INCBIN(BGOCrystal5, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_6.ply");
INCBIN(BGOCrystal6, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_7.ply");
INCBIN(BGOCrystal7, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_8.ply");
INCBIN(BGOCrystal8, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_9.ply");
INCBIN(BGOCrystal9, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_10.ply");
INCBIN(BGOCrystal10, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_11.ply");
INCBIN(BGOCrystal11, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_12.ply");
INCBIN(BGOCrystal12, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_13.ply");
INCBIN(BGOCrystal13, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_14.ply");
INCBIN(BGOCrystal14, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_15.ply");
INCBIN(BGOCrystal15, CLOVER_PATH"/Shield/BGO-Crystals/BGO-Crystal_Modified_16.ply");
INCBIN(BGOPMT0, CLOVER_PATH"/Shield/PMTs/PMT1.ply"); INCBIN(BGOPMT1, CLOVER_PATH"/Shield/PMTs/PMT2.ply");
INCBIN(BGOPMT2, CLOVER_PATH"/Shield/PMTs/PMT3.ply"); INCBIN(BGOPMT3, CLOVER_PATH"/Shield/PMTs/PMT4.ply");
INCBIN(BGOPMT4, CLOVER_PATH"/Shield/PMTs/PMT5.ply"); INCBIN(BGOPMT5, CLOVER_PATH"/Shield/PMTs/PMT6.ply");
INCBIN(BGOPMT6, CLOVER_PATH"/Shield/PMTs/PMT7.ply"); INCBIN(BGOPMT7, CLOVER_PATH"/Shield/PMTs/PMT8.ply");
INCBIN(BGOPMT8, CLOVER_PATH"/Shield/PMTs/PMT9.ply"); INCBIN(BGOPMT9, CLOVER_PATH"/Shield/PMTs/PMT10.ply");
INCBIN(BGOPMT10, CLOVER_PATH"/Shield/PMTs/PMT11.ply"); INCBIN(BGOPMT11, CLOVER_PATH"/Shield/PMTs/PMT12.ply");
INCBIN(BGOPMT12, CLOVER_PATH"/Shield/PMTs/PMT13.ply"); INCBIN(BGOPMT13, CLOVER_PATH"/Shield/PMTs/PMT14.ply");
INCBIN(BGOPMT14, CLOVER_PATH"/Shield/PMTs/PMT15.ply"); INCBIN(BGOPMT15, CLOVER_PATH"/Shield/PMTs/PMT16.ply");

namespace Shield_parts {
    static const MeshReader::MemBuf_t BGOCrystals[] = {
            CONVERT_TO_STRUCT(BGOCrystal0), CONVERT_TO_STRUCT(BGOCrystal1),
            CONVERT_TO_STRUCT(BGOCrystal2), CONVERT_TO_STRUCT(BGOCrystal3),
            CONVERT_TO_STRUCT(BGOCrystal4), CONVERT_TO_STRUCT(BGOCrystal5),
            CONVERT_TO_STRUCT(BGOCrystal6), CONVERT_TO_STRUCT(BGOCrystal7),
            CONVERT_TO_STRUCT(BGOCrystal8), CONVERT_TO_STRUCT(BGOCrystal9),
            CONVERT_TO_STRUCT(BGOCrystal10), CONVERT_TO_STRUCT(BGOCrystal11),
            CONVERT_TO_STRUCT(BGOCrystal12), CONVERT_TO_STRUCT(BGOCrystal13),
            CONVERT_TO_STRUCT(BGOCrystal14), CONVERT_TO_STRUCT(BGOCrystal15)
    };

    static const MeshReader::MemBuf_t BGOPMTs[] = {
            CONVERT_TO_STRUCT(BGOPMT0), CONVERT_TO_STRUCT(BGOPMT1),
            CONVERT_TO_STRUCT(BGOPMT2), CONVERT_TO_STRUCT(BGOPMT3),
            CONVERT_TO_STRUCT(BGOPMT4), CONVERT_TO_STRUCT(BGOPMT5),
            CONVERT_TO_STRUCT(BGOPMT6), CONVERT_TO_STRUCT(BGOPMT7),
            CONVERT_TO_STRUCT(BGOPMT8), CONVERT_TO_STRUCT(BGOPMT9),
            CONVERT_TO_STRUCT(BGOPMT10), CONVERT_TO_STRUCT(BGOPMT11),
            CONVERT_TO_STRUCT(BGOPMT12), CONVERT_TO_STRUCT(BGOPMT13),
            CONVERT_TO_STRUCT(BGOPMT14), CONVERT_TO_STRUCT(BGOPMT15)
    };
}

#endif //BUILDALL_CLOVERDATA_HPP
