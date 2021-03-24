//
// Created by Vetle Wegner Ingeberg on 24/03/2021.
//

#include "DetectorHit.hh"

#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4VisAttributes.hh>
#include <G4UnitsTable.hh>
#include <utility>


G4ThreadLocal G4Allocator<DetectorHit>* B2TrackerHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit()
    : G4VHit()
    , fDetectorID( -1 )
    , fTrackID( -1 )
    , fEdep( 0. )
    , fPos( G4ThreeVector() ){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit(const G4int &detID, const G4int &trackID, const G4double &edep, G4ThreeVector pos)
    : fDetectorID( detID ), fTrackID( trackID ), fEdep( edep ), fPos(std::move( pos )){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if ( pVVisManager ){
        G4Circle circle(fPos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Print()
{
    G4cout << "DetectorID: " << fDetectorID << " TrackID: " << fTrackID;
    G4cout << " Edep: " << std::setw(7) << G4BestUnit(fEdep,"Energy");
    G4cout << " Position: " << std::setw(7) << G4BestUnit( fPos,"Length") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......