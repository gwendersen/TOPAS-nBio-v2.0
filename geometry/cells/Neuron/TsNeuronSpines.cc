#include "TsNeuronSpines.hh"

#include "TsParameterManager.hh"
#include "G4VPhysicalVolume.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "G4PVPlacement.hh"

#include <math.h>
#include <stdlib.h>

using namespace std;

G4LogicalVolume* TsNeuronSpines::EnsureSpineLV(const TsNeuronSpineOptions& opts)
{
	auto neck = new G4Tubs("SpineNeck_Solid", 0.0, opts.neckR, 0.5*opts.neckL, 0.0, twopi);
	auto head = new G4Orb("SpineHead_Solid", opts.headR);

	auto mat = G4Material::GetMaterial(opts.material, /*warning*/true);

	auto neckLV = new G4LogicalVolume(neck, mat, "SpineNeck_LV");
	auto headLV = new G4LogicalVolume(head, mat, "SpineHead_LV");

	auto spineLV = new G4LogicalVolume(new G4Box("SpineWrap_Solid", opts.headR, opts.headR, 0.5*opts.neckL + opts.headR), mat, "Spine_LV");

	new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, 0.5*opts.neckL - (0.5*opts.neckL + opts.headR)), neckLV, "Neck_PV", spineLV, false, 0);
	new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, 0.5*opts.neckL), headLV, "Head_PV", spineLV, false, 0);

	return spineLV;
};

void TsNeuronSpines::Populate(const std::vector<DendriteSegment>& segments, const TsNeuronSpineOptions& opts)
{
	auto spineLV = EnsureSpineLV(opts);
	std::mt19937 rng(opts.seed);
	std::uniform_real_distribution<> U(0.0, 1.0);

	for (const auto& seg : segments) {
		if (opts.onApicalOnly && !seg.isApical) continue;
		if (!opts.onBasal && seg.isBasal) continue;

		const G4ThreeVector u = (seg.p1 - seg.p0).unit();
		G4ThreeVector v = u.orthogonal().unit();
		G4ThreeVector w = u.cross(v).unit();

		const double L = (seg.p1 - seg.p0).mag() / CLHEP::um; // micrometers
		const double meanN = std::max(0.0, opts.densityPerMicron * L);
		const int N = CLHEP::RandPoissonQ::shoot(meanN);

		if (N == 0) continue;
		if (N > 50) {
			auto* param = new SpineParam(spineLV, seg, opts, rng);
			new G4PVParameterised("SpineParamPV", spineLV, seg.segmentLV, kUndefined, N, param, false);
		} else {
			for (int i=0; i < N; i++) {
				const double sL = U(rng) * (seg.p1 - seg.p0).mag();
				const double phi = 2*M_PI*U(rng);
				G4ThreeVector rhat = std::cos(phi)*v + std::sin(phi)*w;
				G4ThreeVector pos = seg.p0 + u*sL + rhat*(seg.radius + opts.neckR);
				G4RotationMatrix* R = new G4RotationMatrix();
				R->setRows(rhat, w.cross(rhat).unit(), rhat.cross(w.cross(rhat)).unit());
				new G4PVPlacement(R, pos, spineLV, "SpinePV", seg.segmentLV, false, i);
			}
		}
	}
};