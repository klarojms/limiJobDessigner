#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPainter>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainTabCtrl->setCurrentIndex(0);

    m_posXLabel = new QLabel(this);
    m_posYLabel = new QLabel(this);
    m_scaleFactor = new QLabel(this);
    ui->statusbar->addPermanentWidget(m_posXLabel);
    ui->statusbar->addPermanentWidget(m_posYLabel);
    ui->statusbar->addPermanentWidget(m_scaleFactor, 1);

    m_graphicsView = new GraphicsView(ui->scrollAreaWidgetContents);
    //connect(m_graphicsView, &GraphicsView::onMouseMove, this, &MainWindow::onSribbleMouseMove);

    auto layout = new QVBoxLayout();
    layout->addWidget(m_graphicsView);

    ui->scrollAreaWidgetContents->setLayout(layout);

    QGraphicsScene *scn = new QGraphicsScene();

    //QPixmap pixmap("E:\\dev\\qt\\limitronic\\limiJobDessigner\\moma2023.png");
    QPixmap pixmap("E:\\dev\\qt\\limitronic\\limiJobDessigner\\pipi.jpg");
    scn->addPixmap(pixmap);
    m_graphicsView->setScene(scn);
    m_graphicsView->show();

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    //m_graphicsView->calcSize();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj==m_graphicsView->viewport() && event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        int pos = mouseEvent->pos().x() + qAbs(m_graphicsView->horizontalRuler()->origin());
        QString msg = "X: "+QString::number(pos);
        qreal mm = (25.4 / m_graphicsView->dpi()) * pos  / m_graphicsView->scaleFactor();
        msg.append(" (");
        msg += QString::number(mm, 'f', 2);
        msg.append(" mm)");
        m_posXLabel->setText(msg);

        pos = mouseEvent->pos().y() + qAbs(m_graphicsView->verticalRuler()->origin());
        msg = "Y: "+QString::number(pos);
        mm = (25.4 / m_graphicsView->dpi()) * pos / m_graphicsView->scaleFactor();
        msg.append(" (");
        msg += QString::number(mm, 'f', 2);
        msg.append(" mm)");
        m_posYLabel->setText(msg);

        msg = "Scale: " + QString::number(std::round(m_graphicsView->scaleFactor() * 100), 'f', 0)+"%";
        m_scaleFactor->setText(msg);
    }
    return false;
}

void MainWindow::refreshStatusBar()
{
    QMouseEvent * mEvt = new QMouseEvent(QEvent::MouseMove, QPointF(.0, .0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::postEvent(m_graphicsView->viewport(), mEvt);
}

void MainWindow::on_increaseZoomButton_clicked()
{
    m_graphicsView->incrScale(true);
    refreshStatusBar();
}

void MainWindow::on_decreaseZoomButton_clicked()
{
    m_graphicsView->incrScale(false);
    refreshStatusBar();
}

void MainWindow::on_originalZoomButton_clicked()
{
    m_graphicsView->resetScale();
    refreshStatusBar();
}

