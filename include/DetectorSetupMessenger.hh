//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_DETECTORSETUPMESSENGER_H
#define AFRODITE_DETECTORSETUPMESSENGER_H

#include <G4Types.hh>
#include <G4UIcommand.hh>
#include <G4UImessenger.hh>

#include <detector/DetectorFactory.hh>

constexpr const int number_of_large_frame_positions = 16;
constexpr const int number_of_small_frame_positions = 8;

struct DetectorInfo_t
{
    int detector_id;        //! Detector number, separate for each detector type
    Detector::Type type;    //! Type of the detector
    double distance;        //! Distance of the detector from the target
};

namespace Slot {
    enum Type {
        standard,
        small
    };
}

struct SlotInfo_t
{
    int slot_id;
    Slot::Type type;
    DetectorInfo_t detector;

    double GetTheta() const;    //! Return the theta angle of the slot
    double GetPhi() const;      //! Return the phi angle of the slot
};

class FrameSetupMessenger : public G4UImessenger
{
public:
    FrameSetupMessenger();
    ~FrameSetupMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue) override;
    G4String GetCurrentValue(G4UIcommand * command);

private:


};


#endif //AFRODITE_DETECTORSETUPMESSENGER_H
