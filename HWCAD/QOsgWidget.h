#pragma once
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
class QOsgWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	QOsgWidget(QWidget *parent = nullptr);
	~QOsgWidget();

	osg::ref_ptr<osgText::Text> createText(const std::string& str, const osg::Vec3& position, float size);
	osg::ref_ptr<osg::Geode> createAxisGeode(float size);

protected:
	void initializeGL()override;
	void paintGL()override;
	void resizeGL(int w, int h)override;
	//将osgGA::EventQueue事件与Qt鼠标事件相关联
	void mouseDoubleClickEvent(QMouseEvent *event)override;
	void mouseMoveEvent(QMouseEvent *event)override;
	void mousePressEvent(QMouseEvent *event)override;
	void mouseReleaseEvent(QMouseEvent *event)override;
	void wheelEvent(QWheelEvent *event)override;
	void keyPressEvent(QKeyEvent *event)override;
	void keyReleaseEvent(QKeyEvent *event)override;

	bool event(QEvent *event) override;
private:
	osgGA::EventQueue* getEventQueue()const;
	void setKeyboardModifiers(QInputEvent* event);
	osg::ref_ptr<osg::Node> createOSGGlider();

private:
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
	osg::ref_ptr<osgViewer::Viewer> _mViewer;
};
