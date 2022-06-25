#include <torch/torch.h>

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef BUILD_SP
#include "SPextractor.h"
#endif
#ifdef BUILD_GCN
#include "GCNextractor.h"
#endif

using namespace ORB_SLAM2;

int main() {
  std::cout << "torch::cuda: " << torch::cuda::is_available() << std::endl;

  cv::Mat img, img_gray;
  img = cv::imread("./imgs/tum00.png");
  if (img.empty()) {
    std::cerr << "img empty!!!" << std::endl;
    return -1;
  }
  cv::cvtColor(img, img_gray, CV_BGR2GRAY);

  int nFeatures = 500;
  float fScaleFactor = 1.2;
  int nLevels = 4;

  std::string str_title;
  FTRextractor* mpFTRextractor;
#ifdef BUILD_SP
  str_title = "SuperPoint";
  float fIniThFAST = 0.015;
  float fMinThFAST = 0.007;
  mpFTRextractor = new FTRextractor(nFeatures, fScaleFactor, nLevels, fIniThFAST, fMinThFAST);
#endif
#ifdef BUILD_GCN
  str_title = "GCNv2";
  float fIniThFAST = 20;
  float fMinThFAST = 7;
  mpFTRextractor = new FTRextractor(nFeatures, fScaleFactor, nLevels, fIniThFAST, fMinThFAST, "./GCN2/gcn2_640x480.pt");
#endif

  std::vector<cv::KeyPoint> mvKeys;
  cv::Mat mDescriptors;

  (*mpFTRextractor)(img_gray, cv::Mat(), mvKeys, mDescriptors);

  std::cout << "keypoints size: " << mvKeys.size() << std::endl;
  std::cout << "desc size: " << mDescriptors.rows << ' ' << mDescriptors.cols << std::endl;

  // cv::drawKeypoints(img_gray, mvKeys, img, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  const float r = 5;
  for (int i = 0; i < mvKeys.size(); i++) {
    const cv::Point2f& pt = mvKeys[i].pt;
    cv::Point2f pt1, pt2;
    pt1.x = pt.x - r;
    pt1.y = pt.y - r;
    pt2.x = pt.x + r;
    pt2.y = pt.y + r;
    cv::rectangle(img, pt1, pt2, cv::Scalar(0, 255, 0));
    cv::circle(img, pt, 2, cv::Scalar(0, 255, 0), -1);
  }
  cv::imshow(str_title, img);
  cv::waitKey(0);

  return 0;
}