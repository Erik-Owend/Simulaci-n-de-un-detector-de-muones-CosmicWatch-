#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() 
{
    // Instanciar el cañón de partículas para 1 partícula por evento
    fParticleGun = new G4ParticleGun(1);

    // 1. Configurar EcoMug para generación en semiesfera (HSphere)
    fEcoMug.SetUseHSphere();

    // 2. Definir el radio de la semiesfera en metros (10 cm = 0.1 m en unidades de EcoMug)
    fEcoMug.SetHSphereRadius(0.1); 

    // 3. Definir el centro de la semiesfera en el origen {0, 0, 0}
    fEcoMug.SetHSphereCenterPosition({{0.0, 0.0, 0.0}});
}

MyPrimaryGenerator::~MyPrimaryGenerator() 
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) 
{
    // Generar un nuevo muon cósmico con EcoMug
    fEcoMug.Generate();

    // Extraer la posición (EcoMug entrega metros; se convierte a unidades Geant4)
    std::array<double, 3> pos = fEcoMug.GetGenerationPosition();
    G4ThreeVector g4pos(pos[0] * m, pos[1] * m, pos[2] * m);

    // Extraer el vector momento en 3D pasando el array por referencia
    std::array<double, 3> mom;
    fEcoMug.GetGenerationMomentum(mom);
    G4ThreeVector g4mom(mom[0], mom[1], mom[2]);

    // Asignar si es mu- o mu+ según la carga muestreada por EcoMug
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    if (fEcoMug.GetCharge() < 0) {
        fParticleGun->SetParticleDefinition(particleTable->FindParticle("mu-"));
    } else {
        fParticleGun->SetParticleDefinition(particleTable->FindParticle("mu+"));
    }

    // Configurar posición, dirección y magnitud del momento en el cañón de Geant4
    fParticleGun->SetParticlePosition(g4pos);
    fParticleGun->SetParticleMomentumDirection(g4mom.unit());
    fParticleGun->SetParticleMomentum(g4mom.mag() * GeV);

    // Generar el vértice primario en el evento actual de Geant4
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
