// Component for TsSonata
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
//Interface to HDF5 files in the SONATA format, which represent neurons (nodes) and synapses (edges) in neural circuit models.

#include "TsSonata.hh"

#include "TsParameterManager.hh"
#include "G4VPhysicalVolume.hh"

#include "G4UnionSolid.hh"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

TsSonata::TsSonata(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
		TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
	ResolveParameters();
}

TsSonata::~TsSonata()
{;}

void TsSonata::ResolveParameters() {
	
	// User defined parameters.

	// Directory containing NeuroMorpho SWC morphology files
	G4String morphoDirName = GetFullParmName("MorphologyDirName");
	if (!fPm->ParameterExists(morphoDirName)) {
		G4cerr << "Topas is exiting due to a serious error in geometry setup." << G4endl;
		G4cerr << "Parameter " << morphoDirName << " has to be specified for this scorer." << G4endl;
		exit(1);
	}
	G4String MorphologyDirName = fPm->GetStringParameter(morphoDirName);

	// HDF5 file containing individual neuron specifications
	G4String nodesName = GetFullParmName("NodesFileName");
	if (!fPm->ParameterExists(nodesName)) {
		G4cerr << "Topas is exiting due to a serious error in geometry setup." << G4endl;
		G4cerr << "Parameter " << nodesName << " has to be specified for this scorer." << G4endl;
		exit(1);
	}
	G4String NodesFileName = fPm->GetStringParameter(nodesName);

	// HDF5 file containing individual synapse specifications
	G4String edgesName = GetFullParmName("EdgesFileName");
	if (!fPm->ParameterExists(edgesName)) {
		G4cerr << "Topas is exiting due to a serious error in geometry setup." << G4endl;
		G4cerr << "Parameter " << edgesName << " has to be specified for this scorer." << G4endl;
		exit(1);
	}
	G4String EdgesFileName = fPm->GetStringParameter(edgesName);

	HLX = fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length");
	HLY = fPm->GetDoubleParameter(GetFullParmName("HLY"), "Length");
	HLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length");

	/********** Get Neurons **********
	// 1. advance to next row in nodes file
	// 2. obtain HLX, HLY, HLZ for this neuron; store in transient parameters
	//   e.g.,
	//		if(!fPm->ParameterExists(HL_)) {
	//			fPm->AddParameter(HL_)
	//		}
	// 3. check if morphology name is in fCellTypes. if not, retrieve matching SWC file and create a neuron with TsNeuroMorpho using HLX/Y/Z and NeuroMorphoFileName
	// 4. add TsNeuroMorpho to fNeurons
	**********/

	/********** insert following logic into Get Neurons loop
	// Read in each NeuroMorpho file
	try {
		for (const auto& entry : std::filesystem::directory_iterator(MorphologyDirName)) {
			// ensure entry is not a directory
			if (entry.is_regular_file()) {
				// ensure entry is a SWC file
				std::string ext = entry.path().extension().string();
				if (ext == ".swc") {
					// create a NeuroMorphoFileName parameter for this morphology file
					fPm->AddParameter("NeuroMorphoFileName", entry.path());
					// need to determine
				}
			}
		}
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
	}
	**********/
}
