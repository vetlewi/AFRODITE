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

    struct CloverParameters : public Parameters
    {
        bool have_HPGe;
        bool have_Shield;

        CloverParameters() : have_HPGe( true ), have_Shield( true ){}
    };

    class CloverFactory : public Detector::DetectorFactory
    {

    public:

        CloverFactory(const bool &have_HPGe = true, const bool &have_Shield = true);

        ~CloverFactory() override;

        G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &checkOverlap) override;

        void SetParameters(const Parameters *param) override;

    private:
        HPGeFactory *crystalFactory;
        ShieldFactory *shieldFactory;

    };
}

#endif // CLOVERFACTORY_HH
