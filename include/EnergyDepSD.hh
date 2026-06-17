//
// Created by Vetle Wegner Ingeberg on 23/03/2021.
//

#ifndef ENERGYDEPSD_HH
#define ENERGYDEPSD_HH

#include "DetectorHit.hh"
#include <G4VSensitiveDetector.hh>

class G4Step;
class G4HCofThisEvent;

class EnergyDepSD : public G4VSensitiveDetector {

public:
    EnergyDepSD(const G4String &name, const G4String &hitsCollectionName);
    ~EnergyDepSD() override = default;

    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
    DetectorHitsCollection *fHitsCollection;

};

#endif // ENERGYDEPSD_HH
