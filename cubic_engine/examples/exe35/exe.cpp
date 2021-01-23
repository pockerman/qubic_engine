#include "cubic_engine/base/config.h"

#ifdef USE_OPEN_CV

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/geometry/geom_point.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc.hpp>
//#include <opencv2/core/utility.hpp>

#include<iostream>
#include<vector>
#include<array>
#include<cmath>


namespace example
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using kernel::GeomPoint;

cv::Point2i
cv_offset(float x, float y, int image_width=2000, int image_height=2000){
  cv::Point2i output;
  output.x = static_cast<int>(x * 100) + image_width/2;
  output.y = image_height - static_cast<int>(y * 100) - image_height/3;
  return output;
}

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

    GeomPoint<2> goal={10.0,10.0};
    
    try{
          #define PI 3.141592653
          bool terminal = false;
          std::vector<real_t> x={0.0, 0.0, PI/8.0, 0.0, 0.0};

          // OpenCV window
          cv::namedWindow("dwa", cv::WINDOW_NORMAL);
          int count = 0;

          for(int i=0; i<1000 && !terminal; i++){


            // visualization
            cv::Mat bg(3500,3500, CV_8UC3, cv::Scalar(255,255,255));

            cv::circle(bg, cv_offset(goal[0], goal[1], bg.cols, bg.rows), 30, cv::Scalar(255,0,0), 5);

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
            }


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
#else
#include <iostream>
#include <stdexcept>
int main() {

   std::cerr<<"This example requires OpenCV. Reconfigure with OpenCV support"<<std::endl;
   throw std::runtime_error("No OpenCV support");
   return 0;
}
#endif

