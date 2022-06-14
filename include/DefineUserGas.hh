//******************************************************************************
// DefineUserGas.hh
//
// 1.00 IS, FRM2, 2013:  First version.
//******************************************************************************
//
#ifndef DefineUserGas_H
#define DefineUserGas_H 1

#include "G4Material.hh"
#include <vector>
#include "globals.hh"

class G4Material;
class G4Element;


class DefineUserGas 
{
  public:
    DefineUserGas();
    ~DefineUserGas();

  public:

     G4double GetUserGasDensity(const std::vector<G4Material*>& mats,
                                const std::vector<G4double>& frac);
				
     G4double GetUserAeff(G4Material *mat);
     G4double GetUserZeff(G4Material *mat);
     
     G4double GetUserGasAeff(const std::vector<G4Material*>& mats,
                             const std::vector<G4double>& frac);
			     
     G4double GetUserGasZeff(const std::vector<G4Material*>& mats,
                             const std::vector<G4double>& frac);
     
     G4Material* ConstructUserGas(const G4String& mname, 
			         const std::vector<G4Element*>& name,
                                 const std::vector<G4int>& nbAtoms,
				 G4double temp,
				 G4double pres);
				 
				  
};

#endif
