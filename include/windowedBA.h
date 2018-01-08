#ifndef WINDOWEDBA_H_INCLUDED
#define WINDOWEDBA_H_INCLUDED

#include <deque>
#include <vector>
#include <iostream>

#include <opencv2/core.hpp>

#include <utils.h>
#include <featureType.h>
#include <stereo_viso.h>
#include <mono_viso.h>

namespace me{

//! function computing Projection matrix from K, R and t
void compPMat(cv::InputArray _K, cv::InputArray _R, cv::InputArray _t, cv::OutputArray _P);
//! Global BA function using OpenCV Point2f
/*! solve BA problem with observations, 3D points and camera poses. Deprecated.*/
void solveWindowedBA(const std::deque<std::vector<cv::Point2f>>& observations, std::vector<ptH3D>& pts3D, std::vector<double>& weight, const MonoVisualOdometry::parameters& params, const cv::Mat& img, std::vector<me::Euld>& ori, std::vector<cv::Vec3d>& pos);
//! Global BA function using WBA_Points
/*! solve BA problem with observations, 3D points and camera poses*/
void solveWindowedBA(std::vector<WBA_Ptf>& pts, const cv::Matx33d& K, const cv::Mat& img, std::vector<me::Euld>& ori, std::vector<cv::Vec3d>& pos, int start, int window_size, int fixedFrames=10);
//! Global BA function for stereo using OpenCV Point2f
/*! solve BA problem with observations, 3D points and camera poses*/
cv::Vec3d solveWindowedBA(const std::vector<std::vector<std::pair<me::ptH2D,me::ptH2D>>>& observations,const std::vector<ptH3D>& pts3D, const cv::Matx33d& K, const cv::Mat& img);
//! optimization function with OpenCV Point2f
/*! implements LM and GN minimization algo. Deprecated */
bool optimize(const std::deque<std::vector<cv::Point2f>>& observations, cv::Mat& state);
//! optimization function with WBA_Points
/*! implements LM and GN minimization algo */
bool optimize(const std::vector<WBA_Ptf>& pts, cv::Mat& state, const int window_size, const int fixedFrames);
//! optimization function for stereo with OpenCV Point2f
/*! implements LM and GN minimization algo. Deprecated */
bool optimize_stereo(const std::vector<std::vector<std::pair<me::ptH2D,me::ptH2D>>>& observations,const std::vector<ptH3D>& pts3D, cv::Mat& state);

//! function computing Projection matrices for each camera from the state vector
std::vector<cv::Matx34d> computeProjectionMatrices(const cv::Mat& Xa);
//! function projecting 3D points into observations
std::deque<std::vector<cv::Point2f>> project_pts(const cv::Mat& Xb, const std::vector<cv::Matx34d>& pMat);

//! computes residual errors from the provided state vector using OpenCV Point2f
/*! Deprecated */
cv::Mat compute_residuals(const std::deque<std::vector<cv::Point2f>>& observations, const cv::Mat& Xb, const cv::Mat& Xa);
//! computes residual errors from the provided state vector using WBA_Points
cv::Mat compute_residuals(const std::vector<WBA_Ptf>& pts, const cv::Mat& Xa, const cv::Mat& Xb);
//! computes residual errors from the provided state vector using OpenCV Point2f
/*! Deprecated */
cv::Mat compute_residuals_stereo(const std::vector<std::vector<std::pair<me::ptH2D,me::ptH2D>>>& observations, const std::vector<ptH3D>& pts3D, const cv::Mat& Xa);

//! computes Jacobian matrix from state vector
void computeJacobian(const cv::Mat& Xa,const cv::Mat& Xb, const cv::Mat& residuals, cv::Mat& JJ, cv::Mat& e);
//! computing Jacobian matrix from pts
void computeJacobian(const std::vector<WBA_Ptf>& pts, const cv::Mat& Xa,  const cv::Mat& Xb, const cv::Mat& residuals, cv::Mat& JJ, cv::Mat& A, cv::Mat& B, cv::Mat& W, cv::Mat& e);
//! computing Jacobian matrix for stereo
/*! Deprecated */
void computeJacobian_stereo(const cv::Mat& Xa,const std::vector<ptH3D>& pts3D, const cv::Mat& residuals, cv::Mat& JJ, cv::Mat& e);

//! Display the different camera poses in a 3D environment
void showCameraPoses(const cv::Mat& Xa);
//! Display the different camera poses and 3D points in a 3D environment
void showCameraPosesAndPoints(const Euld& orientation,const cv::Vec3d& position, const std::vector<ptH3D>& pts);
//! Display the different camera poses and 3D points in a 3D environment
void showCameraPosesAndPoints(const std::vector<Euld>& ori ,const std::vector<cv::Vec3d>& pos, const std::vector<WBA_Ptf>& pts);
//! Display the different camera poses and 3D points in a 3D environment
void showCameraPosesAndPoints(const cv::Mat& P, const std::vector<ptH3D>& pts);
//! Display the different camera poses and 3D points in a 3D environment
void showReprojectedPts(const cv::Mat& img, const std::vector<cv::Matx34d>& pMat, const std::vector<std::vector<cv::Point2f>>& observations, const cv::Mat& Xb);

}

#endif // WINDOWEDBA_H_INCLUDED