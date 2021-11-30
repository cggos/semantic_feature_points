#include <iostream>

#include "SPextractor.h"

using namespace ORB_SLAM2;

int main() {

    int nFeatures = 100;
    float fScaleFactor = 1.2;
    int nLevels = 1;
    float fIniThFAST = 1;
    float fMinThFAST = 1;

    ORBextractor *mpORBextractor = new ORBextractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);

    std::vector<cv::KeyPoint> mvKeys;
    cv::Mat mDescriptors;

    cv::Mat img  = cv::imread("/dev_sdb/datasets/kitti/2011_10_03/2011_10_03_drive_0027_sync/image_00/data/0000000010.png", 0);

    // cv::imshow("raw image", img);
    // cv::waitKey(0);

    (*mpORBextractor)(img, cv::Mat(), mvKeys, mDescriptors);

    std::cout << "keypoints size: " << mvKeys.size() << std::endl;
    std::cout << "desc size: " << mDescriptors.rows << ' ' << mDescriptors.cols << std::endl;

    cv::Mat image_show;
    cv::drawKeypoints(img, mvKeys, image_show, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow("features", image_show);
    cv::waitKey(0);

    return 0;
}