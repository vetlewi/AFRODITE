//
// Created by Vetle Wegner Ingeberg on 29/04/2021.
//

#include <doctest/doctest.h>
#include <meshreader/MeshReader.hh>
#include <meshreader/incbin.h>

#include <G4SystemOfUnits.hh>

#ifndef SRC_PATH
#define SRC_PATH "../"
#endif // SRC_PATH

#ifndef PLY_PATH
#define PLY_PATH SRC_PATH"Mesh-Models"
#endif // PLY_PATH

INCBIN(HPGeCrystalA, PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply");
INCBIN(TargetChamber, PLY_PATH"/STRUCTURES/MathisTC/target_chamber_new_sealed_fused_10umTolerance.ply");

TEST_CASE("MeshReader"){

    const char *TC_path = PLY_PATH"/DETECTORS/CLOVER/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply";
    MeshReader reader_file(TC_path, "HPGe_crystalA");

    const void *buffer = reinterpret_cast<const void *>(gTargetChamberData);
    const size_t size = gTargetChamberSize;
    MeshReader reader_buffer(buffer, size, "HPGe_crystalA");

    // Get the mesh object.
    SUBCASE("Read from file"){
        REQUIRE_NOTHROW( auto *mesh = reader_file.GetSolid() );
    }

    SUBCASE("Read from memory buffer"){
        REQUIRE_NOTHROW( auto *mesh = reader_buffer.GetSolid() );
    }

}