//
// ********************************************************************
// *                                                                  *
// * This file is part of the TOPAS-nBio extensions to the            *
// *   TOPAS Simulation Toolkit.                                      *
// * The TOPAS-nBio extensions are freely available under the license *
// *   agreement set forth at: https://topas-nbio.readthedocs.io/     *
// *                                                                  *
// ********************************************************************
//

#ifndef TsSonata_hh
#define TsSonata_hh

#include "TsNeuroMorpho.hh"
#include "TsVGeometryComponent.hh"

struct Synapse
{
	G4int fromId;
	G4int toId;
	G4int synType; // 0 = excitatory, 1 = inhibitory
};

class TsSonata : public TsVGeometryComponent
{
public:
	TsSonata(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
		TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~TsSonata();

	G4VPhysicalVolume* Construct();
	void ResolveParameters();

private:
	G4int TotalNodes;
	G4int TotalEdges;

	// HDF5 nodes filename
	G4String fNodesFileName;
	// HDF5 edges filename
	G4String fEdgesFileName;

	std::vector<TsNeuroMorpho> fNeurons;
	std::vector<Synapse> fSynapses;
	std::vector<G4String> fCellTypes;
};

#endif