//
// Created by Vetle Wegner Ingeberg on 23/03/2021.
//

#ifndef AFRODITE_DETECTORHIT_HH
#define AFRODITE_DETECTORHIT_HH

#include <G4VHit.hh>
#include <G4ThreeVector.hh>
#include <G4THitsCollection.hh>

class DetectorHit : public G4VHit
{
public:
    DetectorHit(const G4int &detID);
    ~DetectorHit() override = default;

    void AddEdep(const double &e){ fEdep = e; }

private:
    G4int fDetectorID;
    G4int fTrackID;
    G4double fEdep;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;
extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif //AFRODITE_DETECTORHIT_HH
