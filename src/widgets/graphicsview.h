#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>

//#include "ruler.h"
#include "qdruler.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* parent=nullptr);

    void setScene(QGraphicsScene* scene);

    void incrScale(bool increment);

    void resetScale();

    int dpi() const;

    qreal scaleFactor() const;

    QDRuler* verticalRuler() const;
    QDRuler* horizontalRuler() const;

//protected:
//    void mouseMoveEvent(QMouseEvent *event);

//signals:
//    void onMouseMove(const QMouseEvent *event);

private:
    //Ruler* m_verticalRuler;
    //Ruler* m_horizontalRuler;

    QDRuler* m_verticalRuler;
    QDRuler* m_horizontalRuler;

    qreal m_scaleFactor;
    int m_dpi;
};

#endif // GRAPHICSVIEW_H
