//
// Created by Vetle Wegner Ingeberg on 22/03/2021.
//

#ifndef CLOVERFACTORY_HH
#define CLOVERFACTORY_HH

#include <detector/DetectorFactory.hh>

#include <string>

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

#include "Constants.hh"

#include <meshreader/MeshReader.hh>

class HPGeFactory;
class ShieldFactory;

namespace Detector {

    class CloverFactory : public Detector::DetectorFactory
    {

    public:

        explicit CloverFactory(const bool &have_HPGe = true, const bool &have_Shield = true);

        ~CloverFactory() override;

        G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;

    private:
        HPGeFactory *crystalFactory;
        ShieldFactory *shieldFactory;

    };
}

#endif // CLOVERFACTORY_HH
