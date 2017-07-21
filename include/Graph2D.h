#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <vector>
#include <string>
#include <sstream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace me{
//! Class for creating 1D or 2D graphs

/*! create and diplay 1D or 2D graph. A raster image is created so it doesn't behave very well with zoom but it convenient
    to plot values during processing. */
class Graph2D
{



    public:
	//! Type of lines (with dots for values or not)
        enum Type{DOT,LINE,DOTTEDLINE};

	/*! Default constructor. Takes as input:
            - name of the window created
            - nb of curves
            - boolean to display an orthogonal graph or not
            - size of the window
         */
        Graph2D(std::string name, const int nb=1, bool orth=true, Type t=DOTTEDLINE, cv::Size s=cv::Size(640,480));
        void refresh();
	/*!< add a float point to the ith curve. This Function is called for 2D graphs. */
        void addValue(cv::Point2f& v, int i=1);
	/*!< add a double point to the ith curve. This Function is called for 2D graph. */
        void addValue(cv::Point2d& v, int i=1){cv::Point2f pf(v.x,v.y);addValue(pf,i);}
	/*!< add a double value to the ith curve. This Function is called for 1D graph. */
        void addValue(double v, int i=1){if(m_values[i-1].empty()){cv::Point2f pt(0,0);addValue(pt,i);}cv::Point2f p(m_values[i-1].size(),v);addValue(p,i);}
        void addLegend(std::string s, int i=1){m_legend[i-1]=s;} //!< store the legend for the ith curve.
        void clearGraph(); //!< remove values stored and refresh the graph.
        void saveGraph(std::string filename){imwrite(filename,m_image);} //!< save the image created in a file.
        float getLength(int i=1){return m_length[i-1];} //!< get the length of the ith curve (valid only for 2D graph).
        float getNbValues(int i){return m_values[i-1].size();} //!< Returns the number of values of the ith curve.
        float getNbCurves(){return m_nb_curves;} //!< Returns the total number of curves.

        ~Graph2D();

    private:

    std::string m_name; //!< window name.
    int m_nb_curves;
    cv::Mat m_image; //!< Images where the graph is displayed.
    std::vector<std::vector<cv::Point2f>> m_values; //!< vector storing the different values.
    std::vector<cv::Scalar> m_colours; //!< vector storing the color of each curve.
    std::vector<std::string> m_legend; //!< vector storing the legend of each curve.
    std::vector<float> m_length; //!< length of the first curve (if 2D graph).

    bool m_orthogonal; //!< display or not an orthogonal graph.
    int width;
    int height;

    int m_margin = 60;
    float m_max_pts = 50; //!< maximum number of point displayed per curve (slow down the program if too many).
    int count=0; //!< store the number of values if 1D graph.
    float m_min_x;
    float m_min_y;
    float m_max_x;
    float m_max_y;

    Type m_type;


    //! create a random Scalar(color) when a new curve is created.
    static cv::Scalar randomColor(cv::RNG& rng){int icolor=(unsigned)rng;return cv::Scalar(icolor&255,(icolor>>8)&255,(icolor>>16)&255);}
    void plot_background(); //!< create the background (axis and legend).
    void plot_legend(); //!< create the legend.
    void plot_axis(cv::Mat& bg, const int dx, const int dy); //!< create the axis.
};

}

#endif // GRAPH2D_H
