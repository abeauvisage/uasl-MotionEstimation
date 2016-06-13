#ifndef FEATURETYPE_H
#define FEATURETYPE_H

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <cstring>

#define DESCRIPTOR_SIZE 32

using namespace std;

template <typename T>
void printVector(T * data, size_t s, size_t width = 0)
{
	for (size_t i = 0; i < s; i++)
	{
		std::cout << data[i] <<" ";
		if (width && (i+1)%width == 0) std::cout << std::endl;
	}
	std::cout << std::endl;
}

template <typename T>
struct featureType {
	int32_t u;   // u-coordinate
	int32_t v;   // v-coordinate
	T * d;

	featureType() {d = new T();}
	featureType(int32_t u, int32_t v) :u(u), v(v) {}
};

// structure for storing SURF interest points
struct featureFREAK : featureType<unsigned char> {
	//Size : 64

	featureFREAK(const featureFREAK& feat) : featureType(feat.u,feat.v)
    {
        d = new unsigned char[64];
        for(int i=0;i<64;i++)
            std::memcpy( d, feat.d, 64 );
    }

	featureFREAK(int32_t u = 0, int32_t v = 0) : featureType(u, v)
	{
		d = new unsigned char[64];
	}

	featureFREAK& operator=(const featureFREAK& feat)
    {
        if (this != &feat) {
            memcpy( d, feat.d, 64 );
        }
        return *this;

    }

	~featureFREAK()
	{
		if(d) delete[] d;
	}
};

// structure for storing SURF interest points
struct featureSURF : featureType<float> {
	//Size : 128

	featureSURF(const featureSURF& feat) : featureType(feat.u,feat.v)
    {
        d = new float[128];
        for(int i=0;i<128;i++)
            std::memcpy( d, feat.d, 128 );
    }

	featureSURF(int32_t u = 0, int32_t v = 0) : featureType(u, v)
	{
		d = new float[128];
	}

	featureSURF& operator=(const featureSURF& feat)
    {
        if (this != &feat) {
            memcpy( d, feat.d, 128 );
        }
        return *this;

    }

	~featureSURF()
	{
		if (d) delete[] d;
	}
};

struct featurePC : featureType<float> {

    featurePC(const featurePC& feat) : featureType(feat.u,feat.v)
    {
        d = new float[104];
        for(int i=0;i<104;i++)
            std::memcpy( d, feat.d, 104 );
    }

    featurePC(int32_t u=0, int32_t v=0) : featureType(u,v)
    {
        d = new float[104];
        for(int i=0;i<104;i++)
            d[i]=0;
    }

    featurePC& operator=(const featurePC& feat)
    {
        if (this != &feat) {
            memcpy( d, feat.d, 104 );
        }
        return *this;

    }

    ~featurePC()
    {

        if (d) delete d;
    }
};

struct featurePCOnly : featureType<float> {

    featurePCOnly(const featurePCOnly& feat) : featureType(feat.u,feat.v)
    {
        d = new float[24];
        std::memcpy( d, feat.d, 24 );
    }

    featurePCOnly(int32_t u=0, int32_t v=0) : featureType(u,v)
    {
        d = new float[24];
        for(int i=0;i<24;i++)
            d[i]=0;
    }

    featurePCOnly& operator=(const featurePCOnly& feat)
    {
        if (this != &feat) {
            memcpy( d, feat.d, 24 );
        }
        return *this;

    }

    ~featurePCOnly()
    {
        if (d) delete[] d;
    }
};

template <typename T>
struct StereoMatch{
	T f1;
	T f2;
	float m_score;

    StereoMatch(){};
	StereoMatch(const T& feat1, const T& feat2, float score=-1):f1(feat1),f2(feat2),m_score(score){}
	StereoMatch(const StereoMatch& sm):f1(sm.f1),f2(sm.f2),m_score(sm.m_score){}
};

template <typename T>
struct StereoOdoMatches{
    T f1;
    T f2;
    T f3;
    T f4;
    float m_score;
    StereoOdoMatches(){};
	StereoOdoMatches(const T& feat1, const T& feat2, const T& feat3, const T& feat4, float score=-1):f1(feat1),f2(feat2),f3(feat3),f4(feat4), m_score(score){}
	StereoOdoMatches(const StereoOdoMatches& som):f1(som.f1),f2(som.f2),f3(som.f3),f4(som.f4), m_score(som.m_score){}
};

#endif
