#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "src/widgets/graphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Ui::MainWindow *ui;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GraphicsView *m_graphicsView;
    QLabel *m_posXLabel;
    QLabel *m_posYLabel;
    QLabel *m_scaleFactor;

protected:
    void paintEvent(QPaintEvent *e);

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_increaseZoomButton_clicked();
    void on_decreaseZoomButton_clicked();
    void on_originalZoomButton_clicked();

//private slots:
//    void onSribbleMouseMove(const QMouseEvent *event);

    void refreshStatusBar();
};
#endif // MAINWINDOW_H
