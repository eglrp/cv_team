#pragma execution_character_set("utf-8")

#pragma once

#include <QFrame>

#include <QGraphicsView>

#include <QSlider>

#include <QToolButton>

#include <QLabel>

#include <QHBoxLayout>

#include <QStyle>

#include <QWheelEvent>

#include <QGraphicsPixmapItem>

#include <QGraphicsScene>

#include <QScrollBar>

#include <QPixmap>

#include <QString>



#include <opencv2\core\core.hpp>

#include <opencv2\highgui\highgui.hpp>

#include <opencv2\imgproc\imgproc.hpp>


/*
******   ����������ͼ��view�Ľ�ϣ�������ʾ�༭ͼ����棬�������Item��
******   Ҫ���Item��Ҫ�Ȼ�ȡ��Scene,
******   ���������������ͼ�񣬼���ͼ�������Mat���ͣ�Ҳ����ֱ����ͼ��·��
******
******
******
******
*/




class FrameView;



class GView : public QGraphicsView

{

	Q_OBJECT

public:

	GView(FrameView *v) : QGraphicsView(), aview(v) { setMouseTracking(true); }

protected:

#ifndef QT_NO_WHEELEVENT

	void wheelEvent(QWheelEvent *) override;

	void mouseDoubleClickEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event) override;

#endif

private:

	FrameView *aview;

};





class FrameView : public QFrame

{

	Q_OBJECT

public:

	FrameView();

	~FrameView() {}



	QGraphicsView *view() const

	{

		return static_cast<QGraphicsView *>(gview);

	}

	/*





	*/

	void resetView();

	void setTopInfo(QString info);

	void setImage(QString m_fileName);

	void setImage(cv::Mat m_image);



	void adaptToWindow(bool b);



	QGraphicsScene* getScene() { return modelScene; }

	void setScene(QGraphicsScene* scene)

	{

		modelScene = scene;

		this->view()->setScene(modelScene);

	};

	cv::Mat getMat() { return m_cvMat; }

	int getPixmapWidth();

	int getPixmapHeight();



	QImage cvMatToQImage(const cv::Mat &inMat);

	QPixmap cvMatToQPixmap(const cv::Mat &inMat);



signals: void to_imageChanged();



	public slots:

	void zoomIn(int level = 1);

	void zoomOut(int level = 1);



	private slots:

	void setupMatrix();

	void on_adaptToWindow_clicked(bool isAdapt);

	void togglePointerMode();



private:

	GView *gview;

	QSlider *zoomSlider;

	QToolButton *zoomInIcon;

	QToolButton *zoomOutIcon;

	QToolButton *zoomFitIcon;

	QToolButton *selectModeButton;

	QVBoxLayout *zoomSliderLayout;

	QHBoxLayout *leftLayout;

	QLabel *currentInfo;

	QGraphicsScene *modelScene;

	QGraphicsPixmapItem *inputImageItem;



	cv::Mat m_cvMat;
};
