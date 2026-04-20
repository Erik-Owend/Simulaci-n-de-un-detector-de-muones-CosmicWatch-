#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4GeneralParticleSource();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    //G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

    //if(particle == G4Geantino::Geantino())
    //{
        //G4int Z = 27;
        //G4int A = 60;

        //G4double charge = 0.*eplus;
        //G4double energy = 0.*keV;

        //G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);

        //fParticleGun->SetParticleDefinition(ion);
        //fParticleGun->SetParticleCharge(charge);
    //}    

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
