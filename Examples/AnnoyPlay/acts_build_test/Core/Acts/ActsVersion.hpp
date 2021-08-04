// This file is part of the Acts project.
//
// Copyright (C) 2016-2020 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

//  Caution: this is the only Acts header that is guaranteed
//  to change with every Acts release. Including this header
//  will cause a recompile every time a new Acts version is
//  used.

namespace Acts {

// clang-format does not like the CMake  replacement variables
// clang-format off
constexpr unsigned int VersionMajor = 9u;
constexpr unsigned int VersionMinor = 9u;
constexpr unsigned int VersionPatch = 9u;
// clang-format on
constexpr unsigned int Version =
    10000u * VersionMajor + 100u * VersionMinor + VersionPatch;
constexpr const char* const CommitHash = "56f63b24071d36bc55d003233bc5cf93144b27ec-dirty";
constexpr const char* const CommitHashShort = "56f63b240-dirty";

}  // namespace Acts
