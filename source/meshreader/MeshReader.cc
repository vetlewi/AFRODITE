//
// Created by Vetle Wegner Ingeberg on 29/04/2021.
//

#include "meshreader/MeshReader.hh"

#include <array>
#include <sstream>
#include <string>
#include <vector>

#include <G4SystemOfUnits.hh>
#include <G4Exception.hh>
#include <G4ExceptionSeverity.hh>
#include <G4ThreeVector.hh>
#include <G4TessellatedSolid.hh>
#include <G4TriangularFacet.hh>

#include <happly.h>


namespace {

//! Build a G4TessellatedSolid from raw vertex positions and face index lists.
//! Vertices are scaled and offset; faces with more than three vertices are
//! fan-triangulated. The PLY meshes shipped with AFRODITE are already
//! triangulated, so the fan loop is only a safety net for other inputs.
G4VSolid *BuildSolid(const std::vector<std::array<double, 3>> &vertices,
                     const std::vector<std::vector<size_t>> &faces,
                     const char *name, const double &scale,
                     const G4ThreeVector &offset, const bool &reverse)
{
    auto solid = new G4TessellatedSolid(name);

    auto toPoint = [&](const size_t &index) {
        const auto &v = vertices[index];
        return G4ThreeVector(v[0] * scale + offset.x(),
                             v[1] * scale + offset.y(),
                             v[2] * scale + offset.z());
    };

    for (const auto &face : faces) {
        for (size_t i = 2; i < face.size(); ++i) {
            const G4ThreeVector point_1 = toPoint(face[0]);
            const G4ThreeVector point_2 = toPoint(face[i - 1]);
            const G4ThreeVector point_3 = toPoint(face[i]);

            G4TriangularFacet *facet;
            if ( !reverse ) {
                facet = new G4TriangularFacet(point_1, point_2, point_3, ABSOLUTE);
            } else {
                facet = new G4TriangularFacet(point_2, point_1, point_3, ABSOLUTE);
            }
            solid->AddFacet((G4VFacet*) facet);
        }
    }

    solid->SetSolidClosed(true);

    if (solid->GetNumberOfFacets() == 0) {
        G4Exception(__PRETTY_FUNCTION__, "Loaded mesh has 0 faces.",
                    FatalException, "The file may be empty.");
        return nullptr;
    }
    return solid;
}

//! Read vertices and faces out of a parsed PLY and turn them into a solid.
G4VSolid *BuildFromPLY(happly::PLYData &ply, const char *name, const double &scale,
                       const G4ThreeVector &offset, const bool &reverse)
{
    return BuildSolid(ply.getVertexPositions(), ply.getFaceIndices<size_t>(),
                      name, scale, offset, reverse);
}

} // namespace

G4VSolid *GetMesh(const char *fname, const char *name, double scale, G4ThreeVector offset, bool reverse)
{
    try {
        happly::PLYData ply{std::string(fname)};
        return BuildFromPLY(ply, name, scale, offset, reverse);
    } catch (const std::exception &e) {
        G4Exception(__PRETTY_FUNCTION__,
                    "Cannot load mesh",
                    G4ExceptionSeverity::FatalException,
                    std::string("Could not read PLY file '" + std::string(fname) + "': " + e.what()).c_str());
        return nullptr;
    }
}

G4VSolid *GetMesh(const void *buffer, const size_t &length, const char *name, double scale, G4ThreeVector offset,
                  bool reverse)
{
    try {
        // happly reads from a std::istream; wrap the in-memory PLY in one.
        std::istringstream stream(std::string(static_cast<const char*>(buffer), length));
        happly::PLYData ply(stream);
        return BuildFromPLY(ply, name, scale, offset, reverse);
    } catch (const std::exception &e) {
        G4Exception(__PRETTY_FUNCTION__,
                    "Cannot load mesh",
                    G4ExceptionSeverity::FatalException,
                    std::string("Could not read PLY from memory buffer: " + std::string(e.what())).c_str());
        return nullptr;
    }
}


MeshReader::MeshReader(const char *fname, const char *name, const double &scale, const G4ThreeVector &offset,
                       const bool &reverse)
    : solid( nullptr )
    , solid_future( std::async(std::launch::async,
                               [fname, name, scale, offset, reverse](){
        return GetMesh(fname, name, scale, offset, reverse); }))
{}

MeshReader::MeshReader(const void *buffer, const size_t &length, const char *name, const double &scale,
                       const G4ThreeVector &offset, const bool &reverse)
    : solid( nullptr )
    , solid_future( std::async(std::launch::async,
                               [buffer, length, name, scale, offset, reverse]
                               { return GetMesh(buffer, length, name, scale, offset, reverse); }) )
{}

MeshReader::MeshReader(const MeshReader::MemBuf_t &buffer, const char *name, const G4ThreeVector &offset)
        : solid( nullptr )
        , solid_future( std::async(std::launch::async,
                                   [buffer, name, offset]
                                   { return GetMesh(buffer.buffer, buffer.size, name, mm, offset, false); }) )
{}

G4VSolid *MeshReader::GetSolid()
{
    if ( !solid ){
        solid_future.wait();
        solid = solid_future.get();
    }
    return solid;
}
