//
// Created by Vetle Wegner Ingeberg on 22/04/2021.
//

#ifndef AFRODITE_DETECTORSETUPMESSENGER_H
#define AFRODITE_DETECTORSETUPMESSENGER_H

#include <G4Types.hh>
#include <G4UIcommand.hh>
#include <G4UImessenger.hh>

#include <G4GenericMessenger.hh>

#include <detector/DetectorFactory.hh>

class G4UIcmdWithABool;

constexpr const int number_of_large_frame_positions = 16;
constexpr const int number_of_small_frame_positions = 8;

struct GammaDetectorInfo {
    double distance; //! Distance from the target
    Detector::Type type; //! Detector type
};

struct ParticleDetectorInfo {
    double distance; //! Distance from the target
    double thickness; //! Thickness of the detector
    Detector::Type type; //! Detector type
};

class DetectorSetupMessenger : public G4UImessenger
{
public:
    DetectorSetupMessenger();
    ~DetectorSetupMessenger() override;

    void SetNewValue(G4UIcommand* command, G4String newValue) override;
    G4String GetCurrentValue(G4UIcommand * command) override;

    std::vector<GammaDetectorInfo> GetStdSlot() const
    { return std::vector<GammaDetectorInfo>(standard_slot,standard_slot+number_of_large_frame_positions); }

    std::vector<GammaDetectorInfo> GetSmallSlot() const
    { return std::vector<GammaDetectorInfo>(small_slot,small_slot+number_of_small_frame_positions); }

    std::vector<ParticleDetectorInfo> GetParticleArray() const
    { return std::vector<ParticleDetectorInfo>(particle_array, particle_array+2); }

    bool TargetPresent() const { return targetPresent; }
    bool FramePresent() const { return framePresent; }
    bool TargetChamberPresent() const { return targetChamberPresent; }

private:
    GammaDetectorInfo standard_slot[number_of_large_frame_positions];
    GammaDetectorInfo small_slot[number_of_small_frame_positions];

    ParticleDetectorInfo particle_array[2];

    bool targetPresent;
    bool framePresent;
    bool targetChamberPresent;

    G4UIdirectory* fGammaDetectorDir;
    G4UIdirectory* fParticleDetectorDir;

    G4UIcommand* fSTDGammaDetectorTypeCmd;
    G4UIcommand* fSTDGammaDetectorDistanceCmd;

    G4UIcommand* fSmallGammaDetectorTypeCmd;
    G4UIcommand* fSmallGammaDetectorDistanceCmd;

    G4UIcommand* fParticleDetectorPresentCmd;
    G4UIcommand* fParticleDetectorPosCmd;
    G4UIcommand* fParticleDetectorThicknessCmd;

    G4UIcmdWithABool* fTargetPresent;

    G4UIcmdWithABool* fFramePresent;
    G4UIcmdWithABool* fTargetChamberPresent;


    std::vector<G4UIcommand*> commands;
};


#endif //AFRODITE_DETECTORSETUPMESSENGER_H
