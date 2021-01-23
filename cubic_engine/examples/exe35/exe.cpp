#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/serial_kmeans.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"



#include<iostream>
#include<vector>
#include<array>
#include<cmath>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;

}

int main() {
   
    using namespace cv;
    using namespace example;
    using Obstacle = std::vector<std::array<float, 2>>;

    Obstacle ob({
        {{-1, -1}},
        {{0, 2}},
        {{4.0, 2.0}},
        {{5.0, 4.0}},
        {{5.0, 5.0}},
        {{5.0, 6.0}},
        {{5.0, 9.0}},
        {{8.0, 9.0}},
        {{7.0, 9.0}},
        {{12.0, 12.0}}
      });
    
    try{
          #define PI 3.141592653
          bool terminal = false;
          std::vector<real_t> x={0.0, 0.0, PI/8.0, 0.0, 0.0};
          std::vector<real_t> goal = {10.0,10.0};

          cv::namedWindow("dwa", cv::WINDOW_NORMAL);
          int count = 0;

          for(int i=0; i<1000 && !terminal; i++){


            // visualization
            cv::Mat bg(3500,3500, CV_8UC3, cv::Scalar(255,255,255));

            /*cv::circle(bg, cv_offset(goal[0], goal[1], bg.cols, bg.rows), 30, cv::Scalar(255,0,0), 5);

            for(unsigned int j=0; j<ob.size(); j++){
              cv::circle(bg, cv_offset(ob[j][0], ob[j][1], bg.cols, bg.rows), 20, cv::Scalar(0,0,0), -1);
            }

            for(unsigned int j=0; j<1; j++){
              cv::circle(bg, cv_offset(x[0], x[1], bg.cols, bg.rows), 7, cv::Scalar(0,255,0), -1);
            }

            cv::circle(bg, cv_offset(x[0], x[1], bg.cols, bg.rows), 30, cv::Scalar(0,0,255), 5);


            cv::arrowedLine(bg, cv_offset(x[0], x[1], bg.cols, bg.rows),
                            cv_offset(x[0] + std::cos(x[2]), x[1] + std::sin(x[2]), bg.cols, bg.rows),
                            cv::Scalar(255,0,255), 7);

            if (std::sqrt(std::pow((x[0] - goal[0]), 2) + std::pow((x[1] - goal[1]), 2)) <= 1.0e-4){
                  terminal = true;
                  for(unsigned int j=0; j<1; j++){
                    cv::circle(bg, cv_offset(x[0], x[1], bg.cols, bg.rows), 7, cv::Scalar(0,0,255), -1);
                  }
            }*/


            cv::imshow("dwa", bg);
            cv::waitKey(5);

            // std::string int_count = std::to_string(count);
            // cv::imwrite("./pngs/"+std::string(5-int_count.length(), '0').append(int_count)+".png", bg);

            count++;
          }

    }
    catch(cv::Exception& e){
        std::cerr<<"OpenCV error: "
                 <<e.what()<<std::endl;
    }
    catch(std::runtime_error& e){
        std::cerr<<"Runtime error: "
                 <<e.what()<<std::endl;
    }
    catch(std::logic_error& e){
        std::cerr<<"Logic error: "
                 <<e.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception was raised whilst running example."<<std::endl;
    }
   
    return 0;
}

