#include "QOsgWidget.h"

QOsgWidget::QOsgWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	_mViewer(new osgViewer::Viewer),
	_mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded(this->x(), this->y(), this->width(), this->height()))
{
	setFocusPolicy(Qt::StrongFocus);
	this->setMinimumSize(100, 100);
}

QOsgWidget::~QOsgWidget()
{

}

void QOsgWidget::initializeGL() 
{
	osg::Camera* camera = _mViewer->getCamera();
	camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.1f));
	
	camera->setGraphicsContext(_mGraphicsWindow.get());

	osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator();
	manipulator->setAllowThrow(false);
	_mViewer->setCameraManipulator(manipulator.get());
	_mViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	_mViewer->addEventHandler((new osgGA::StateSetManipulator(camera->getOrCreateStateSet())));
	//_mViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	_mViewer->addEventHandler(new osgViewer::StatsHandler);
	osg::ref_ptr<osg::Group> root = new osg::Group();

	QByteArray barr = QString("root").toLocal8Bit();
	char* bdata = barr.data();
	root->setName(bdata);
	osg::ref_ptr<osg::Node> refNode = createOSGGlider();
	osg::Node* nodeptr = refNode.get();
	osg::BoundingSphere bounSphere = nodeptr->getBound();
	double dRad = bounSphere.radius();
	
	//root->addChild(createAxisGeode(dRad));
	root->addChild(nodeptr);

	camera->setProjectionMatrixAsOrtho(-dRad, dRad, -dRad, dRad, dRad, 3 * dRad);
	camera->setViewport(0, 0, this->width(), this->height());

	//优化场景数据
	/*osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());*/
	_mViewer->setSceneData(root);
	_mViewer->realize();
}

void QOsgWidget::resizeGL(int w, int h)
{
	this->getEventQueue()->windowResize(this->x(), this->y(), w, h);
	_mGraphicsWindow->resized(this->x(), this->y(), w, h);
	osg::Camera* camera = _mViewer->getCamera();
	double x = this->x();
	double y = this->y();
	camera->setViewport(x, y, w, h);
}

void QOsgWidget::paintGL()
{
	_mViewer->frame();
}

osgGA::EventQueue* QOsgWidget::getEventQueue()const 
{
	return _mGraphicsWindow->getEventQueue();
};

void QOsgWidget::mouseDoubleClickEvent(QMouseEvent *event) 
{
	setKeyboardModifiers(event);
	int button = 0;
	switch (event->button())
	{
	case Qt::LeftButton:
	button = 1;
	break;
	case Qt::MidButton:
		button = 2;
		break;
	case Qt::RightButton:
		button = 3;
		break;
	}
	getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
	QOpenGLWidget::mouseDoubleClickEvent(event);
}

void QOsgWidget::mouseMoveEvent(QMouseEvent *event) 
{
	setKeyboardModifiers(event);
	getEventQueue()->mouseMotion(event->x(), event->y());
	QOpenGLWidget::mouseMoveEvent(event);
}

void QOsgWidget::mousePressEvent(QMouseEvent *event) 
{
	setKeyboardModifiers(event);
	int button = 0;
	switch (event->button())
	{
	case Qt::LeftButton:
		button = 1;
		break;
	case Qt::MidButton:
		button = 2;
		break;
	case Qt::RightButton:
		button = 3;
		break;
	}
	getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
	QOpenGLWidget::mousePressEvent(event);
}

void QOsgWidget::mouseReleaseEvent(QMouseEvent *event) {
	setKeyboardModifiers(event);
	int button = 0;
	switch (event->button())
	{
	case Qt::LeftButton:
		button = 1;
		break;
	case Qt::MidButton:
		button = 2;
		break;
	case Qt::RightButton:
		button = 3;
		break;
	}
	getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
	QOpenGLWidget::mouseReleaseEvent(event);
}

