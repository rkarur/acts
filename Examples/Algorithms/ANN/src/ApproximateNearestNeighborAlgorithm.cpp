// This file is part of the Acts project.
//
// Copyright (C) 2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <annoylib.h>

#include "ActsExamples/ANN/ApproximateNearestNeighborAlgorithm.hpp"
#include "ActsExamples/EventData/Trajectories.hpp"



namespace ActsExamples {
    ApproximateNearestNeighborAlgorithm::ApproximateNearestNeighborAlgorithm(
	ApproximateNearestNeighborAlgorithm::Config cfg,
	Acts::Logging::Level lvl) :
	ActsExamples::BareAlgorithm("ApproximateNearestNeighborAlgorithm", lvl),
	m_cfg(std::move(cfg))
    {
	if (m_cfg.inputSpacePoints.empty())
	    throw std::invalid_argument("Missing inputSpacePoints collection");
	if (m_cfg.inputSourceLinks.empty())
	    throw std::invalid_argument("Missing inputSourceLinks collection");
	if (m_cfg.inputMeasurements.empty())
	    throw std::invalid_argument("Missing inputMeasurements collection");
	if (m_cfg.outputProtoTracks.empty())
	    throw std::invalid_argument("Missing outputProtoTracks collection");
	if (m_cfg.outputTrajectories.empty())
	    throw std::invalid_argument("Missing outputTrajectories collection");


	for (size_t i = 0; i < m_cfg.queries; i++) {
	    SeedingAlgorithm::Config seedingCfg {m_cfg.seedingCfg};
	    seedingCfg.inputSpacePoints = {m_cfg.inputSpacePoints + std::to_string(i)};
	    seedingCfg.outputSeeds = m_cfg.seedingCfg.outputSeeds + std::to_string(i);
	    seedingCfg.outputProtoTracks = m_cfg.outputProtoTracks + std::to_string(i);
	    m_seedFinders.push_back(SeedingAlgorithm(seedingCfg, lvl));

	    TrackParamsEstimationAlgorithm::Config paramsEstimationCfg {m_cfg.paramsEstimationCfg};
	    paramsEstimationCfg.inputSeeds = seedingCfg.outputSeeds;
	    paramsEstimationCfg.inputProtoTracks = seedingCfg.outputProtoTracks;
	    paramsEstimationCfg.outputTrackParameters = paramsEstimationCfg.outputTrackParameters + std::to_string(i);
	    paramsEstimationCfg.outputProtoTracks = paramsEstimationCfg.outputProtoTracks + std::to_string(i);
	    m_paramsEstimators.push_back(TrackParamsEstimationAlgorithm(paramsEstimationCfg, lvl));

	    TrackFindingAlgorithm::Config trackFindingCfg {m_cfg.trackFindingCfg};
	    trackFindingCfg.inputInitialTrackParameters = paramsEstimationCfg.outputTrackParameters;
	    trackFindingCfg.outputTrajectories = m_cfg.outputTrajectories + std::to_string(i);
	    m_trackFinders.push_back(TrackFindingAlgorithm(trackFindingCfg, lvl));
	}


    }

    ActsExamples::ProcessCode ApproximateNearestNeighborAlgorithm::execute(
	const ActsExamples::AlgorithmContext& ctx) const {
	const SimSpacePointContainer& allsps = ctx.eventStore.get<SimSpacePointContainer>(m_cfg.inputSpacePoints);
	AnnIndex idx = buildIndex(allsps); // TODO
	for (size_t i = 0; i < m_cfg.queries; i++) {
	    std::string key = m_cfg.inputSpacePoints + std::to_string(i);
	    SimSpacePointContainer sps = annQuery(idx, allsps); // TODO
	    ctx.eventStore.add(key, std::move(sps));
	    m_seedFinders.at(i).execute(ctx);
	    m_paramsEstimators.at(i).execute(ctx);
	    m_trackFinders.at(i).execute(ctx);
	}

	// Consolidate information for the performance writers
	ProtoTrackContainer ptracks;
	TrajectoriesContainer trajs;
	for (size_t i = 0; i < m_cfg.queries; i++) {
	    std::string i_ptracks_key = m_cfg.outputProtoTracks + std::to_string(i);
	    const ProtoTrackContainer& i_ptracks = ctx.eventStore.get<ProtoTrackContainer>(i_ptracks_key);
	    ptracks.insert(ptracks.end(), i_ptracks.begin(), i_ptracks.end());

	    std::string i_trajs_key = m_cfg.outputTrajectories + std::to_string(i);
	    const TrajectoriesContainer& i_trajs = ctx.eventStore.get<TrajectoriesContainer>(i_trajs_key);
	    trajs.insert(trajs.end(), i_trajs.begin(), i_trajs.end());
	}
	ctx.eventStore.add(m_cfg.outputProtoTracks, std::move(ptracks));
	ctx.eventStore.add(m_cfg.outputTrajectories, std::move(trajs));

	return ActsExamples::ProcessCode::SUCCESS;
    }

    ApproximateNearestNeighborAlgorithm::AnnIndex
    ApproximateNearestNeighborAlgorithm::buildIndex(const SimSpacePointContainer& sps) const
    {
	// TODO
	return nullptr;
    }

    SimSpacePointContainer
    ApproximateNearestNeighborAlgorithm::annQuery(const AnnIndex&, const SimSpacePointContainer& sps) const
    {
	// TODO
	SimSpacePointContainer dummy {sps};
	return dummy;
    }
    
}
