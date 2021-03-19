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

#include "G4SystemOfUnits.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

#include <fstream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                          OPERATION MODES
//////////////////////////////////////////////////////////////////////////

///////////////     CLOVER Detectors - PIXIE16 Sampling     ///////////////////
const G4double      CLOVER_SamplingTime = 10; // ns
const G4int         CLOVER_TotalTimeSamples = 10; //
const G4double      CLOVER_TotalSampledTime = CLOVER_SamplingTime * CLOVER_TotalTimeSamples; // ns
const G4int         CLOVER_ComptonSupression_TimeWindow = 3; // Amount of CLOVER Time Samples


///////////////     CLOVER BGO Anti-Compton Shield - PIXIE16 Sampling    ///////////////////
const G4double      CLOVER_Shield_BGO_SamplingTime = CLOVER_SamplingTime; // ns
const G4int         CLOVER_Shield_BGO_TotalTimeSamples = CLOVER_TotalTimeSamples + CLOVER_ComptonSupression_TimeWindow; //
const G4double      CLOVER_Shield_BGO_TotalSampledTime = CLOVER_Shield_BGO_SamplingTime * CLOVER_Shield_BGO_TotalTimeSamples; // ns
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

///////////////     OCL LaBr3 Detectors - PIXIE16 Sampling     ///////////////////
const G4double      OCLLABR_SamplingTime = 10; // ns
const G4int         OCLLABR_TotalTimeSamples = 10; //
const G4double      OCLLABR_TotalSampledTime = OCLLABR_SamplingTime * OCLLABR_TotalTimeSamples; // ns

///////////////     LaBr3 Detectors - PIXIE16 Sampling     ///////////////////
const G4double      FTALABR_SamplingTime = 10; // ns
const G4int         FTALABR_TotalTimeSamples = 10; //
const G4double      FTALABR_TotalSampledTime = FTALABR_SamplingTime * FTALABR_TotalTimeSamples; // ns


class EventAction : public G4UserEventAction
{
public:
    EventAction();

    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event *event);

    virtual void EndOfEventAction(const G4Event *event);

    G4int evtNb;


    ////////////////////////
    //      CLOVERS

    G4double CLOVER_HPGeCrystal_EDep[9][4][CLOVER_TotalTimeSamples];
    G4bool CLOVER_HPGeCrystal_EDepVETO[9][4][CLOVER_TotalTimeSamples];
    G4double CLOVER_EDep[9][CLOVER_TotalTimeSamples];

    G4double CLOVER_energy[9];
    G4double BGO_energy[9];
    G4double OCLLABR_energy[2];
    G4double FTALABR_energy[6];


    void AddEnergyCLOVER_HPGeCrystal(G4int i, G4int j, G4int k, G4double a) { CLOVER_HPGeCrystal_EDep[i][j][k] += a; };


    /////////////////////////////////////////
    //      CLOVER Shield BGO Crystals
    G4double CLOVER_BGO_EDep[9][16][CLOVER_Shield_BGO_TotalTimeSamples + CLOVER_ComptonSupression_TimeWindow];

    void AddEnergyBGODetectors(G4int i, G4int j, G4int k, G4double a) { CLOVER_BGO_EDep[i][j][k] += a; };


    ////////////////////////
    //      LABRs
    G4double OCLLABR_EDep[2][OCLLABR_TotalTimeSamples];
    G4double FTALABR_EDep[6][FTALABR_TotalTimeSamples];

    void AddEnergyOCLLABR_Crystal(G4int i, G4int k, G4double a) { OCLLABR_EDep[i][k] += a; };
    void AddEnergyFTALABR_Crystal(G4int i, G4int k, G4double a) { FTALABR_EDep[i][k] += a; };

};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


