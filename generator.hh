#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "EcoMug.h"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction 
{
public:
    MyPrimaryGenerator();
    ~MyPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *anEvent) override;

private:
    G4ParticleGun *fParticleGun;
    EcoMug fEcoMug; // Instancia del generador EcoMug
};

#endif
