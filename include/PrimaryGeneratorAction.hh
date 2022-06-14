//
// ********************************************************************
// 1.00 IS, FRM2  
// *                                                                  *
// ********************************************************************
//
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"


class PrimaryGenerator;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event*);
  
private:
    PrimaryGenerator* fPrimaryGenerator;
    G4ParticleGun* particleGun;

};

#endif


