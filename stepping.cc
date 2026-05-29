#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    if(volume != fScoringVolume)
        return;

    G4double edep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edep);

    if(step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {
        G4StepPoint *point = step->GetPreStepPoint();

        G4ThreeVector pos = point->GetPosition();

        G4ThreeVector dir = point->GetMomentumDirection();

        fEventAction->SetX(pos.x());
        fEventAction->SetY(pos.y());
        fEventAction->SetZ(pos.z());

        fEventAction->SetTheta(dir.theta());
        fEventAction->SetPhi(dir.phi());
    }
}
