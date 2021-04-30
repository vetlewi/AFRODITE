//
// Created by Vetle Wegner Ingeberg on 29/04/2021.
//

#ifndef MESHREADER_HH
#define MESHREADER_HH

#include <future>
#include <G4ThreeVector.hh>

class G4VSolid;

class MeshReader
{
public:
    struct MemBuf_t {
        const void *buffer;
        size_t size;
    };
public:

    /*!
     * Construct the solid from a file. Will throw if unable to open file.
     * @param file path to file
     */
    MeshReader(const char *file, const char *name, const double &scale = 1.0,
               const G4ThreeVector &offset = G4ThreeVector(), const bool &reverse = false);

    /*!
     * Construct the solid from a memory buffer.
     * @param buffer Pointer to the memory buffer.
     * @param length Size of the memory buffer.
     */
    MeshReader(const void *buffer, const size_t &length, const char *name, const double &scale = 1.0,
               const G4ThreeVector &offset = G4ThreeVector(), const bool &reverse = false);

    /*!
     * Same as above but with a struct wrapper to make usage easier from other classes
     * \param buffer
     * \param name
     * \param scale
     * \param offset
     * \param reverse
     */
    MeshReader(const MemBuf_t &buffer, const char *name, const double &scale = 1.0,
               const G4ThreeVector &offset = G4ThreeVector(), const bool &reverse = false);

    MeshReader(const MemBuf_t &buffer, const char *name, const G4ThreeVector &offset = G4ThreeVector());

    /*!
     * Get the Geant4 solid
     * @return Pointer to the solid object.
     */
    G4VSolid *GetSolid();


private:
    G4VSolid *solid;
    std::future<G4VSolid *> solid_future;
};

#endif // MESHREADER_HH
