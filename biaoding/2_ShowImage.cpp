#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/calib3d/calib3d.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <iostream>  
#include <fstream>  
#include <sstream>
#include <linux/videodev2.h>
using namespace cv;  
using namespace std;  

int main( )
{    
    Size image_size;  /* 图像的尺寸 */ 
    image_size.width =640 ;  
    image_size.height =480;     
    Size board_size = Size(3,4);    /* 标定板上每行、列的角点数 */  
    vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点 */  
    vector<vector<Point2f> > image_points_seq; /* 保存检测到的所有角点 */  
    string filename;  
    int count= -1 ;//用于存储角点个数。

     /* 输出检验*/  
     VideoCapture cap(0);   
     Mat imageInput;
     while(1)
     {
     	bool bSuccess=cap.read(imageInput);  
     	flip(imageInput, imageInput, 1); // flip by x axis  
     	if (!bSuccess) //if not success, break loop  
     	{
            cout << "Cannot read a frame from video stream" << endl;
            break;
     	}
     	
     	/* 提取角点 */  
     	if (0 == findChessboardCorners(imageInput,board_size,image_points_buf))  
     	{             
     	    cout<<"can not find chessboard corners!\n"; //找不到角点  
	    continue;  
     	}
     	else   
     	{  
     	    Mat view_gray;  
     	    cvtColor(imageInput,view_gray,CV_RGB2GRAY);  
     	    ///* 亚像素精确化 */  
     	    find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //对粗提取的角点进行精确化  
     	    //cornerSubPix(view_gray,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));  
     	    image_points_seq.push_back(image_points_buf);  //保存亚像素角点  
     	    ///* 在图像上显示角点位置 */  
     	    drawChessboardCorners(view_gray,board_size,image_points_buf,true); //用于在图片中标记角点  
     	    imshow("Camera Calibration",view_gray);//显示图片  
     
     	}  
	int total = image_points_seq.size();  
        cout<<"total = "<<total<<endl;  
        int CornerNum=board_size.width*board_size.height;  //每张图片上总的角点数  
        
	for (int ii=0 ; ii<CornerNum ;ii++)  
        {  
            if (0 == ii%CornerNum)// 24 是每幅图片的角点个数。此判断语句是为了输出 图片号，便于控制台观看   
            {     
                int i = -1;  
                i = ii/CornerNum;  
                int j=i+1;  
                cout<<"--> di "<<j <<"fu--> : "<<endl;  
            }  
            if (0 == ii%3)  // 此判断语句，格式化输出，便于控制台查看  
            {  
                cout<<endl;  
            }  
            else  
            {  
                cout.width(10);  
            }  
            //输出所有的角点  
            cout<<" -->"<<image_points_seq[0][ii].x;  
            cout<<" -->"<<image_points_seq[0][ii].y;  
        }     
        cout<<"角点提取完成！\n"; 
	image_points_seq.clear(); 
    	waitKey(30);    
    }   
    waitKey(0);
    return 0;  
}  
