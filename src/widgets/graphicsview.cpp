#include <QGridLayout>
#include <QWidget>

#include "graphicsview.h"
#include "../../mainwindow.h"

GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView{ parent }
    , m_scaleFactor{ 1.0 }
    , m_dpi{ 96 }
{
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setViewportMargins(RULER_BREADTH, RULER_BREADTH, 0, 0);
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(QMargins(0, 0, 0, 0));

    m_verticalRuler = new QDRuler(QDRuler::RulerType::Vertical, this);
    m_verticalRuler->setDpi( m_dpi );
    m_horizontalRuler  = new QDRuler(QDRuler::RulerType::Horizontal, this);
    m_horizontalRuler->setDpi( m_dpi );

    //m_verticalRuler->setRulerUnit(0.5);
    //m_horizontalRuler->setRulerUnit(7.087);

    QWidget* fake = new QWidget();
    fake->setBackgroundRole(QPalette::Window);
    fake->setFixedSize(RULER_BREADTH, RULER_BREADTH);
    gridLayout->addWidget(fake, 0, 0);
    gridLayout->addWidget(m_horizontalRuler, 0, 1, Qt::AlignLeft | Qt::AlignTop);
    gridLayout->addWidget(m_verticalRuler, 1, 0, Qt::AlignLeft | Qt::AlignTop);
    gridLayout->addWidget(this->viewport(), 1, 1);

    this->setLayout(gridLayout);
}

void GraphicsView::setScene(QGraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
    if (scene) {
        //m_verticalRuler->setFixedHeight(scene->height());
        //m_horizontalRuler->setFixedWidth(scene->width());
    }
}

void GraphicsView::incrScale(bool incr)
{
    if(incr) {
        if(m_scaleFactor<1)
            m_scaleFactor += 0.2;
        else if(m_scaleFactor<14)
            m_scaleFactor += 1.0;
    } else {
        if(m_scaleFactor>1)
            m_scaleFactor -= 1.0;
        else if(m_scaleFactor>0.4)
            m_scaleFactor -= 0.2;
    }
    this->resetTransform();
    //m_verticalRuler->resetScale();
    //m_horizontalRuler->resetScale();

    this->scale(m_scaleFactor, m_scaleFactor);
    m_verticalRuler->setRulerZoom(m_scaleFactor);
    m_horizontalRuler->setRulerZoom(m_scaleFactor);
}

void GraphicsView::resetScale()
{
    m_scaleFactor = 1.0;
    this->resetTransform();
    m_verticalRuler->setRulerZoom(m_scaleFactor);
    m_horizontalRuler->setRulerZoom(m_scaleFactor);
}

int GraphicsView::dpi() const
{
    return m_dpi;
}

qreal GraphicsView::scaleFactor() const
{
    return m_scaleFactor;
}

QDRuler *GraphicsView::verticalRuler() const
{
    return m_verticalRuler;
}

QDRuler *GraphicsView::horizontalRuler() const
{
    return m_horizontalRuler;
}

//void GraphicsView::mouseMoveEvent(QMouseEvent *event)
//{
    //emit onMouseMove(event);
    //qDebug() << event->pos();
    //QWidget *parent = qobject_cast<QWidget*>(this->parent());
    //MainWindow *mainWindow = qobject_cast<MainWindow*>(parent->parent());

    //QGraphicsView::mouseMoveEvent(event);
//}

