//
// Created by Vetle Wegner Ingeberg on 28/06/2021.
//

#ifndef DETECTORFACTORIES_HH
#define DETECTORFACTORIES_HH

#include <map>

#include <detector/DetectorFactory.hh>

namespace Detector {

    class DetectorFactories {
    private:

        //! Mapping storing the detector factories
        std::map<Type, DetectorFactory *> factoryMapping;

    public:

        //! Construct all the different types of detector factories
        DetectorFactories();

        //! Ensure that all the factories are deleted when killed
        ~DetectorFactories();

        //! Find the correct factory from enum type
        DetectorFactory *GetFactory(const Type &type) const;

        //! Find the correct factory from string
        DetectorFactory *GetFactory(const char *type) const;


    };
}

#endif //DETECTORFACTORIES_HH
