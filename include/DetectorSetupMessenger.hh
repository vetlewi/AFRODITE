//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_DETECTORSETUPMESSENGER_H
#define AFRODITE_DETECTORSETUPMESSENGER_H

#include <G4Types.hh>
#include <G4UIcommand.hh>
#include <G4UImessenger.hh>

#define NUMBER_OF_LARGE_FRAME_POSITIONS 16
#define NUMBER_OF_SMALL_FRAME_POSITIONS 8

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInterger;
class G4UIcmdWithABool;

class DetectorSetupMessenger;

class LargeFrameSlot {
private:


};

class DetectorSetupMessenger : public G4UImessenger
{
private:
    G4UIdirectory *fDirectory;


public:
    DetectorSetupMessenger();
    ~DetectorSetupMessenger() override = default;



private:


};

#endif //AFRODITE_DETECTORSETUPMESSENGER_H
