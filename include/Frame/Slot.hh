//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_SLOT_HH
#define AFRODITE_SLOT_HH

#include <detector/DetectorFactory.hh>

//! A single mounting position on the AFRODITE frame.
//!
//! The direction of a slot (theta, phi) is a *fixed* property of the frame
//! geometry and is never changed at runtime. What can be configured at runtime
//! is which detector (if any) occupies the slot and at what distance.
//!
//! Angles are stored in the standard physical convention used throughout the
//! geometry: theta is the polar angle measured from the +z beam axis and phi is
//! the azimuthal angle measured from +x towards +y. Both are stored in Geant4
//! internal units (radians); distance is stored in internal length units.
struct Slot {
    double theta = 0.;                              //!< fixed polar angle from +z (frame geometry)
    double phi = 0.;                                //!< fixed azimuthal angle from +x (frame geometry)

    Detector::Type type = Detector::Type::none;     //!< runtime: occupant; none means empty
    double distance = 0.;                           //!< runtime: detector-centre distance from target
    bool shield = true;                             //!< runtime: CLOVER BGO shield (ignored for other types)

    //! A slot is occupied when it holds an actual detector.
    bool occupied() const { return type != Detector::Type::none; }
};

#endif //AFRODITE_SLOT_HH
