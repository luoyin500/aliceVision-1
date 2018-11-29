// This file is part of the AliceVision project.
// Copyright (c) 2017 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/mvsData/Matrix3x3.hpp>
#include <aliceVision/mvsData/Point2d.hpp>
#include <aliceVision/mvsData/Point3d.hpp>
#include <aliceVision/mvsData/Pixel.hpp>
#include <aliceVision/mvsData/SeedPointCams.hpp>
#include <aliceVision/mvsData/StaticVector.hpp>
#include <aliceVision/mvsData/structures.hpp>

#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <string>

namespace aliceVision {
namespace mvsUtils {

enum class EFileType {
    P = 0,
    K = 1,
    iK = 2,
    R = 3,
    iR = 4,
    C = 5,
    iP = 6,
    har = 7,
    prematched = 8,
    seeds = 9,
    growed = 10,
    op = 11,
    occMap = 12,
    wshed = 13,
    nearMap = 14,
    seeds_prm = 15,
    seeds_flt = 16,
    img = 17,
    seeds_seg = 18,
    graphCutMap = 20,
    graphCutPts = 21,
    growedMap = 22,
    agreedMap = 23,
    agreedPts = 24,
    refinedMap = 25,
    seeds_sfm = 26,
    radial_disortion = 27,
    graphCutMesh = 28,
    agreedMesh = 29,
    nearestAgreedMap = 30,
    segPlanes = 31,
    agreedVisMap = 32,
    diskSizeMap = 33,
    imgT = 34,
    depthMap = 35,
    simMap = 36,
    mapPtsTmp = 37,
    depthMapInfo = 38,
    camMap = 39,
    mapPtsSimsTmp = 40,
    nmodMap = 41,
    D = 42,
};

class MultiViewParams
{
public:
    /// prepareDenseScene data
    std::string mvDir;
    /// global data prefix
    std::string prefix;
    /// camera projection matrix P
    std::vector<Matrix3x4> camArr;
    /// camera intrinsics matrix K: [focalx skew u; 0 focaly v; 0 0 1]
    std::vector<Matrix3x3> KArr;
    /// inverse camera intrinsics matrix K
    std::vector<Matrix3x3> iKArr;
    /// camera rotation matrix R
    std::vector<Matrix3x3> RArr;
    /// inverse camera rotation matrix R
    std::vector<Matrix3x3> iRArr;
    /// camera position C in world coordinate system
    std::vector<Point3d> CArr;
    /// K * R inverse matrix
    std::vector<Matrix3x3> iCamArr;
    ///
    std::vector<Point3d> FocK1K2Arr;
    /// number of cameras
    int ncams;
    /// cuda device id
    int CUDADeviceNo;
    ///
    float simThr;
    int g_border = 2;
    bool verbose;

    boost::property_tree::ptree _ini;

    MultiViewParams(const std::string& iniFile,
                    const std::string& depthMapFolder = "",
                    const std::string& depthMapFilterFolder = "",
                    bool readFromDepthMaps = false,
                    int downscale = 1,
                    StaticVector<CameraMatrices>* cameras = nullptr);

    ~MultiViewParams();

    inline int getViewId(int index) const
    {
        return _imagesParams.at(index).viewId;
    }

    inline int getOriginalWidth(int index) const
    {
        return _imagesParams.at(index).width;
    }

    inline int getOriginalHeight(int index) const
    {
        return _imagesParams.at(index).height;
    }

    inline int getOriginalSize(int index) const
    {
        return _imagesParams.at(index).size;
    }

    inline int getWidth(int index) const
    {
        return _imagesParams.at(index).width / getDownscaleFactor(index);
    }

    inline int getHeight(int index) const
    {
        return _imagesParams.at(index).height / getDownscaleFactor(index);
    }

    inline int getSize(int index) const
    {
        return _imagesParams.at(index).size / getDownscaleFactor(index);
    }
    inline const std::vector<imageParams>& getImagesParams() const
    {
        return _imagesParams;
    }
    inline const imageParams& getImageParams(int i) const
    {
        return _imagesParams.at(i);
    }

