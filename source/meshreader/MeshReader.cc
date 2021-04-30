//
// Created by Vetle Wegner Ingeberg on 29/04/2021.
//

#include "meshreader/MeshReader.hh"

#include <string>

#include <G4SystemOfUnits.hh>
#include <G4Exception.hh>
#include <G4ExceptionSeverity.hh>
#include <G4ThreeVector.hh>
#include <G4TessellatedSolid.hh>
#include <G4TriangularFacet.hh>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


G4VSolid *GetMeshFromScene(const aiScene *scene, const char *name, const double &scale,
                              const G4ThreeVector &offset, const bool &reverse)
{
    auto m = scene->mMeshes[0];
    auto solid = new G4TessellatedSolid(name);

    G4ThreeVector point_1;
    G4ThreeVector point_2;
    G4ThreeVector point_3;

    for(unsigned int i=0; i < m->mNumFaces; i++)
    {
        const aiFace& face = m->mFaces[i];

        point_1.setX(m->mVertices[face.mIndices[0]].x * scale + offset.x());
        point_1.setY(m->mVertices[face.mIndices[0]].y * scale + offset.y());
        point_1.setZ(m->mVertices[face.mIndices[0]].z * scale + offset.z());

        point_2.setX(m->mVertices[face.mIndices[1]].x * scale + offset.x());
        point_2.setY(m->mVertices[face.mIndices[1]].y * scale + offset.y());
        point_2.setZ(m->mVertices[face.mIndices[1]].z * scale + offset.z());

        point_3.setX(m->mVertices[face.mIndices[2]].x * scale + offset.x());
        point_3.setY(m->mVertices[face.mIndices[2]].y * scale + offset.y());
        point_3.setZ(m->mVertices[face.mIndices[2]].z * scale + offset.z());

        G4TriangularFacet * facet;
        if ( !reverse ) {
            facet = new G4TriangularFacet(point_1, point_2, point_3, ABSOLUTE);
        } else {
            facet = new G4TriangularFacet(point_2, point_1, point_3, ABSOLUTE);
        }
        solid->AddFacet((G4VFacet*) facet);
    }

    solid->SetSolidClosed(true);

    if (solid->GetNumberOfFacets() == 0) {
        G4Exception(__PRETTY_FUNCTION__, "Loaded mesh has 0 faces.",
                    FatalException, "The file may be empty.");
        return nullptr;
    }
    return solid;
}

G4VSolid *GetMesh(const char *fname, const char *name, double scale, G4ThreeVector offset, bool reverse)
{
    Assimp::Importer importer;
    auto *scene = importer.ReadFile(fname,aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_CalcTangentSpace);
    if ( !scene ){
        G4Exception(__PRETTY_FUNCTION__,
                    "Cannot load mesh",
                    G4ExceptionSeverity::FatalException,
                    std::string("Could not open file '" + std::string(fname) + "'").c_str());
    }
    return GetMeshFromScene(scene, name, scale, offset, reverse);
}

G4VSolid *GetMesh(const void *buffer, const size_t &length, const char *name, double scale, G4ThreeVector offset,
                  bool reverse)
{
    Assimp::Importer importer;
    auto *scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_CalcTangentSpace,
                                              "PLY");
    if ( !scene ){
        G4Exception(__PRETTY_FUNCTION__,
                    "Cannot load mesh",
                    G4ExceptionSeverity::FatalException,
                    "Could not open file");
    }
    return GetMeshFromScene(scene, name, scale, offset, reverse);
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