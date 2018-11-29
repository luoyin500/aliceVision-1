// This file is part of the AliceVision project.
// Copyright (c) 2017 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/mvsData/StaticVector.hpp>
#include <aliceVision/mvsUtils/MultiViewParams.hpp>
#include <aliceVision/mesh/Mesh.hpp>

namespace aliceVision {

class Point3d;

namespace mvsUtils {
class PreMatchCams;
} // namespace mvsUtils

namespace mesh {


void filterLargeEdgeTriangles(Mesh* me, float avelthr);

void meshPostProcessing(Mesh*& inout_mesh, StaticVector<StaticVector<int>*>*& inout_ptsCams, StaticVector<int>& usedCams,
                      mvsUtils::MultiViewParams& mp, mvsUtils::PreMatchCams& pc,
                      const std::string& resultFolderName,
                      StaticVector<Point3d>* hexahsToExcludeFromResultingMesh, Point3d* hexah);

} // namespace mesh
} // namespace aliceVision
