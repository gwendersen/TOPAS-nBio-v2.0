#ifndef TsNeuronSpines_hh
#define TsNeuronSpines_hh

#include "TsVGeometryComponent.hh"

struct TsNeuronSpineOptions
{
	G4double densityPerMicron;
	G4String shape;
	G4double neckL, neckR, headR;
	G4String material;
	G4int seed;
};

struct DendriteSegment
{
	G4LogicalVolume* segmentLV;
	G4ThreeVector p0, p1;
	G4double radius;
	G4bool isApical, isBasal;
};

class TsNeuronSpines
{
public:
	TsNeuronSpines(TsParameterManager* pm, TsVGeometryComponent* owner, G4LogicalVolume* worldLV);
	~TsNeuronSpines();

	void Populate(const std::vector<DendriteSegment>& segments, const TsNeuronSpineOptions& opts);

private:
	G4LogicalVolume* EnsureSpineLV(const TsNeuronSpineOptions& opts);



};

#endif