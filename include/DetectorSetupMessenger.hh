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
class G4UIcmdWithAString;

struct STDSlotInfo_t {
    int detectorID; // 0 - 15
    Detector::Type type;
    double slotDistance;

    STDSlotInfo_t() : detectorID( 0 ), type( Detector::none ), slotDistance( 0 ){}
    STDSlotInfo_t(const int &id, const Detector::Type &typ, const double &distance)
        : detectorID( id ), type( typ ), slotDistance( distance ){}

    STDSlotInfo_t(const char *param);
};

class CMDSlotID : public G4UIcommand
{
public:
    CMDSlotID(const char* theCommandPath, G4UImessenger* theMessenger);
    ~CMDSlotID() override = default;
    static std::pair<int, STDSlotInfo_t> GetSlotInfo(const char *param);
};

class DetectorSetupMessenger : public G4UImessenger
{
private:
    G4UIdirectory *fDirectory;
    CMDSlotID *slotCmd;


    STDSlotInfo_t std_slots[number_of_large_frame_positions];

public:
    DetectorSetupMessenger();
    ~DetectorSetupMessenger() override;


    void SetNewValue(G4UIcommand* command, G4String newValue) override;

    inline STDSlotInfo_t GetSlot(const size_t &n) const { return std_slots[n]; }

//#if TEST_INTERFACE
    inline G4UIcommand *getSlotCMD(){ return slotCmd; }
//#endif // TEST_INTERFACE

};

#endif //AFRODITE_DETECTORSETUPMESSENGER_H
