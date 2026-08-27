//
// Created by Vetle Wegner Ingeberg on 04/05/2021.
//

#ifndef AFRODITE_DETECTORFRAME_HH
#define AFRODITE_DETECTORFRAME_HH

#include <Frame/Slot.hh>

//! Runtime-configurable description of the AFRODITE detector frame.
//!
//! The frame has two independent families of slots, addressed by global index:
//!   - large slots (0..15): may hold a clover, an OCL LaBr3 or an FTA LaBr3.
//!   - small slots (0..7):  may hold an FTA LaBr3 only.
//!
//! Each slot's direction is fixed by the frame geometry (seeded in the
//! constructor). By default every slot is empty; detectors are placed through
//! the /AFRODITE/ UI commands (see DetectorSetupMessenger) before
//! /run/initialize. The structure-presence flags select the passive volumes.
class DetectorFrame
{
public:
    static constexpr int num_large = 16;
    static constexpr int num_small = 8;

    //! Seeds the fixed slot directions; all slots start empty.
    DetectorFrame();

    Slot &large(int id) { return large_slots[id]; }
    Slot &small(int id) { return small_slots[id]; }
    const Slot &large(int id) const { return large_slots[id]; }
    const Slot &small(int id) const { return small_slots[id]; }

    static bool validLarge(int id) { return id >= 0 && id < num_large; }
    static bool validSmall(int id) { return id >= 0 && id < num_small; }

    //! Detector types each slot family is allowed to hold.
    static bool largeAllows(Detector::Type type);
    static bool smallAllows(Detector::Type type);

    // Passive structures (not slots). Defaults reproduce the previous build.
    bool place_chamber = true;
    bool place_frame = true;
    bool place_target = true;

private:
    Slot large_slots[num_large];
    Slot small_slots[num_small];
};

#endif //AFRODITE_DETECTORFRAME_HH
