// This file is part of the Acts project.
//
// Copyright (C) 2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <annoylib.h>

#include "ActsExamples/ANN/ApproximateNearestNeighborAlgorithm.hpp"


namespace ActsExamples {
    ApproximateNearestNeighborAlgorithm::ApproximateNearestNeighborAlgorithm(
	ApproximateNearestNeighborAlgorithm::Config cfg,
	Acts::Logging::Level lvl) :
	ActsExamples::BareAlgorithm("ApproximateNearestNeighborAlgorithm", lvl),
	m_cfg(std::move(cfg))
    {}

    ActsExamples::ProcessCode ApproximateNearestNeighborAlgorithm::execute(
	const ActsExamples::AlgorithmContext& ctx) const {
    }
    
}
