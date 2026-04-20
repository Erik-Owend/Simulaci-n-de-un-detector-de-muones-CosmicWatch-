#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
    fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");
fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup");

    nCols = 10;
    nRows = 10;

    DefineMaterials();

    xWorld = 0.4*m/2;
    yWorld = 0.4*m/2;
    zWorld = 0.4*m/2;

    isCherenkov = false;
    isScintillator = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    H2O = new G4Material("H2O", 1.000*g/cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    C = nist->FindOrBuildElement("C");

    Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5*perCent);
    Aerogel->AddMaterial(H2O, 37.4*perCent);
    Aerogel->AddElement(C, 0.1*perCent);

    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    G4double energy[5] = {1.239841939*eV/0.48, 1.239841939*eV/0.44, 1.239841939*eV/0.43, 1.239841939*eV/0.42, 1.239841939*eV/0.38};
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4double rindexWorld[2] = {1.0, 1.0};
    //G4double rindexNaI[2] = {1.78, 1.78};
    G4double rindexScint[5] = {1.60, 1.59, 1.58, 1.57, 1.56};
    G4double absorLenScint[5] = {160*cm, 160*cm, 160*cm, 160*cm, 160*cm};
	G4double decaytScint = 2.4*ns;
	G4double LYieldScint = 10./keV;
    G4double reflectivity[2] = {0.98, 0.98};

    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);

    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 5);

    //Na = nist->FindOrBuildElement("Na");
    //I = nist->FindOrBuildElement("I");
    //NaI = new G4Material("NaI", 3.67*g/cm3, 2);
    //NaI->AddElement(Na, 1);
    //NaI->AddElement(I, 1);

	Scint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    //G4double fraction[2] = {1.0, 1.0};

    G4double spectScint[5] = {0.18, 0.8, 1.0, 0.85, 0.07};

    //G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
    //mptNaI->AddProperty("RINDEX", energy, rindexNaI, 2);
    //mptNaI->AddProperty("FASTCOMPONENT", energy, fraction, 2, true);
    //mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38./keV);
    //mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.0);
    //mptNaI->AddConstProperty("FASTTIMECONSTANT", 250*ns, true);
    //mptNaI->AddConstProperty("YIELDRATIO", 1., true);

    G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();
	mptScint->AddProperty("RINDEX", energy, rindexScint, 5);
	mptScint->AddProperty("ABSLENGTH", energy, absorLenScint, 5, false, true);
	mptScint->AddProperty("SCINTILLATIONCOMPONENT1", energy, spectScint, 5);
	mptScint->AddConstProperty("SCINTILLATIONYIELD", LYieldScint);
	mptScint->AddConstProperty("RESOLUTIONSCALE", 0.0);
	mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", decaytScint);
	mptScint->AddConstProperty("SCINTILLATIONYIELD1", 1.);

    //NaI->SetMaterialPropertiesTable(mptNaI);

	Scint->SetMaterialPropertiesTable(mptScint);

    worldMat->SetMaterialPropertiesTable(mptWorld);

    mirrorSurface = new G4OpticalSurface("mirrorSurface");

    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetFinish(ground);
    mirrorSurface->SetModel(unified);

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    
    mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 5);

    mirrorSurface->SetMaterialPropertiesTable(mptMirror);

	//Mirror Scint
	mirrorCoatScint = new G4OpticalSurface("mirrorCoatScint");

	mirrorCoatScint->SetType(dielectric_metal);
	mirrorCoatScint->SetFinish(ground);
	mirrorCoatScint->SetModel(unified);

    G4double reflectivityScint[2] = {0.98, 0.98};

	G4MaterialPropertiesTable *mptMirrorScint = new G4MaterialPropertiesTable();
	mptMirrorScint->AddProperty("REFLECTIVITY", energy, reflectivityScint, 5);
	
	mirrorCoatScint->SetMaterialPropertiesTable(mptMirrorScint);

	//Mirror SiPM
	mirrorCoatSiPM = new G4OpticalSurface("mirrorCoatSiPM");

	mirrorCoatSiPM->SetType(dielectric_dielectric);
	mirrorCoatSiPM->SetFinish(groundfrontpainted);
	mirrorCoatSiPM->SetModel(unified);

	G4double reflectivitySiPM[2] = {0.0, 0.0};

	G4MaterialPropertiesTable *mptMirrorSiPM = new G4MaterialPropertiesTable();
	mptMirrorSiPM->AddProperty("REFLECTIVITY", energy, reflectivitySiPM, 5);

	mirrorCoatSiPM->SetMaterialPropertiesTable(mptMirrorSiPM);

	//Boundary Scint and SiPM
	opBoundary = new G4OpticalSurface("Boundary");

	opBoundary->SetType(dielectric_dielectric);
	opBoundary->SetFinish(Rough_LUT);
	opBoundary->SetModel(unified);
}

void MyDetectorConstruction::ConstructCherenkov()
{
    solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

    logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicalRadiator");

    G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);

    fScoringVolume = logicRadiator;

    physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.30*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);

    //solidRadiator2 = new G4Box("solidRadiator2", 0.4*m, 0.4*m, 0.01*m);

    //logicRadiator2 = new G4LogicalVolume(solidRadiator2, Aerogel, "logicalRadiator2");

    //physRadiator2 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.20*m), logicRadiator2, "physRadiator2", logicWorld, false, 0, true);

    solidDetector = new G4Box("solidDetector", xWorld/nRows, yWorld/nCols, 0.01*m);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    for(G4int i = 0; i < nRows; i++)
    {
        for(G4int j = 0; j < nCols; j++)
        {
            physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/nRows, -0.5*m+(j+0.5)*m/nCols, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*nCols, true);        
        }    
    }
}

void MyDetectorConstruction::ConstructScintillator()
{
    solidScintillator = new G4Box("solidScintillator", 5*cm/2, 5*cm/2, 1*cm/2);

    logicScintillator = new G4LogicalVolume(solidScintillator, Scint, "logicalScintillator");

    solidDetector = new G4Box("solidDetector", 6*mm/2, 6*mm/2, 1*mm/2);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    fScoringVolume = logicScintillator;

    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., -1.1*cm/2), logicDetector, "physDetector", logicWorld, false, 0, true);

    G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicScintillator, mirrorCoatScint);

    G4LogicalSkinSurface *skinSiPM = new G4LogicalSkinSurface("skinSiPM", logicDetector, mirrorCoatSiPM);

    G4LogicalBorderSurface* SiPMSurface = new G4LogicalBorderSurface("SiPMSurface", physScintillator, physDetector, opBoundary);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    if(isCherenkov)
        ConstructCherenkov();

    if(isScintillator)
        ConstructScintillator();

    return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    if(logicDetector != NULL)
        logicDetector->SetSensitiveDetector(sensDet);
}
