//******************************************************************************
// PowtexConstruction.hh
//
// 1.00 IS, ESS, 2022:  First version.
//******************************************************************************
//

class G4VPhysicalVolume;
class G4Material;

#include "G4VUserDetectorConstruction.hh"

class PowtexConstruction : public G4VUserDetectorConstruction
{
  public:
  
    PowtexConstruction();
    ~PowtexConstruction();
    
    G4VPhysicalVolume* Construct();

  private:
     
     void DefineMaterials();
     
     #include "PowtexMantelConstruct.ihh"
     #include "PowtexMaterials.ihh"
     
     G4Material*  fMaterial;   
  

};

