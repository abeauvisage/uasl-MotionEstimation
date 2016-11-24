#ifndef STEREO_VISO_H
#define STEREO_VISO_H

#include <iostream>

#include "viso.h"
#include "featureType.h"

class StereoVisualOdometry: public VisualOdometry {

public:

    enum Method {GN, LM};

    struct parameters  {
        double  baseline;
        Method method;
        int n_ransac;
        bool ransac;
        double  inlier_threshold;
        bool    reweighting;
        double f1,f2;
        double cu1,cu2;
        double cv1,cv2;
        double step_size;
        double eps,e1,e2,e3,e4;
        int max_iter;
        parameters () {
            method = GN;
            f1=1;f2=1;
            cu1=0;cu2=0;
            cv1=0;cv2=0;
            baseline = 1.0;
            n_ransac = 200;
            inlier_threshold = 2.0;
            ransac=true;
            reweighting = true;
            step_size = 1;
            eps = 1e-8;
            e1 = 1e-3;
            e2 = 1e-3;
            e3 = 1e-1;
            e4 = 1e-1;
            max_iter=20;
        }
    };


    StereoVisualOdometry(parameters param=parameters());
    ~StereoVisualOdometry(){};

    bool process(const std::vector<StereoOdoMatches<cv::Point2f>>& matches);
    virtual cv::Mat getMotion();

    std::vector<cv::Point3d> getPts3D(){return pts3D;}
    std::vector<int> getInliers_idx(){return inliers_idx;}
    void computeReprojErrors(const std::vector<StereoOdoMatches<cv::Point2f>>& matches, const std::vector<int>& inliers);

private:

    std::vector<cv::Point3d> pts3D;
    std::vector<int> inliers_idx; // inliers indexes

    parameters m_param;

    cv::Mat J;
    cv::Mat x;
    cv::Mat observations;
    cv::Mat predictions;
    cv::Mat residuals;


    bool optimize(const std::vector<StereoOdoMatches<cv::Point2f>>& matches, const std::vector<int32_t>& selection, bool weight);
    void projectionUpdate(const std::vector<StereoOdoMatches<cv::Point2f>>& matches, const std::vector<int32_t>& selection, bool weight);
    cv::Mat applyFunction(const std::vector<StereoOdoMatches<cv::Point2f>>& matches, cv::Mat& x_, const std::vector<int32_t>& selection);
    std::vector<int> computeInliers(const std::vector<StereoOdoMatches<cv::Point2f>>& matches);
    std::vector<int> randomIndexes(int nb_samples, int nb_tot);
};

#endif // VSTEREO_VISO_H
