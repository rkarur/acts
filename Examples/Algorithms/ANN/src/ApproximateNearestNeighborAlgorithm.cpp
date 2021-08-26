// This file is part of the Acts project.
//
// Copyright (C) 2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include "ActsExamples/ANN/ApproximateNearestNeighborAlgorithm.hpp"
#include "ActsExamples/EventData/Trajectories.hpp"

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

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


	for (size_t i = 0; i < m_cfg.queries ; i++) {
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
    std::vector<std::vector<float>> s_points;
    std::vector<int> idxs_left(allsps.size());
    std::iota(idxs_left.begin(),idxs_left.end(),0);
	AnnIndex idx = buildIndex(allsps, s_points); // TODO
    std::vector<int> hits_idx; hits_idx.resize(s_points.size(),0);
//     std::cout << m_cfg.queries << "<--- \n";
    
    
//     duration<double, std::milli> time_diff;    
	for (size_t i = 0; i < m_cfg.queries ; i++) {
	    std::string key = m_cfg.inputSpacePoints + std::to_string(i);
///////        
        int hit_idx = rand() % allsps.size();
	    SimSpacePointContainer sps = annQuery(idx, allsps, hit_idx, m_cfg.bucketSize,idxs_left);
///////
//         SimSpacePointContainer sps;
//         if (idxs_left.size() != 0) {
//         int hit_idx = idxs_left[rand() % idxs_left.size()];
// 	    sps = annQuery_v1(idx, allsps, hit_idx, m_cfg.bucketSize,idxs_left);}
//         else{
//         sps = {};
//         }
///////        
//         SimSpacePointContainer sps = allsps;
	    ctx.eventStore.add(key, std::move(sps));
//         auto t1 = high_resolution_clock::now();
	    m_seedFinders.at(i).execute(ctx);
	    m_paramsEstimators.at(i).execute(ctx);
	    m_trackFinders.at(i).execute(ctx);
//         auto t2 = high_resolution_clock::now();
//         time_diff += t2-t1;
	}
//     std::cout << "TIME TAKEN: " << time_diff.count() << " ms \n";
        
	// Consolidate information for the performance writers
	ProtoTrackContainer ptracks;
	TrajectoriesContainer trajs;
	for (size_t i = 0; i < m_cfg.queries ; i++) {
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
    ApproximateNearestNeighborAlgorithm::buildIndex(const SimSpacePointContainer& sps, std::vector<std::vector<float>>& s_points) const
    {
	// TODO
    AnnIndex obj(3);
//     s_points.clear();
//     s_points.resize(sps.size());
    for (size_t i = 0; i < sps.size(); i++){
//         s_points[i].resize(3);
//         s_points[i][0] = sps[i].x();
//         s_points[i][1] = sps[i].y();
//         s_points[i][2] = sps[i].z();
        //s_points[i].data()
        obj.add_item(i,sps[i].ptr);
    }
    obj.build(1);
	return obj;
    }

    SimSpacePointContainer
    ApproximateNearestNeighborAlgorithm::annQuery(const AnnIndex& IndexIQ, const SimSpacePointContainer& sps, int hit_id, int bucket_size, std::vector<int>& idxs_left) const
    {
	// TODO
// 	SimSpacePointContainer dummy {sps};
    SimSpacePointContainer subset = {};
    std::vector<int> nn_idx;
    std::vector<float> nn_dist;
    IndexIQ.get_nns_by_item(hit_id, bucket_size+1, -1, &nn_idx, &nn_dist);
    for (size_t i = 0; i < nn_idx.size(); i++){
        subset.push_back(sps[nn_idx[i]]);
//         idxs_union.insert(nn_idx[i]);
//         std::cout<<nn_idx[i]<<std::endl;
    } 

	return subset;
    }

    SimSpacePointContainer
    ApproximateNearestNeighborAlgorithm::annQuery_v1(const AnnIndex& IndexIQ, const SimSpacePointContainer& sps, int hit_id, int bucket_size, std::vector<int>& idxs_left) const
    {
	// TODO
// 	SimSpacePointContainer dummy {sps};
    SimSpacePointContainer subset = {};
    std::vector<int> nn_idx;
    std::vector<float> nn_dist;
    IndexIQ.get_nns_by_item(hit_id, bucket_size+1, -1, &nn_idx, &nn_dist);
    for (size_t i = 0; i < nn_idx.size(); i++){
        subset.push_back(sps[nn_idx[i]]);
    } 
    std::sort(nn_idx.begin(),nn_idx.end());
    std::vector<int> difference;
    std::set_difference(idxs_left.begin(), idxs_left.end(),nn_idx.begin(),nn_idx.end(),std::back_inserter(difference));
    idxs_left = difference;
//     std::cout<<idxs_left.size()<<"\n";
	return subset;
    }

}
