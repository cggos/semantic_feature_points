#include <torch/torch.h>

#include <iostream>

#include "GCNextractor.h"
#include "SPextractor.h"

using namespace ORB_SLAM2;

int main() {
  if (!torch::cuda::is_available()) {
    std::cerr << "torch::cuda is NOT available!" << std::endl;
    return -1;
  }

  cv::Mat img, img_gray;
  img = cv::imread("../imgs/tum00.png");
  if (img.empty()) {
    std::cerr << "img empty!!!" << std::endl;
    return -1;
  }
  cv::cvtColor(img, img_gray, CV_BGR2GRAY);

  int nFeatures = 500;
  float fScaleFactor = 1.2;
  int nLevels = 4;
  float fIniThFAST = 20;  // 20
  float fMinThFAST = 7;   // 7

  // ORBextractor *mpFTRextractor = new ORBextractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);
  GCNextractor* mpFTRextractor = new GCNextractor(nFeatures, fScaleFactor, nLevels, fIniThFAST, fMinThFAST);

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
  cv::imshow("features", img);
  cv::waitKey(0);

  return 0;
}