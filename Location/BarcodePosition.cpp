#include "BarcodePosition.h"

BarcodePosition::BarcodePosition()
{

}

BarcodePosition::~BarcodePosition()
{
}

vector<Point> BarcodePosition::FindBarcode(Mat src)
{
	Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	//��ͼ�����ƽ������  
	blur(src_gray, src_gray, Size(3, 3));
	//ʹ�Ҷ�ͼ��ֱ��ͼ���⻯  
	equalizeHist(src_gray, src_gray);
	//��ֵ��
	Mat threshold_output;
	threshold(src_gray, threshold_output, 112, 255, THRESH_BINARY); //ָ��112��ֵ���ж�ֵ��

	Scalar color = Scalar(1, 1, 255);
	vector<vector<Point>> contours, contours2;
	vector<Vec4i> hierarchy;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	//Mat drawing2 = Mat::zeros( src.size(), CV_8UC3 );  
	Mat drawingAllContours = Mat::zeros(src.size(), CV_8UC3);

	//���ö�ֵ�����Ѱ������
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	//Ѱ�������ķ���
	int tempindex1 = 0;
	int tempindex2 = 0;

	for (int i = 0;i<contours.size();i++)
	{
		if (hierarchy[i][2] == -1)
			continue;
		else
			tempindex1 = hierarchy[i][2];                //��һ��������������

		if (hierarchy[tempindex1][2] == -1)
			continue;
		else
		{
			tempindex2 = hierarchy[tempindex1][2];        //�ڶ���������������
														  //��¼���������������������������������ǵı�Ŵ洢
			in.a1 = i;
			in.a2 = tempindex1;
			in.a3 = tempindex2;
			vin.push_back(in);
		}
	}
	//�������������
	vector<index>::iterator it;
	for (it = vin.begin();it != vin.end();)
	{
		vector<Point> out1Contours = contours[it->a1];
		vector<Point> out2Contours = contours[it->a2];
		double lenth1 = arcLength(out1Contours, 1);
		double lenth2 = arcLength(out2Contours, 1);
		if (abs(lenth1 / lenth2 - 2)>1)
		{
			it = vin.erase(it);
		}
		else
		{
			drawContours(drawing, contours, it->a1, Scalar(255, 255, 255), CV_FILLED, 8); //rng.uniform(0,255)
			it++;
		}
	}
	//��ȡ������λ�ǵ���������  
	Point point[3];
	int i = 0;
	vector<Point> pointThree;
	for (it = vin.begin(), i = 0;it != vin.end();i++, it++)
	{
		point[i] = Center_cal(contours, it->a1);
		pointThree.push_back(point[i]);
	}

	if (pointThree.size() <3)
	{
		cout << "�ҵ��Ķ�λ�ǵ㲻��3��" << endl;
		return vector<Point>();
	}
	//������������������㶨λ�ǵ�������Ӷ�������߳�
	double area = contourArea(contours[vin[0].a1]);
	int area_side = cvRound(sqrt(double(area)));
	for (int i = 0; i<3; i++)
	{
		//����������λ�ǵ���������  
		line(drawing, point[i % 3], point[(i + 1) % 3], color, area_side / 10, 8);
	}
	vin.clear();
	return pointThree;
}

Point BarcodePosition::Center_cal(vector<vector<Point>> contours, int i)
{
	int centerx = 0, centery = 0;
	size_t n = contours[i].size();
	//����ȡ��С�����εı߽���ÿ���ܳ���������ȡһ��������꣬  
	//������ȡ�ĸ����ƽ�����꣨��ΪС�����εĴ������ģ�  
	centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
	centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
	Point point1 = Point(centerx, centery);
	return point1;
}
