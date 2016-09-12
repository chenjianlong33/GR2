
#include "asus_jerry_chen_gr2_GR_Result.h"

#include "Eigen/Dense"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#include <android/log.h>

#define LOGE(x...) __android_log_print(ANDROID_LOG_ERROR, "JNI", x)



using namespace Eigen;
//using namespace std;
using namespace cv;

double dTheta2[505];
double dTheta1[10100];
double dMean_PCA[1600];
double dM_PCA[160000];
char data[20];
double  atdata = -1;
char filename[80];
char bitmap1[9];
double dbitmap[1600];
MatrixXd layer1(1, 101);
MatrixXd layer2(1, 101);

MatrixXd theta1(100,101);
MatrixXd theta2(5,101);
MatrixXd M_PCA(1600,100);
MatrixXd mean_PCA(1,1600);
MatrixXd bmp(1,1600);

Mat mat1;
int readBitmap()
{
    Mat mat = imread(filename,0);
    LOGE("xxxxxxxxxxxxxxxxxxxxxxmat %dxxxxxxxxxxxxxxxxxxxxxxxxxxx",mat.cols);
    if (mat.cols > 40)
	{
		resize(mat, mat1, cv::Size(40, 40), 0, 0, CV_INTER_AREA);
	}
	else
	{
		resize(mat, mat1, cv::Size(40, 40), 0, 0, CV_INTER_LINEAR);
	}
}

int readTheta1()
{
	int fd;
    fd = open(filename, O_RDWR  , 0666);
    if (fd == -1)
    {
        return -1;
    }

	for (int j = 0;j < 101;j++)
	{
		read(fd, data,  20);
		dTheta1[j] = atof(data);
	}
	for (int i = 1, k = 101;i < 100;i++)
	{
		read(fd, data,  2);
		for (int j = 0;j < 101;j++, k++)
		{
			read(fd, data,  20);
            dTheta1[k] = atof(data);
		}
	}
	close(fd);
	return 0;
}


int readTheta2()
{
    int fd;
    fd = open(filename, O_RDWR  , 0666);
    if (fd == -1)
    {
        return -1;
    }

    for (int j = 0;j < 101;j++)
    {
        read(fd, data,  20);
        dTheta2[j] = atof(data);
    }
    for (int i = 1, k = 101;i < 5;i++)
    {
        read(fd, data,  2);
        for (int j = 0;j < 101;j++, k++)
        {
            read(fd, data,  20);
            dTheta2[k] = atof(data);
        }
    }
    close(fd);
    return 0;
}


int readThetaMean_PCA()
{
    int fd;
    fd = open(filename, O_RDWR  , 0666);
    if (fd == -1)
    {
        return -1;
    }

	for (int j = 0;j < 1600;j++)
	{
		read(fd, data,  20);
        dMean_PCA[j] = atof(data);
	}

    close(fd);

	return 0;
}

int readThetaM_PCA()
{
	int fd;
    fd = open(filename, O_RDWR  , 0666);
    if (fd == -1)
    {
        return -1;
    }
	for (int j = 0;j < 100;j++)
	{
		read(fd, data,  20);
        dM_PCA[j] = atof(data);
	}
	for (int i = 1, k = 100;i < 1600;i++)
	{
		read(fd, data,  2);
		for (int j = 0;j < 100;j++, k++)
		{
			read(fd, data,  20);
			dM_PCA[k] = atof(data);
		}
	}
    close(fd);
	return 0;
}





int neural_network()
{

    bmp = bmp.array() - mean_PCA.array();
    bmp = bmp*M_PCA;

    int num_labels = 5;


    layer1.setOnes(1,101);
	layer1.block(0,1,1,100) = bmp;
	LOGE("==layer1=%f %f=== ",layer1(0,0), layer1(0,51));

    Matrix<double, 1, 100> h1 = 1/(1+(-1*(layer1 * theta1.transpose()).array()).exp());



  //  Matrix<double, 1, 100> h1 = 1.0 / ((((layer1 * theta1.transpose()).array()*(-1)).array().exp()).array() +1).array();
    LOGE("==h1=%f %f=== ",h1(0,0), h1(0,51));
    //	h1.print();
    layer2.setOnes(1,101);
    layer2.block(0,1,1,100) = h1;
    LOGE("==layer2=%f %f=== ",layer2(0,0), layer2(0,51));
       //arma::mat theta2(Theta2, 2, 26, false);
    Matrix<double, 1, 5> h2 = 1/(1+(-1*(layer2 * theta2.transpose()).array()).exp());
       //	h2.print();
    LOGE("==h2=%f %f %f %f %f=== ",h2(0,0), h2(0,1), h2(0,2), h2(0,3), h2(0,4));
       int col = -1;
       int row = -1;
    int gesture_id = -10;
       double a= h2.maxCoeff(&row, &col);
       if (a > 0.9)
       {
           LOGE("%d %d ",row, col);
           gesture_id = col;
       //    isflush = true;
       }

       return gesture_id;
}



