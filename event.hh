#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

//#include "g4root.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
public:
    MyEventAction(MyRunAction*);
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void AddEdep(G4double edep) { fEdep += edep; }

    void SetX(G4double x) {fX = x;}
    void SetY(G4double y) {fY = y;}
    void SetZ(G4double z) {fZ = z;}

    void SetTheta(G4double theta) {fTheta = theta;}
    void SetPhi(G4double phi) {fPhi = phi;}

private:
    G4double fEdep, fX, fY, fZ, fTheta, fPhi;
};

#endif
