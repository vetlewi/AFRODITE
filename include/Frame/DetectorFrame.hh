//
// Created by Vetle Wegner Ingeberg on 04/05/2021.
//

#ifndef BUILDALL_DETECTORFRAME_HH
#define BUILDALL_DETECTORFRAME_HH

#include <Frame/Slot.hh>

class DetectorFrame
{
private:

    // The sixteen slots on the AFRODITE frame
    Slot std_slots[16];
    Slot sml_slots[8];

};

#endif //BUILDALL_DETECTORFRAME_HH
