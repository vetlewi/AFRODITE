#ifndef CO60_SOURCE_GENERATOR
#define CO60_SOURCE_GENERATOR

#include <globals.hh>
#include <Randomize.hh>

#include <vector>

class G4ParticleGun;
class G4Event;

class Co60EventGenerator
{

public:
    Co60EventGenerator(G4ParticleGun *);
    ~Co60EventGenerator() = default;

    void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *fParticleGun;
    std::vector<double> distribution;
    G4RandGeneral randAngularDist;

    // Get a random angle from the distribution
    double randAng();
};

#endif // CO60_SOURCE_GENERATOR