JNIEXPORT jint JNICALL Java_asus_jerry_1chen_gr2_GR_1Result_getResultFromJni
        (JNIEnv *env, jobject obi,jstring filesDir)
{

 //   jint c = 123456;
//   double a[6] = { 1,2,3,4,5,6};
 //   double b[6] = { 1,2,3,4,5,6};
 //   Matrix<double, 1, 6> ma(a);
 //   Matrix<double, 6, 1> mb(b);

    snprintf(filename, sizeof(filename), "%s/RX_Theta1_1600to100.txt", env->GetStringUTFChars(filesDir, false));

    LOGE("%s",filename);

    readTheta1();

    snprintf(filename, sizeof(filename), "%s/RX_Theta2_1600to100.txt", env->GetStringUTFChars(filesDir, false));

    LOGE("%s",filename);

    readTheta2();

    snprintf(filename, sizeof(filename), "%s/RX_PCA_mapping_M_1600to100.txt", env->GetStringUTFChars(filesDir, false));

    LOGE("%s",filename);

    readThetaM_PCA();

    snprintf(filename, sizeof(filename), "%s/RX_PCA_mapping_mean_1600to100.txt", env->GetStringUTFChars(filesDir, false));

    LOGE("%s",filename);

    readThetaMean_PCA();

    snprintf(filename, sizeof(filename), "%s/5.bmp", env->GetStringUTFChars(filesDir, false));

    LOGE("%s",filename);

    readBitmap();

    for(int i=0;i<1600;i++)
    {
        for(int j=0;j<100;j++)
        {
            M_PCA(i,j) = dM_PCA[i*100+j];
        }
    }


    for(int i=0;i<100;i++)
    {
        for(int j=0;j<101;j++)
        {
            theta1(i,j) = dTheta1[i*101+j];
        }
    }
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<101;j++)
        {
            theta2(i,j) = dTheta2[i*101+j];
        }
    }

    for(int j=0;j<1600;j++)
    {
        mean_PCA(0,j) = dMean_PCA[j];
    }

    unsigned char *p;
	p = mat1.ptr<uchar>(0);
    for(int j=0;j<1600;j++)
    {
        bmp(0,j) = p[j];
        if(j==50)
        {
         LOGE("xxxx  %d %d %d %d %d %d xxxxx ",p[j],p[j+1],p[j+2],p[j+3],p[j+4],p[j+5]);
        }
    }


    bmp = (bmp.array()-128).array()/128;
    LOGE("xxxx  %f %f %f %f %f xxxxx ",theta1(3,3), theta2(3, 3), M_PCA(3,4), mean_PCA(0,50),bmp(0,50));
    int result = neural_network();
//	std::cout << ma;


//	std::cout<<ma.maxCoeff();

//	std::cout << ma.array().exp();

//	std::cout << ma.array()*ma.array();//.*

//	std::cout << ma * ma;//*


//	std::cout << 1/(1+(-1*ma.array()).exp());
//    double b[5] = {7,8,9,0,1};
//    Matrix<double, 1, 5> mb(b);
//    ma.block(0,1,1,5) = mb;

 //   c=ma*mb;
 //   c=ma*mb;
 //   c=dTheta2[0]*10000;
 //   double d =  dMean_PCA[1599];
 //   int e = d*1000000;
    //   e = bitmap[0];
  //  LOGE("!! %d %d %d %d %d %d %d %d %d %d",(int)bmp(0,19+40*15),(int)bmp(0,19+40*16),(int)bmp(0,19+40*17),(int)bmp(0,19+40*18),(int)bmp(0,19+40*19),(int)bmp(0,19+40*20),(int)bmp(0,19*40*21),(int)bmp(0,19+40*22),(int)bmp(0,19+40*23),(int)bmp(0,19+40*24));
  //  LOGE("!! %f %f ",dbitmap[0],bmp(0,0));
  //  LOGE("!! %f %f ",dMean_PCA[0],mean_PCA(0,0));
  //  LOGE("!! %d %d %d %d %d %d %d %d %d %d",(int)bitmap[15*40+19],(int)bitmap[16*40+19],(int)bitmap[17*40+19],(int)bitmap[18*40+19],(int)bitmap[19*40+19],(int)bitmap[20*40+19],(int)bitmap[21*40+19],(int)bitmap[22*40+19],(int)bitmap[23*40+19],(int)bitmap[24*40+19]);
    return result;
 //   return 40;
}