    inline int getDownscaleFactor(int index) const
    {
        return _imagesScale.at(index) * _processDownscale;
    }

    inline int getProcessDownscale() const
    {
        return _processDownscale;
    }

    inline int getMaxImageWidth() const
    {
        return _maxImageWidth;
    }

    inline int getMaxImageHeight() const
    {
        return _maxImageHeight;
    }

    inline int getNbCameras() const
    {
        return _imagesParams.size();
    }

    inline std::vector<double> getOriginalP(int index) const
    {
        std::vector<double> p44; // projection matrix (4x4) scale 1
        const Matrix3x4& p34 = camArr.at(index); // projection matrix (3x4) scale = getDownscaleFactor()
        const int downscale = getDownscaleFactor(index);
        p44.assign(p34.m, p34.m + 12);
        std::transform(p44.begin(), p44.begin() + 8, p44.begin(), std::bind1st(std::multiplies<double>(),downscale));
        p44.push_back(0);
        p44.push_back(0);
        p44.push_back(0);
        p44.push_back(1);
        return p44;
    }

    inline const std::string& getImageExtension() const
    {
        return _imageExt;
    }

    inline const std::string& getDepthMapFolder() const
    {
        return _depthMapFolder;
    }

    inline const std::string& getDepthMapFilterFolder() const
    {
        return _depthMapFilterFolder;
    }

    bool is3DPointInFrontOfCam(const Point3d* X, int rc) const;
    void getPixelFor3DPoint(Point2d* out, const Point3d& X, const Matrix3x4& P) const;
    void getPixelFor3DPoint(Point2d* out, const Point3d& X, int rc) const;
    void getPixelFor3DPoint(Pixel* out, const Point3d& X, int rc) const;
    double getCamPixelSize(const Point3d& x0, int cam) const;
    double getCamPixelSize(const Point3d& x0, int cam, float d) const;
    double getCamPixelSizeRcTc(const Point3d& p, int rc, int tc, float d) const;
    double getCamPixelSizePlaneSweepAlpha(const Point3d& p, int rc, int tc, int scale, int step) const;
    double getCamPixelSizePlaneSweepAlpha(const Point3d& p, int rc, StaticVector<int>* tcams, int scale, int step) const;

    double getCamsMinPixelSize(const Point3d& x0, std::vector<unsigned short>* tcams) const;
    double getCamsMinPixelSize(const Point3d& x0, StaticVector<int>& tcams) const;

    bool isPixelInImage(const Pixel& pix, int d, int camId) const;
    bool isPixelInImage(const Pixel& pix, int camId) const;
    bool isPixelInImage(const Point2d& pix, int camId) const;
    void decomposeProjectionMatrix(Point3d& Co, Matrix3x3& Ro, Matrix3x3& iRo, Matrix3x3& Ko, Matrix3x3& iKo, Matrix3x3& iPo, const Matrix3x4& P) const;

private:
    /// image params list (width, height, size)
    std::vector<imageParams> _imagesParams;
    /// image scale list
    std::vector<int> _imagesScale;
    /// downscale apply to input images during process
    int _processDownscale = 1;
    /// maximum width
    int _maxImageWidth = 0;
    /// maximum height
    int _maxImageHeight = 0;
    /// images extension
    std::string _imageExt = ".exr";
    /// depthMapEstimate data folder
    std::string _depthMapFolder;
    /// depthMapFilter data folder
    std::string _depthMapFilterFolder;
    /// use silhouettes
    bool _useSil = false;

    void initFromConfigFile(const std::string& iniFile);
    void loadCameraFile(int i, const std::string& fileNameP, const std::string& fileNameD);

    inline void resizeCams(int _ncams)
    {
        ncams = _ncams;
        camArr.resize(ncams);
        KArr.resize(ncams);
        iKArr.resize(ncams);
        RArr.resize(ncams);
        iRArr.resize(ncams);
        CArr.resize(ncams);
        iCamArr.resize(ncams);
        FocK1K2Arr.resize(ncams);
        _imagesScale.resize(ncams);
    }
};

} // namespace mvsUtils
} // namespace aliceVision
