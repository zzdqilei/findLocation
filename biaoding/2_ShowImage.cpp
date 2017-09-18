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
    Size image_size;  /* ͼ��ĳߴ� */ 
    image_size.width =640 ;  
    image_size.height =480;     
    Size board_size = Size(3,4);    /* �궨����ÿ�С��еĽǵ��� */  
    vector<Point2f> image_points_buf;  /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */  
    vector<vector<Point2f> > image_points_seq; /* �����⵽�����нǵ� */  
    string filename;  
    int count= -1 ;//���ڴ洢�ǵ������

     /* �������*/  
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
     	
     	/* ��ȡ�ǵ� */  
     	if (0 == findChessboardCorners(imageInput,board_size,image_points_buf))  
     	{             
     	    cout<<"can not find chessboard corners!\n"; //�Ҳ����ǵ�  
	    continue;  
     	}
     	else   
     	{  
     	    Mat view_gray;  
     	    cvtColor(imageInput,view_gray,CV_RGB2GRAY);  
     	    ///* �����ؾ�ȷ�� */  
     	    find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //�Դ���ȡ�Ľǵ���о�ȷ��  
     	    //cornerSubPix(view_gray,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));  
     	    image_points_seq.push_back(image_points_buf);  //���������ؽǵ�  
     	    ///* ��ͼ������ʾ�ǵ�λ�� */  
     	    drawChessboardCorners(view_gray,board_size,image_points_buf,true); //������ͼƬ�б�ǽǵ�  
     	    imshow("Camera Calibration",view_gray);//��ʾͼƬ  
     
     	}  
	int total = image_points_seq.size();  
        cout<<"total = "<<total<<endl;  
        int CornerNum=board_size.width*board_size.height;  //ÿ��ͼƬ���ܵĽǵ���  
        
	for (int ii=0 ; ii<CornerNum ;ii++)  
        {  
            if (0 == ii%CornerNum)// 24 ��ÿ��ͼƬ�Ľǵ���������ж������Ϊ����� ͼƬ�ţ����ڿ���̨�ۿ�   
            {     
                int i = -1;  
                i = ii/CornerNum;  
                int j=i+1;  
                cout<<"--> di "<<j <<"fu--> : "<<endl;  
            }  
            if (0 == ii%3)  // ���ж���䣬��ʽ����������ڿ���̨�鿴  
            {  
                cout<<endl;  
            }  
            else  
            {  
                cout.width(10);  
            }  
            //������еĽǵ�  
            cout<<" -->"<<image_points_seq[0][ii].x;  
            cout<<" -->"<<image_points_seq[0][ii].y;  
        }     
        cout<<"�ǵ���ȡ��ɣ�\n"; 
	image_points_seq.clear(); 
    	waitKey(30);    
    }   
    waitKey(0);
    return 0;  
}  
