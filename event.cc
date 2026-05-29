#include "event.hh"

#include "G4SystemOfUnits.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;

    fX = 0.;
    fY = 0.;
    fZ = 0.;

    fTheta = 0.;
    fPhi = 0.;
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
    G4cout << "Energy deposition: " << fEdep << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(0, 0, fEdep);

    man->FillNtupleDColumn(0, 1, fX/cm);
    man->FillNtupleDColumn(0, 2, fY/cm);
    man->FillNtupleDColumn(0, 3, fZ/cm);

    man->FillNtupleDColumn(0, 4, fTheta/deg);
    man->FillNtupleDColumn(0, 5, fPhi/deg);

    man->AddNtupleRow(0);
}   
