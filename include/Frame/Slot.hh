//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_SLOT_HH
#define AFRODITE_SLOT_HH

#include <Detectors/DetectorFactory.hh>

class Slot {

private:

    int detector_number;
    Detector::DetectorFactory *factory;

public:

    // Construction of a slot is done by giving it a detector type and a distance
    Slot(const int &dnum = 0, Detector::DetectorFactory *factory = nullptr);

    // Build the detector in place
    void Assemble();

};


#endif //AFRODITE_SLOT_HH
