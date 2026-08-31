//
// Created by Vetle Wegner Ingeberg on 03/05/2021.
//

#ifndef BUILDALL_CLOVERDATA_HPP
#define BUILDALL_CLOVERDATA_HPP

#include <meshreader/MeshReader.hh>

// The mesh data below is generated at build time by bin2cpp (see
// cmake/EmbedBinary.cmake) from the .ply files under Mesh-Models/, one
// header per asset. Each header declares `<NAME>_data` (an array with a
// known bound) and `<NAME>_size` (a compile-time constant), so building the
// MemBuf_t below needs no runtime code — it's pure static data.
#include "vaccum.mesh.hh"
#include "encasement.mesh.hh"
#include "HPGeCrystalA.mesh.hh"
#include "HPGeCrystalB.mesh.hh"
#include "HPGeCrystalC.mesh.hh"
#include "HPGeCrystalD.mesh.hh"
#include "HPGeContactA.mesh.hh"
#include "HPGeContactB.mesh.hh"
#include "HPGeContactC.mesh.hh"
#include "HPGeContactD.mesh.hh"
#include "body.mesh.hh"
#include "heavimet.mesh.hh"
#include "PMTConArray.mesh.hh"
#include "BGOCrystal0.mesh.hh"
#include "BGOCrystal1.mesh.hh"
#include "BGOCrystal2.mesh.hh"
#include "BGOCrystal3.mesh.hh"
#include "BGOCrystal4.mesh.hh"
#include "BGOCrystal5.mesh.hh"
#include "BGOCrystal6.mesh.hh"
#include "BGOCrystal7.mesh.hh"
#include "BGOCrystal8.mesh.hh"
#include "BGOCrystal9.mesh.hh"
#include "BGOCrystal10.mesh.hh"
#include "BGOCrystal11.mesh.hh"
#include "BGOCrystal12.mesh.hh"
#include "BGOCrystal13.mesh.hh"
#include "BGOCrystal14.mesh.hh"
#include "BGOCrystal15.mesh.hh"
#include "BGOPMT0.mesh.hh"
#include "BGOPMT1.mesh.hh"
#include "BGOPMT2.mesh.hh"
#include "BGOPMT3.mesh.hh"
#include "BGOPMT4.mesh.hh"
#include "BGOPMT5.mesh.hh"
#include "BGOPMT6.mesh.hh"
#include "BGOPMT7.mesh.hh"
#include "BGOPMT8.mesh.hh"
#include "BGOPMT9.mesh.hh"
#include "BGOPMT10.mesh.hh"
#include "BGOPMT11.mesh.hh"
#include "BGOPMT12.mesh.hh"
#include "BGOPMT13.mesh.hh"
#include "BGOPMT14.mesh.hh"
#include "BGOPMT15.mesh.hh"

#define MESH_STRUCT(NAME) MeshReader::MemBuf_t{ NAME##_data, NAME##_size }

namespace HPGe_Components {

    static const MeshReader::MemBuf_t vaccum = MESH_STRUCT(vaccum);
    static const MeshReader::MemBuf_t encasement = MESH_STRUCT(encasement);

    static const MeshReader::MemBuf_t crystals[] = {
            MESH_STRUCT(HPGeCrystalA), MESH_STRUCT(HPGeCrystalB),
            MESH_STRUCT(HPGeCrystalC), MESH_STRUCT(HPGeCrystalD)};

    static const MeshReader::MemBuf_t contacts[] = {
            MESH_STRUCT(HPGeContactA), MESH_STRUCT(HPGeContactB),
            MESH_STRUCT(HPGeContactC), MESH_STRUCT(HPGeContactD)
    };
}

namespace Shield_parts {

    static const MeshReader::MemBuf_t body = MESH_STRUCT(body);
    static const MeshReader::MemBuf_t heavimet = MESH_STRUCT(heavimet);
    static const MeshReader::MemBuf_t PMTConArray = MESH_STRUCT(PMTConArray);

    static const MeshReader::MemBuf_t BGOCrystals[] = {
            MESH_STRUCT(BGOCrystal0), MESH_STRUCT(BGOCrystal1),
            MESH_STRUCT(BGOCrystal2), MESH_STRUCT(BGOCrystal3),
            MESH_STRUCT(BGOCrystal4), MESH_STRUCT(BGOCrystal5),
            MESH_STRUCT(BGOCrystal6), MESH_STRUCT(BGOCrystal7),
            MESH_STRUCT(BGOCrystal8), MESH_STRUCT(BGOCrystal9),
            MESH_STRUCT(BGOCrystal10), MESH_STRUCT(BGOCrystal11),
            MESH_STRUCT(BGOCrystal12), MESH_STRUCT(BGOCrystal13),
            MESH_STRUCT(BGOCrystal14), MESH_STRUCT(BGOCrystal15)
    };

    static const MeshReader::MemBuf_t BGOPMTs[] = {
            MESH_STRUCT(BGOPMT0), MESH_STRUCT(BGOPMT1),
            MESH_STRUCT(BGOPMT2), MESH_STRUCT(BGOPMT3),
            MESH_STRUCT(BGOPMT4), MESH_STRUCT(BGOPMT5),
            MESH_STRUCT(BGOPMT6), MESH_STRUCT(BGOPMT7),
            MESH_STRUCT(BGOPMT8), MESH_STRUCT(BGOPMT9),
            MESH_STRUCT(BGOPMT10), MESH_STRUCT(BGOPMT11),
            MESH_STRUCT(BGOPMT12), MESH_STRUCT(BGOPMT13),
            MESH_STRUCT(BGOPMT14), MESH_STRUCT(BGOPMT15)
    };
}

#undef MESH_STRUCT

#endif //BUILDALL_CLOVERDATA_HPP
