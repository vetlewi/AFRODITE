//
// Created by Vetle Wegner Ingeberg on 24/03/2021.
//

#include "EnergyDepSD.hh"
#include "DetectorHit.hh"

#include <G4HCofThisEvent.hh>
#include <G4ThreeVector.hh>
#include <G4Step.hh>
#include <G4SDManager.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EnergyDepSD::EnergyDepSD(const G4String &name, const G4String &hitsCollectionName)
    : G4VSensitiveDetector(name)
    , fHitsCollection( nullptr )
{
    collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EnergyDepSD::Initialize(G4HCofThisEvent *hitCollection)
{
    fHitsCollection =
            new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hitCollection
    G4int hcID =
            G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hitCollection->AddHitsCollection(hcID, fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool EnergyDepSD::ProcessHits(G4Step *step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();

    if ( edep == 0 ) return false;

    auto newHit = new DetectorHit(step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(),
                                  step->GetTrack()->GetTrackID(), edep, step->GetPostStepPoint()->GetPosition());
    fHitsCollection->insert( newHit );
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EnergyDepSD::EndOfEvent(G4HCofThisEvent *hitCollection)
{
    if ( verboseLevel>1 ) {
        G4int nofHits = fHitsCollection->entries();
        G4cout << G4endl
               << "-------->Hits Collection: in this event they are " << nofHits
               << " hits in the tracker chambers: " << G4endl;
        for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
