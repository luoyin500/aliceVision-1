// This file is part of the AliceVision project.
// Copyright (c) 2016 AliceVision contributors.
// Copyright (c) 2012 openMVG contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

namespace aliceVision {

namespace sfmData {
class SfMData;
} // namespace sfmData

namespace sfm {

/**
 * @brief colorizeTracks Add the associated color to each 3D point of
 * the sfm_data, using the track to determine the best view from which
 * to get the color.
 * @param[in,out] sfmData The container of the data
 * @return true if everything went well
 */
bool colorizeTracks(sfmData::SfMData& sfmData);

} // namespace sfm
} // namespace aliceVision
