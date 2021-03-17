

#include <CADMesh/CADMesh.hh>
#include <G4VSolid.hh>
#include <G4UImanager.hh>

#include <chrono>
#include <G4UIcmdWithAString.hh>
#include <G4UImessenger.hh>
#include <G4GenericMessenger.hh>
#include <G4UserRunAction.hh>
#include <G4RunManager.hh>
#include <G4PhysListFactory.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4VUserActionInitialization.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

class FilePathMessenger : public G4UserRunAction
{
private:
    G4GenericMessenger *fMessenger;
    G4String fOutput;
public:

    FilePathMessenger()
        : fMessenger( new G4GenericMessenger(this, "/FilePath/", "FilePath setter") )
        {
            G4GenericMessenger::Command &fileCmd
                = fMessenger->DeclareProperty("output", fOutput);
        }

    void ShowValue()
    {
        std::cout << "Ouput path: " << fOutput << std::endl;
    }

};

class ActionInitializer : public G4VUserActionInitialization
{
private:
    FilePathMessenger *msg;
public:
    ActionInitializer() : msg( new FilePathMessenger ){}
    virtual ~ActionInitializer() { delete msg; }

    virtual void BuildForMaster() const {
        SetUserAction( msg );
    }
    virtual void Build() const {
        SetUserAction( msg );
    }

    void Print(){
        msg->ShowValue();
    }
};

class DetInit : public G4VUserDetectorConstruction
{
public:
    DetInit() = default;
    ~DetInit() override = default;

    virtual G4VPhysicalVolume* Construct(){
        auto WorldSize = 15.*m;
        G4Box* SolidWorld = new G4Box("World", WorldSize/2, WorldSize/2, WorldSize/2);
        G4Material* G4_Galactic_Material = G4Material::GetMaterial("G4_Galactic");
        G4LogicalVolume*
                LogicWorld = new G4LogicalVolume(SolidWorld,                //its solid
                                                 G4_Galactic_Material,      //its material
                                                 "World");                  //its name
        G4VPhysicalVolume*
                PhysiWorld = new G4PVPlacement(0,                        //no rotation
                                               G4ThreeVector(),          //at (0,0,0)
                                               LogicWorld,               //its logical volume
                                               "World",                  //its name
                                               0,                        //its mother  volume
                                               false,                    //no boolean operation
                                               0);
        return PhysiWorld; }
};

int main()
{
    G4RunManager * runManager = new G4RunManager;

    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = 0;
    G4String physName = "QGSP_BERT";
    // reference PhysicsList via its name
    phys = factory.GetReferencePhysList(physName);
    phys->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new DetInit);

    ActionInitializer* actionInitialization
            = new ActionInitializer();
    runManager->SetUserInitialization(actionInitialization);

    runManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    actionInitialization->Print();
    UImanager->ApplyCommand("/FilePath/output thisfile.root");
    actionInitialization->Print();





}

/*int main()
{
    G4ThreeVector offset_CLOVER_Shield_Body = G4ThreeVector(0*cm, 0*cm, 0*cm);
    CADMesh * mesh_CLOVER_Shield_Body = new CADMesh("../Mesh-Models/DETECTORS/CLOVER/Shield/Body/Body.ply", "PLY", mm, offset_CLOVER_Shield_Body, false);

    std::cout << "Starting long process... " << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    G4VSolid * Solid_CLOVER_Shield_Body = mesh_CLOVER_Shield_Body->TessellatedMesh();
    auto end = std::chrono::high_resolution_clock::now();

    auto time_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Total runtime: " << time_duration.count() << std::endl;
    return 0;
}*/