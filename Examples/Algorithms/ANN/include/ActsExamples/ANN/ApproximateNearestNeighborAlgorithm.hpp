// This file is part of the Acts project.
//
// Copyright (C) 2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <annoylib.h>
#include <kissrandom.h>

#include "ActsExamples/Framework/BareAlgorithm.hpp"
#include "ActsExamples/Framework/WhiteBoard.hpp"
#include "ActsExamples/TrackFinding/SeedingAlgorithm.hpp"
#include "ActsExamples/TrackFinding/TrackFindingAlgorithm.hpp"
#include "ActsExamples/TrackFinding/TrackParamsEstimationAlgorithm.hpp"

namespace ActsExamples {
    class ApproximateNearestNeighborAlgorithm final : public BareAlgorithm {
    public:
	struct Config {
	    std::string inputSpacePoints;
	    std::string inputSourceLinks;
	    std::string inputMeasurements;
	    std::string outputProtoTracks;
	    std::string outputTrajectories;
	    size_t bucketSize;
	    size_t queries;
	    SeedingAlgorithm::Config seedingCfg;
	    TrackParamsEstimationAlgorithm::Config paramsEstimationCfg;
	    TrackFindingAlgorithm::Config trackFindingCfg;
	};

	ApproximateNearestNeighborAlgorithm(Config cfg, Acts::Logging::Level lvl);

	ActsExamples::ProcessCode execute(
	    const ActsExamples::AlgorithmContext& ctx) const final;

    private:
	Config m_cfg;

	typedef AnnoyIndex<int, float, Angular, Kiss32Random,AnnoyIndexSingleThreadedBuildPolicy> AnnIndex; // TODO Change this!! // Changed!
	AnnIndex buildIndex(const SimSpacePointContainer&, std::vector<std::vector<float>>&) const;
	SimSpacePointContainer annQuery(const AnnIndex&, const SimSpacePointContainer&, int, int) const;

	// Note: gotta find a better way?
	std::vector<SeedingAlgorithm> m_seedFinders;
	std::vector<TrackParamsEstimationAlgorithm> m_paramsEstimators;
	std::vector<TrackFindingAlgorithm> m_trackFinders;
    };
}