void QOsgWidget::wheelEvent(QWheelEvent *event) 
{
	setKeyboardModifiers(event);
	getEventQueue()->mouseScroll(event->orientation() == Qt::Vertical ?
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
	QOpenGLWidget::wheelEvent(event);
}

bool QOsgWidget::event(QEvent *event)
{
	bool handled = QOpenGLWidget::event(event);
	switch (event->type())
	{
	case QEvent::MouseButtonPress:
	case QEvent::MouseButtonRelease:
	case QEvent::MouseMove:
	case QEvent::Wheel:
	case QEvent::MouseButtonDblClick:
	case QEvent::KeyPress:
	case QEvent::KeyRelease:
		this->update();
		break;
	default:
		break;
	}
	return handled;
}

void QOsgWidget::setKeyboardModifiers(QInputEvent* event) 
{
	int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier);
	unsigned mask = 0;
	if (modkey & Qt::ShiftModifier) {
		mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
	}
	if (modkey & Qt::AltModifier) {
		mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
	}
	if (modkey & Qt::ControlModifier) {
		mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
	}
	getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void QOsgWidget::keyPressEvent(QKeyEvent *event) 
{
	setKeyboardModifiers(event);
	//QByteArray KeyData = event->text().toLocal8Bit();
	/*const char* cKey = KeyData.data();
	osgGA::GUIEventAdapter::KeySymbol symbol = osgGA::GUIEventAdapter::KeySymbol(event->key());*/
	//getEventQueue()->keyPress();
	switch (event->key())
	{
	case Qt::Key_F:
	{
		//从观察者获取节点的方式
		osg::Geode* geode = dynamic_cast<osg::Geode*>(_mViewer->getSceneData());
		int count = geode->getNumDrawables();
		for (int i = 0; i < count; ++i) {
			osg::ShapeDrawable* draw = dynamic_cast<osg::ShapeDrawable*> (geode->getDrawable(i));
			std::string name = draw->getName();
			if (name == "zidingyi1") {
				draw->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));
			}
		}
		getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_F);
	}
	break;
	case Qt::Key_D:
	{
		//从观察者获取节点的方式
		osg::Geode* geode = dynamic_cast<osg::Geode*>(_mViewer->getSceneData());
		int count = geode->getNumDrawables();
		for (int i = 0; i < count; ++i) 
		{
		osg::Drawable* draw = geode->getDrawable(i);
			std::string name = draw->getName();
			if (name == "zidingyi1") 
			{
				geode->removeDrawable(draw);
			}
		}
	}
	break;
	case Qt::Key_S:
	{
		getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_S);
	}
	break;
	case Qt::Key_L:
	{
		getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_L);
	}
	break;
	case Qt::Key_W:
	{
		getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_W);
	}
	break;
	default:
		break;
	}
	QOpenGLWidget::keyPressEvent(event);
}

void QOsgWidget::keyReleaseEvent(QKeyEvent *event)
{
	setKeyboardModifiers(event);

	QOpenGLWidget::keyReleaseEvent(event);
}

osg::ref_ptr<osg::Node> QOsgWidget::createOSGGlider() 
{
	osg::ref_ptr<osg::Node> node = osgDB::readRefNodeFile("D:/OSG/3Dmodels/tank.osg");
	QByteArray barr = QString("飞机").toLocal8Bit();
	char* bdata = barr.data();
	node->setName(bdata);
	return node.get();
}

osg::ref_ptr<osgText::Text> QOsgWidget::createText(const std::string& str, const osg::Vec3& position, float size) 
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont("fonts/arial.ttf");  // 设置字体路径
	text->setCharacterSize(size);      // 设置字符大小
	text->setPosition(position);       // 设置位置
	text->setText(str);                // 设置文本内容
	text->setAlignment(osgText::Text::LEFT_BOTTOM); // 设置对齐方式
	return text.release();
}

osg::ref_ptr<osg::Geode> QOsgWidget::createAxisGeode(float size)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(createText("X", osg::Vec3(0.0f, size, 0.0f), 10.0f));
	geode->addDrawable(createText("Y", osg::Vec3(size, 0.0f, 0.0f), 10.0f));
	geode->addDrawable(createText("Z", osg::Vec3(0.0f, 0.0f, size), 10.0f));
	return geode.release();
}