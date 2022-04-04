/// \file PIIActionInitialization.hh
/// \brief Definition of the PIIActionInitialization class

#ifndef PIIActionInitialization_h
#define PIIActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class PIIDetectorConstruction;
class PIIEventAction;
class PIISteppingAction;

/// Action initialization class.
///

class PIIActionInitialization : public G4VUserActionInitialization
{
  public:
    PIIActionInitialization(PIIDetectorConstruction*, PIIEventAction*, PIISteppingAction*);
    virtual ~PIIActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    PIIDetectorConstruction* fDetConstruction;
    PIIEventAction* fEventAction;
    PIISteppingAction* fStepAction;
};

#endif
