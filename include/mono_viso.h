#ifndef MONOVISUALODOMETRY_H
#define MONOVISUALODOMETRY_H

#include "viso.h"
#include "featureType.h"

namespace me{

//! Visual Odometry class for stereovision

/*! This class compute motion from monocular images.
    It computes and extract the rotation and translation from the Essential matrix using the OpenCV library.
    A RANSAC outlier rejection scheme is used to keep only good matches.
 */
class MonoVisualOdometry : public VisualOdometry
{

public:
    //! Mono parameters
    /*! contains calibration parameters and info about the method used. */
    struct parameters  {
        bool ransac;                //! use of RANSAC or not.
        double prob;                //! level of confidence for the estimation (probability).
        double  inlier_threshold;   //! inlier threshold for RANSAC.
        double f;                   //! focal length of the camera.
        double cu;                  //! principal point.
        double cv;                  //! principal point.
        parameters () {
            ransac=true;
            prob=0.999;
            inlier_threshold = 1.0;
            f=1;
            cu=0;
            cv=0;
        }
    };
        /*! Main constructor. Takes a set of stereo parameters as input. */
        MonoVisualOdometry(parameters param=parameters());

        /*! Function processing a set of matches. Estimate the Essential matrix and extract R and t.
            Returns true if a motion is successfuly computed, false otherwise. */
        bool process(const std::vector<StereoMatch<cv::Point2f>>& matches);
        virtual cv::Mat getMotion(){return m_Rt;} //!< Returns the Transformation matrix.
        std::vector<int> getInliersIdx(){return m_inliers;} //!< Returns the set of inlier indices.
        cv::Mat getEssentialMat(){return m_E;} //!< Returns the Essential matrix.

    private:
        parameters m_param;         //!< Mono parameters.
        cv::Mat m_Rt;               //!< Transformation matrix.
        cv::Mat m_E;                //!< Essential matrix.
        std::vector<int> m_inliers; //!< List of inliers.
};

}

#endif // MONOVISUALODOMETRY_H
