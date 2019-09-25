#ifndef TRUNMANAGER_HH
#define TRUNMANAGER_HH

#include "G4RunManager.hh"
#include "TMessenger.hh"

class TRunManager : public G4RunManager
{
  public:
    TRunManager();
    virtual ~TRunManager();

    virtual void InitializeGeometry();
    virtual void InitializePhysics();
    virtual void BeamOn(G4int numEvents, const char *macroFile=0, G4int numSelect=-1);

    void Run(int argc=0, char **argv=nullptr, const G4String &type="");

    void BuildInitsAndActions();

    /// Suppress String Stream Buffer
    void SetSuppressSSBInitGeometry(bool val) { fSuppressSSBInitGeometry = val; }
    void SetSuppressSSBInitPhysic(bool val)   { fSuppressSSBInitPhysics = val; }
    void SetSuppressSSBBeamO(bool val)        { fSuppressSSBBeamOn = val; }

  private:
    TMessenger *fMessenger = nullptr;

    bool fSuppressSSBInitGeometry = true;
    bool fSuppressSSBInitPhysics = true;
    bool fSuppressSSBBeamOn = true;
};

#endif
