#include "HWCAD.h"
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>  
#include "QOsgWidget.h"

HWCAD::HWCAD(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(centralWidget);
	QOsgWidget* glWidget = new QOsgWidget();
	layout->addWidget(glWidget);
	setCentralWidget(glWidget);
}

HWCAD::~HWCAD()
{}
