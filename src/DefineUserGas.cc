//******************************************************************************
// DefineUserGas.cc
//
// 1.00 IS, FRM2, 2013:  First version.
//******************************************************************************
//
#include "DefineUserGas.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4NistMaterialBuilder.hh"
#include "G4NistElementBuilder.hh"
#include "G4MaterialTable.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

#include <vector>
#include "globals.hh"
#include "G4ios.hh"

DefineUserGas::DefineUserGas()
{;}

DefineUserGas::~DefineUserGas()
{;}

G4double DefineUserGas::GetUserGasDensity(const std::vector<G4Material*>& mats,
                                              const std::vector<G4double>& frac)
{
 
 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 
 if (mat_size ==0) {
     G4cout<< " Please input the constituents of the new gas! "
	   << G4endl;
	   return 0;
	   }

 for (G4int i=0; i<mat_size; i++){
 	G4double rho = mats[i]->GetDensity()/(g/cm3);
 	rhoGas += frac[i]*rho;         //density compound material
      } 
 
// G4cout<<"**********************************************"<<G4endl;
// G4cout<<"Output DefineUserGas: density gas mixture ="<<rhoGas<<" g/cm3"<<G4endl;
// G4cout<<"density gas mixture ="<<G4BestUnit(rho*mg/cm3,"Volumic Mass")<<G4endl;
// G4cout<<"**********************************************"<<G4endl;

 return rhoGas;
 
 
}

G4double DefineUserGas::GetUserGasZeff(const std::vector<G4Material*>& mats,
                                           const std::vector<G4double>& frac)
{

 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 G4double fracT = 0.;
 G4double Zeff = 0.;
 
 if (mat_size ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;
	   }

 for (G4int i=0; i<mat_size; i++){
  	fracT += frac[i];         //density compound material
      } 
 
 if (fracT > 1) {     
//     G4cout<< " The sum of the gas mass fractions is greater than 1!"
//	   << G4endl;
	   return -1;
	   }
 else {
 	for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
 		rhoGas += frac[i]*rho;         //density compound material
        	Zeff += frac[i]*rho/rhoGas*GetUserZeff(mats[i]);
      		}
      
// 	G4cout<<"**********************************************"<<G4endl;
// 	G4cout<<"Output DefineUserGas: Zeff gas mixture ="<<Zeff<<G4endl;
// 	G4cout<<"**********************************************"<<G4endl;
 
 	return Zeff;
	}
}

G4double DefineUserGas::GetUserGasAeff(const std::vector<G4Material*>& mats,
                                           const std::vector<G4double>& frac)
{
 
 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 G4double fracT = 0.;
 G4double Aeff = 0.;
 
 if (mat_size ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;
	   }

 for (G4int i=0; i<mat_size; i++){
  	fracT += frac[i];         //density compound material
      } 
 
 if (fracT > 1) {     
//     G4cout<< " The sum of the gas mass fractions is greater than 1!"
//	   << G4endl;
	   return -1;
	   }
 else {
	 for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
 		rhoGas += frac[i]*rho;         //density compound material
        	Aeff += frac[i]*rho/rhoGas*GetUserAeff(mats[i]);
      		}

// 	G4cout<<"**********************************************"<<G4endl;
// 	G4cout<<"Output DefineUserGas: Aeff gas mixture ="<<Aeff<<G4endl;
// 	G4cout<<"**********************************************"<<G4endl;

 	return Aeff;
	}
}

G4double DefineUserGas::GetUserAeff(G4Material *mat)
{
 
 G4double aeff=0.;
 
     const G4int nom = mat->GetNumberOfElements();
 
 	for (G4int i=0; i<nom; i++){
  	G4double a = mat->GetElement(i)->GetA()/(g/mole);
  	G4double massFraction = mat->GetFractionVector()[i];
  	         aeff += a*massFraction;
		 
//        G4cout<<"**********************************************"<<G4endl;
//	G4cout<<"Output DefineUserGas: mat ="<<mat->GetElement(i)->GetName()<<" A="<<a<<" mass fract="<<massFraction<<G4endl;
	}
 
	G4cout<<" Output DefineUserGas: Aeff mat ="<<aeff<<G4endl;
 return aeff; 
}

G4double DefineUserGas::GetUserZeff(G4Material *mat)
{
 
 G4double zeff=0.;
 
     const G4int nom = mat->GetNumberOfElements();
 
 	for (G4int i=0; i<nom; i++){
  	G4double z = mat->GetElement(i)->GetZ();
  	G4double massFraction = mat->GetFractionVector()[i];
  	         zeff += z*massFraction;
		 
//        G4cout<<"**********************************************"<<G4endl;
//	G4cout<<"Output DefineUserGas: mat ="<<mat->GetElement(i)->GetName()<<" Z ="<<z<<" mass fract="<<massFraction<<G4endl;
	}
 
//	G4cout<<" Output DefineUserGas: Zeff mat ="<<zeff<<G4endl;
 return zeff; 
}


G4Material* DefineUserGas::ConstructUserGas(const G4String& mname, 
			                        const std::vector<G4Element*>& name,
                                                const std::vector<G4int>& nbAtoms,
		                                G4double temp,G4double pres)
{
 
 G4NistManager* man = G4NistManager::Instance();
  
 G4int els = name.size();
 
 G4State state = kStateGas;
 
 G4double massPerMole = 0.;
 
 if (els ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;
	   }
	   
 
 for (G4int i=0; i<els; i++){
                G4double zelem = name[i]->GetZ();
                massPerMole += nbAtoms[i] * man->GetAtomicMassAmu(int(zelem))*amu_c2;
		}
 
 G4double rho = massPerMole/(Avogadro*k_Boltzmann*temp/pres);  		
		
// G4cout<<"**********************************************"<<G4endl;
// G4cout<<"Output ConstructUserGas: density new gas ="<<rho<<G4endl;
// G4cout<<"**********************************************"<<G4endl;

 G4Material* mat = new G4Material(mname,rho*g/cm3,els,state,temp,pres);

 for (G4int i=0; i<els; i++){
 		mat->AddElement(name[i],nbAtoms[i]);
		}
		
 return mat;

}
 
