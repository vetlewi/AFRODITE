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
#define NUMBER_OF_LARGE_FRAME_POSITIONS 16
#define NUMBER_OF_SMALL_FRAME_POSITIONS 8

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInterger;
class G4UIcmdWithABool;

class DetectorSetupMessenger;

struct STDSlotInfo_t {
    int slotID; // 0 - 15
    Detector::Type type;
    double slotDistance;

    STDSlotInfo_t() = default;
    STDSlotInfo_t(const int &id, const Detector::Type &typ, const double &distance)
        : slotID( id ), type( typ ), slotDistance( distance ){}

    STDSlotInfo_t(const char *param);
};

class DetectorSetupMessenger : public G4UImessenger
{
private:
    G4UIdirectory *fDirectory;

    using slot_t = std::pair<Detector::Type, double>;

    slot_t std_slots[number_of_large_frame_positions];
    slot_t sml_slots[number_of_small_frame_positions];

public:
    DetectorSetupMessenger();
    ~DetectorSetupMessenger() override = default;



private:


};

#endif //AFRODITE_DETECTORSETUPMESSENGER_H
