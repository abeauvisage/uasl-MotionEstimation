#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ostream>

#include "featureType.h"

#include <opencv2/core.hpp>

#define PI 3.14156592

namespace me{

//! template class Quat
/*! represents a quaternion. Must be a float-point type (float or double), results with other data type are not guaranteed */
template <typename T>
class Quat;

//! template class Euler
/*! represents Euler angles in 3 axis (x,y,z). Must be a float-point type (float or double), results with other data type are not guaranteed */
template <typename T>
class Euler;

typedef Euler<double> Euld; //!< type Euler angles with double precision.
typedef Euler<float> Eulf;
typedef Quat<double> Quatd;
typedef Quat<float> Quatf;

template<typename T>
class Euler {

private:

	T m_roll; //!< angle around the x-axis
	T m_pitch; //!< angle around the y-axis.
	T m_yaw; //!< angle around the z-axis.

    inline void deg2Rad(){m_roll *= PI/180; m_pitch *= PI/180; m_yaw *= PI/180;} //!< converting the angles into rad units.
	inline void rad2Deg(){m_roll *= 180/PI; m_pitch *= 180/PI; m_yaw *= 180/PI;} //!< converting the angles into degrees units.
	inline void computeCosSin(T& cr, T& sr, T& cp, T& sp, T& cy, T& sy) const{cr=cos(m_roll);sr=sin(m_roll);cp=cos(m_pitch);sp=sin(m_pitch);cy=cos(m_yaw);sy=sin(m_yaw);} //!<< computing sine and cosine for each axis.

public:
    /*! Main constructor. Takes 3 angles as input and the unit. By default  all angles are 0 and expressed in radians. */
	Euler(T r=0, T p=0, T y=0, bool rad=true ): m_roll(r), m_pitch(p), m_yaw(y) {if(!rad)deg2Rad();}
	/*! retrieves Euler angles from a rotation matrix and create an Euler object. */
	Euler(const cv::Mat& M){fromMat(M);}
	/*! copy constructor. */
	Euler(const Euler& e): m_roll(e.roll()), m_pitch(e.pitch()), m_yaw(e.yaw()){}

	//displaying
	friend std::ostream& operator<<(std::ostream& os, const Euler<T>& e){
        os << "[" << e.m_roll << "," << e.m_pitch << "," << e.m_yaw << ", rad]";
        return os;
    }
	std::string getDegrees(); /*!< return the degree angles as a string (for display purposes). */

	cv::Matx<T,3,3> getR3() const;      //!< 3x3 R Matrix.
	cv::Matx<T,4,4> getR4() const;      //!< 4x4 Rt Matrix, t is null because only the angle is known.
	cv::Matx<T,3,3> getE() const;       //!< 3x3 E matrix.
	cv::Matx<T,3,3> getdRdr() const;    //!< 3x3 derivative of R along the x axis (roll).
	cv::Matx<T,3,3> getdRdp() const;    //!< 3x3 derivative of R along the y axis (pitch).
	cv::Matx<T,3,3> getdRdy() const;    //!< 3x3 derivative of R along the z axis (yaw).
	void fromMat(const cv::Mat& R);     //!< compute Euler angles from a rotation matrix R.
    Quat<T> getQuat() const;            //!< convert Euler angles to a Quaternion of the same type.

    //operator
    void operator+=(Euler& e); //!< concatenate with another Euler object.
    cv::Vec<T,3> operator*(const cv::Vec<T,3>& v); //!< rotate a 3-vector with the rotation described by the object.
	cv::Vec<T,4> operator*(const cv::Vec<T,4>& v); //!< rotate a 4-vector with the rotation described by the object.

    //access
    T roll() const {return m_roll;}
    T pitch() const {return m_pitch;}
    T yaw() const {return m_yaw;}
};

template <typename T>
class Quat {

private:

	T m_w; //!< real component.
	T m_x; //!< x-axis.
	T m_y; //!< y-axis.
	T m_z; //!< z-axis.

public:

    /*! Main constructor. Default values are 1 for the real part and 0 for the axis components. */
	Quat(T w=1, T x=0, T y=0, T z=0): m_w(w), m_x(x), m_y(y), m_z(z){}
	/*! Create a Quat object from a rotation matrix and normalize it. */
	Quat(const cv::Mat& M){fromMat(M);norm();}
	/*! copy constructor. */
	Quat(const Quat& q): m_w(q.w()), m_x(q.x()), m_y(q.y()), m_z(q.z()){norm();}

	void norm(); //!< normalize the object
	Quat conj() const {return Quat(m_w,-m_x,-m_y,-m_z);} //!< returns the conjugate of the object.

	friend std::ostream& operator<<(std::ostream& os, const Quat<T>& q){
        os << "[" << q.m_w << "|" << q.m_x << "," << q.m_y << "," << q.m_z << "] angle: " << 2*acos(q.m_w);
        return os;
    }

    // conversions
    inline cv::Matx<T,4,4> getQ() const;        //!< Q matrix to multiply with another quaternion.
    inline cv::Matx<T,4,4> getQ_() const;       //!< inverse of Q. Represents the inverse rotation.
    inline cv::Matx<T,4,4> getdQdq0() const;    //!< dQ/dw.
	inline cv::Matx<T,4,4> getdQ_dq0() const;   //!< dQ^-1/dw.
	inline cv::Matx<T,4,4> getdQdq1() const;    //!< dQ/dx.
	inline cv::Matx<T,4,4> getdQ_dq1() const;   //!< dQ^-1/dx.
	inline cv::Matx<T,4,4> getdQdq2() const;    //!< dQ/dy.
	inline cv::Matx<T,4,4> getdQ_dq2() const;   //!< dQ^-1/dy.
	inline cv::Matx<T,4,4> getdQdq3() const;    //!< dQ/dz.
	inline cv::Matx<T,4,4> getdQ_dq3() const;   //!< dQ^-1/dz.

	inline cv::Matx<T,3,3> getR3() const;   //!< returns the 3x3 corresponding rotation matrix.
	inline cv::Matx<T,4,4> getR4() const;   //!< returns the 4x4 corresponding rotation matrix.
	void fromMat(const cv::Mat& M);         //!< update thet object from a rotation matrix.
	Euler<T> getEuler();                    //!< convert the quaternion object into Euler angles.

	//operators
	void operator*=(const Quat& q);
	void operator*=(const double& d){m_w*=d;m_x*=d;m_y*=d;m_z*=d;}
	Quat operator*(const Quat& q) const;
	Quat operator*(const double d) const;
	cv::Vec<T,3> operator*(const cv::Vec<T,3>& v);
	cv::Vec<T,4> operator*(const cv::Vec<T,4>& v);
	Quat operator+(const Quat& q) const;
	void operator+=(const Quat& q);
	void operator-=(const Quat& q);
	void operator/=(double nb);

	//access
	double w() const {return m_w;}
	double x() const {return m_x;}
	double y() const {return m_y;}
	double z() const {return m_z;}
};

std::vector<pt2D> nonMaxSupScanline3x3(const cv::Mat& input, cv::Mat& output);

}

#endif // UTILS_H_INCLUDED
