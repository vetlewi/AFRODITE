//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//      ----------------------------------------------------------------
//                          AFRODITE (iThemba Labs)
//      ----------------------------------------------------------------
//
//      Github repository: https://www.github.com/KevinCWLi/AFRODITE
//
//      Main Author:    K.C.W. Li
//
//      email: likevincw@gmail.com
//

#ifndef EventAction_h
#define EventAction_h 1

#include <G4UserEventAction.hh>
#include <G4Types.hh>
#include "Constants.hh"

#include <SteppingAction.hh>

class G4Event;

class EventAction : public G4UserEventAction
{
public:
    EventAction();

    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event *event);

    virtual void EndOfEventAction(const G4Event *event);

protected:

    G4double CLOVER_energy[numberOf_CLOVER];
    G4double BGO_energy[numberOf_CLOVER];

    G4double OCLLABR_energy[numberOf_OCLLaBr3];
    G4double FTALABR_energy[numberOf_FTALaBr3];

    G4double DeltaE_ring_energy[numberOf_SiRings];
    G4double DeltaE_sector_energy[numberOf_SiSectors];
#if ANALYZE_SI_DETECTORS
    G4double E_ring_energy[numberOf_SiRings];
    G4double E_sector_energy[numberOf_SiSectors];
#endif // ANALYZE_SI_DETECTORS

    friend class SteppingAction;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


