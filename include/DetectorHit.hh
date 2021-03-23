//
// Created by Vetle Wegner Ingeberg on 23/03/2021.
//

#ifndef AFRODITE_DETECTORHIT_HH
#define AFRODITE_DETECTORHIT_HH

#include <G4VHit.hh>

class DetectorHit : public G4VHit
{
public:
    DetectorHit();
    DetectorHit(const DetectorHit &);

    ~DetectorHit() override;

    const DetectorHit &operator=(const DetectorHit &);
    G4int operator==(const DetectorHit &) const;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    void Draw() override;
    void Print() override;

    void SetDetectorID(const G4int &id){  }

private:
    G4
    G4int fTrackID;
    G4double fEdep;
    G4ThreeVector fPos;

};

#endif //AFRODITE_DETECTORHIT_HH
