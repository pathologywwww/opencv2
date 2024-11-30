# include <iostream>
# include <opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<time.h>
#include<string.h>
using namespace std;
using namespace cv;



//主函数
int main()
{
    
    Mat kernel =getStructuringElement(MORPH_RECT,Size(5,5),Point(-1,-1));
    VideoCapture video;
    video.open("../video.mp4");
    if(!video.isOpened()){
        return -1;
    }
    Mat frame,imgHsv,mask;
    Rect Roi;
    Scalar scalarL =Scalar(78,43,46);
    Scalar scalarH=Scalar(99,255,255);
    while(video.read(frame)){
        resize(frame,frame,Size(1000,500));
        cvtColor(frame,imgHsv,COLOR_BGR2HSV);
        inRange(imgHsv,scalarL,scalarH,mask);
        morphologyEx(mask,mask,MORPH_OPEN,kernel);
        dilate(mask,mask,kernel);
        vector<vector<Point>>contours;
        findContours(mask,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
        if(contours.size()>0){
        double max=0.0;
        for(size_t i=0;i<contours.size();i++){
            double area =contourArea(contours[static_cast<int>(i)]);
            if(area>max){
                Roi=boundingRect(contours[static_cast<int>(i)]);
                for(size_t i=0;i<contours.size();i++){
                Point2f P[4];
                RotatedRect rect =minAreaRect(contours[static_cast<int>(i)]);
                rect.points(P);
                float dur;
                clock_t start,end;
                start=clock();
                rectangle(frame,Roi,Scalar(0,0,255));
                end=clock();
                dur=(float)(end-start);
                string n=to_string(dur);
                rectangle(frame,Rect(),Scalar(0,0,255),-1,LINE_8);
                putText(frame,n,P[2],FONT_HERSHEY_SIMPLEX,1,Scalar(0,120,120),1,1);

        }
            }
        }
    }
        else{
            Roi.x=Roi.y=Roi.width=Roi.height=0;
    }
        imshow("hsv",mask);
        imshow("ori",frame);
        waitKey(50);
    }
    
    return 0;

}
