#ifndef QDRULER_H
#define QDRULER_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

#define RULER_BREADTH 20

class QDRuler : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
    Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)
public:
    enum class RulerType { Horizontal, Vertical };

    QDRuler(QDRuler::RulerType rulerType, QWidget* parent = nullptr);
    Q_ENUM(RulerType)

    QSize minimumSizeHint() const;
    QDRuler::RulerType rulerType() const;
    qreal origin() const;
    qreal rulerUnit() const;
    qreal rulerZoom() const;
    int dpi() const;

    qreal toMM();

public slots:
    void setOrigin(const qreal origin);
    void setRulerUnit(const qreal rulerUnit);
    void setRulerZoom(const qreal rulerZoom);
    void setCursorPos(const QPoint cursorPos);
    void setMouseTrack(const bool track);
    void setDpi(const int dpi);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);
    void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition);
    //void drawMousePosTick(QPainter* painter);

    void calcAdvance();
private:
    RulerType m_RulerType;
    qreal m_Origin;
    qreal m_RulerUnit;
    qreal m_RulerZoom;
    QPoint m_CursorPos;
    bool m_MouseTracking;
    int m_dpi;
    qreal m_advance;
};

#endif // QDRULER_H
