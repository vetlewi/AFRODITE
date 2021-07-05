//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_DETECTORS_HH
#define AFRODITE_DETECTORS_HH

class G4AssemblyVolume;

namespace Detector {

    struct Parameters {
    };

    enum Type
    {
        none,
        clover,
        fta_labr,
        ocl_labr,
        alba_labr, // Not yet implemented
        s2_silicon
    };

    class DetectorFactory {

    private:
        const Detector::Type detector_type;

    protected:
        explicit DetectorFactory(const Detector::Type &type) : detector_type( type ){}

    public:
        virtual ~DetectorFactory() = default;
        virtual G4AssemblyVolume *GetAssembly(const int &copy_no, const bool &overlap) = 0;
        //virtual void PlaceAssembly(const int &copy_no, const bool &overlap) = 0;

        inline Detector::Type GetType() const { return detector_type; }

        virtual void SetParameters(const Parameters *param) = 0;

    };
}

#endif //AFRODITE_DETECTORS_HH
