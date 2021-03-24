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
    DetectorHit();

    DetectorHit(const G4int &detID, const G4int &trackID, const G4double &edep, G4ThreeVector pos);

    ~DetectorHit() override = default;

    inline G4bool operator==(const DetectorHit &rhs) const { return (this == &rhs); }

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw() override;
    void Print() override;

    inline void SetDetectorID(const G4int &id){ fDetectorID = id; }
    inline void SetTrackID(const G4int &track){ fTrackID = track; }
    inline void SetEDep(const G4double &edep){ fEdep = edep; }
    inline void SetPos(const G4ThreeVector &pos){ fPos = pos; }

    inline G4int GetDetectorID() const { return fDetectorID; }
    inline G4int GetTrackID() const { return fTrackID; }
    inline G4double GetEdep() const { return fEdep; }
    inline G4ThreeVector GetPos() const { return fPos; }

private:
    G4int fDetectorID;
    G4int fTrackID;
    G4double fEdep;
    G4ThreeVector fPos;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void *DetectorHit::operator new(size_t)
{
    if ( !DetectorHitAllocator )
        DetectorHitAllocator = new G4Allocator<DetectorHit>;
    return (void*)DetectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorHit::operator delete(void *hit)
{
    DetectorHitAllocator->FreeSingle((DetectorHit*) hit);
}

#endif //AFRODITE_DETECTORHIT_HH
