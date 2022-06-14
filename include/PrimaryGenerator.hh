//
// ********************************************************************
// 1.00 IS, ESS  
// *                                                                  *
// ********************************************************************
//
//

#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VPrimaryGenerator.hh"

class G4Event;

class PrimaryGenerator : public G4VPrimaryGenerator
{
public:
  PrimaryGenerator();
  ~PrimaryGenerator();
  
public:
  virtual void GeneratePrimaryVertex(G4Event*);
  G4int GetNoPrimaries() const;
 
private:
 
  G4int fprimaries;
  
};

 inline G4int PrimaryGenerator::GetNoPrimaries() const { return fprimaries;}
 
#endif


