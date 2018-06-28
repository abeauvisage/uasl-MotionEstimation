#ifndef DATA_UTILS_H_INCLUDED
#define DATA_UTILS_H_INCLUDED

#include <opencv2/core.hpp>

#include "utils.h"

namespace me{

//! Structure representing inertial data.
/*! Contains acceleration, angular velocity, orientation and timestamp. */
struct ImuData{

    //accelerometer
    cv::Vec3d acc;
    //gyroscope
    cv::Vec3d gyr;
    //position
    cv::Vec3d pos;
    //orientation
    Quat<double> orientation;

    int64_t stamp;

    //! Main constructor. By default all parameters are equal to 0.
    ImuData(int64_t stamp_=0, double a_x=0, double a_y=0, double a_z=0, double g_x=0,double g_y=0, double g_z=0, double p_x=0, double p_y=0, double p_z=0, const Quat<double>& orient=Quat<double>()){acc=cv::Vec3d(a_x,a_y,a_z);gyr=cv::Vec3d(g_x,g_y,g_z);pos=cv::Vec3d(p_x,p_y,p_z),orientation=orient;stamp=stamp_;}
    //! Copy constructor.
    ImuData(const ImuData& data){acc=data.acc;gyr=data.gyr;pos=data.pos;orientation=data.orientation;stamp=data.stamp;}

    //! concatenation operator. Sum up  acceleration, angular velocity and orientation. Timestamp is replaced by the one of the added ImuData.
    void operator+=(const ImuData& imu){
        acc += imu.acc;
        gyr += imu.gyr;
        pos = imu.pos;
        orientation = imu.orientation;

        stamp = imu.stamp;
    }
    //! Sum up  acceleration, angular velocity and orientation. Timestamp is replaced by the one of the added ImuData. Return a new ImuData.
    ImuData operator+(const ImuData& imu) const{
        ImuData res;
        res.acc = acc+imu.acc;
        res.gyr = gyr+imu.gyr;
        res.pos = imu.pos;
        res.orientation = imu.orientation;

        res.stamp = imu.stamp;
        return res;
    }

    //! Division operator. Divide acceleration and angular velocity by a specific value. Used for computing mean of ImuData.
    void operator/=(const int nb){
        acc /= (double)nb;
        gyr /= (double)nb;
    }

};

//! Structure representing a GPS coordinate.
/*! Contains longitude, latitude, altitude and timestamp. */
struct GpsData{

    double lon;
    double lat;
    double alt;

    int status; // deprecated

    int64_t stamp;

    //! Main constructor. By defaut all parameters are equal to 0.
    GpsData(int64_t stamp_=0, double longitude=0, double latitude=0, double altitude=0, int status_=0){lon=longitude;lat=latitude;alt=altitude;stamp=stamp_;status=status_;}
    //! Copy constructor.
    GpsData(const GpsData& data){lon=data.lon;lat=data.lat;alt=data.alt;status=data.status;stamp=data.stamp;}
};

}

#endif // DATA_UTILS_H_INCLUDED
