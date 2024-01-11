#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QDebug>
#include <QGraphicsView>
#include <QScrollBar>
#include "qdruler.h"

QDRuler::QDRuler(RulerType rulerType, QWidget *parent)
    : QWidget(parent)
    , m_RulerType(rulerType)
    , m_Origin(0.)
    , m_RulerUnit(1.)
    , m_RulerZoom(1.)
    , m_MouseTracking(false)
    , m_dpi{ 96 }
{
    setMouseTracking(true);
    //QFont txtFont("Goudy Old Style", 5, 20);
    QFont txtFont("Arial", 7);
    txtFont.setStyleHint(QFont::TypeWriter, QFont::PreferOutline);
    setFont(txtFont);

    if(parent) {
        if(m_RulerType == RulerType::Vertical) {
            connect(qobject_cast<QGraphicsView*>(parent)->verticalScrollBar(), &QScrollBar::valueChanged, this, &QDRuler::setOrigin);
        } else {
            connect(qobject_cast<QGraphicsView*>(parent)->horizontalScrollBar(), &QScrollBar::valueChanged, this, &QDRuler::setOrigin);
        }
    }
}

QSize QDRuler::minimumSizeHint() const
{
    return QSize(RULER_BREADTH,RULER_BREADTH);
}

QDRuler::RulerType QDRuler::rulerType() const
{
    return m_RulerType;
}

qreal QDRuler::origin() const
{
    return m_Origin;
}

qreal QDRuler::rulerUnit() const
{
    return m_RulerUnit;
}

qreal QDRuler::rulerZoom() const
{
    return m_RulerZoom;
}

int QDRuler::dpi() const
{
    return m_dpi;
}

void QDRuler::setOrigin(const qreal origin)
{
    if (m_Origin != origin) {
        //qDebug() << m_Origin << origin;
        //m_Origin = -origin;
        m_Origin = -origin;
        update();
    }
}

void QDRuler::setRulerUnit(const qreal rulerUnit)
{
    if (m_RulerUnit != rulerUnit) {
        m_RulerUnit = rulerUnit;
        update();
    }
}

void QDRuler::setRulerZoom(const qreal rulerZoom)
{
    if (m_RulerZoom != rulerZoom) {
        m_RulerZoom = rulerZoom;
        update();
    }
}

void QDRuler::setCursorPos(const QPoint cursorPos)
{
    m_CursorPos = this->mapFromGlobal(cursorPos);
    m_CursorPos += QPoint(RULER_BREADTH,RULER_BREADTH);
    update();
}

void QDRuler::setMouseTrack(const bool track)
{
    if (m_MouseTracking != track) {
        m_MouseTracking = track;
        update();
    }
}

void QDRuler::setDpi(const int dpi)
{
    if (m_dpi != dpi) {
        m_dpi = dpi;
        update();
    }
}

void QDRuler::mouseMoveEvent(QMouseEvent *event)
{
    m_CursorPos = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void QDRuler::paintEvent(QPaintEvent *event)
{
    //qDebug() << "Paso";
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
    QPen pen(Qt::black, 0); // zero width pen is cosmetic pen
    //pen.setCosmetic(true);
    painter.setPen(pen);

    // We want to work with floating point, so we are considering the rect as QRectF
    //QRectF rulerRect = qobject_cast<QWidget*>(parent())->rect(); //this->rect();
    if(m_RulerType == RulerType::Vertical)
        setFixedHeight(qobject_cast<QGraphicsView*>(parent())->scene()->height()*m_RulerZoom);
    else
        setFixedWidth(qobject_cast<QGraphicsView*>(parent())->scene()->width()*m_RulerZoom);
    //m_horizontalRuler->setFixedWidth(scene->width());

    QRectF rulerRect = this->rect();

    //qDebug() << rulerRect;

    // at first fill the rect
    painter.fillRect(rulerRect, Qt::lightGray);
    //painter.fillRect(rulerRect, QColor(220,200,180));
    //painter.fillRect(rulerRect, QColor(236, 233, 216));

    // drawing a scale of 1
    drawAScaleMeter(&painter, rulerRect, 1, (RulerType::Horizontal == m_RulerType ? rulerRect.height() : rulerRect.width())/2);

    // drawing the current mouse position indicator
    //painter.setOpacity(0.4);
    //drawMousePosTick(&painter);
    //painter.setOpacity(1.0);

    // drawing no man's land between the ruler & view
    QPointF starPt = RulerType::Horizontal == m_RulerType ? rulerRect.bottomLeft() : rulerRect.topRight();
    QPointF endPt = RulerType::Horizontal == m_RulerType ? rulerRect.bottomRight() : rulerRect.bottomRight();
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(starPt, endPt);
}

void QDRuler::drawAScaleMeter(QPainter *painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
{
    // rulerRect está en pixels
    // startPositoin está en pixels
    // scaleMeter es la separación entre marcas (mm)

    // Flagging whether we are horizontal or vertical only to reduce
    // to cheching many times
    bool isHorzRuler = RulerType::Horizontal == m_RulerType;

    // Ruler rectangle starting mark
    qreal rulerStartMark = isHorzRuler ? rulerRect.left() : rulerRect.top();
    // Ruler rectangle ending mark
    qreal rulerEndMark = isHorzRuler ? rulerRect.right() : rulerRect.bottom();

    if (m_Origin >= rulerStartMark && m_Origin <= rulerEndMark)
    {
        // Condition A # If origin point is between the start & end mard,
        // we have to draw both from origin to left mark & origin to right mark.
        //qDebug() << "A";

        drawFromOriginTo(painter, rulerRect, m_Origin, rulerEndMark, 0, scaleMeter, startPositoin);
        //drawFromOriginTo(painter, rulerRect, m_Origin, rulerStartMark, 0, -scaleMeter, startPositoin);
    }
    else if (m_Origin < rulerStartMark)
    {
        //qDebug() << "B";
        // Condition B # If origin point is left of the start mark, we have to draw
        // from origin to end mark.

        int tickNo = int((rulerStartMark - m_Origin) / scaleMeter);

        //qDebug() << m_Origin << rulerStartMark << tickNo;
        //drawFromOriginTo(painter, rulerRect, m_Origin + scaleMeter * tickNo, rulerEndMark, tickNo, scaleMeter, startPositoin);
        drawFromOriginTo(painter, rulerRect, tickNo, rulerEndMark, tickNo, scaleMeter, startPositoin);
    }
    else if (m_Origin > rulerEndMark)
    {
        // Condition C # If origin point is right of the end mark, we have to draw
        // from origin to start mark.
        //qDebug() << "C";

        int tickNo = int((m_Origin - rulerEndMark) / scaleMeter);
        drawFromOriginTo(painter, rulerRect, m_Origin - scaleMeter * tickNo, rulerStartMark, tickNo, -scaleMeter, startPositoin);
    }
}

void QDRuler::drawFromOriginTo(QPainter *painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo, qreal step, qreal startPosition)
{
    calcAdvance();

    // rulerRect está en pixels
    // startMark está en pixels
    //m_advance = toMM();
    //qDebug() << m_advance;

    bool isHorzRuler = RulerType::Horizontal == m_RulerType;
    qreal const pixToMM = toMM();
    //qDebug() << "pix to mm:"<< pixToMM << "zoom:" << m_RulerZoom << "ruler rect:" << rulerRect << "start mark:" << startMark << "end mark:" << endMark << "start tick no:" << startTickNo << "step:" << step << "start pos:" << startPosition;

    //if(isHorzRuler) {
        //painter->drawLine(startMark, rulerRect.top() + startPosition, endMark, rulerRect.top() + startPosition);
    //}

    int marcas{10};
    if(m_RulerZoom<1.0)
        marcas = (1-m_RulerZoom)*10 + 10;

    qreal pos = 0;
    int n{ 0 };

    while(pos < (endMark-startTickNo)) {
        //qDebug() << posMM;
        int startPos = startPosition;
        if((n % marcas) == 0) {
            startPos = 0;
        }

        qreal x1 = isHorzRuler ? pos : rulerRect.left() + startPos;
        qreal y1 = isHorzRuler ? rulerRect.top() + startPos : pos;
        qreal x2 = isHorzRuler ? pos : rulerRect.right();
        qreal y2 = isHorzRuler ? rulerRect.bottom() : pos;
        painter->drawLine(QLineF(x1, y1, x2, y2));
        if((n % marcas) == 0) {
            qreal posMM =  std::round((startTickNo+pos) / m_advance);
            //qDebug() << posMM;

            QPainterPath txtPath;
            txtPath.addText(x1 + 1, y1 + (isHorzRuler ? 7 : -2), this->font(), QString::number(qAbs(posMM)));
            painter->drawPath(txtPath);
        }
        pos = pos + m_advance;
        n++;
    }

/*
    bool isHorzRuler = RulerType::Horizontal == m_RulerType;
    int iterate = 0;

    for (qreal current = startMark; (step < 0 ? current >= endMark : current <= endMark); current += step)
    {
        qreal x1 = isHorzRuler ? current : rulerRect.left() + startPosition;
        qreal y1 = isHorzRuler ? rulerRect.top() + startPosition : current;
        qreal x2 = isHorzRuler ? current : rulerRect.right();
        qreal y2 = isHorzRuler ? rulerRect.bottom() : current;
        painter->drawLine(QLineF(x1, y1, x2, y2));
        if (m_DrawText)
        {
            QPainterPath txtPath;
            txtPath.addText(x1 + 1,y1 + (isHorzRuler ? 7 : -2),this->font(),QString::number(qAbs(int(step) * startTickNo++)));
            painter->drawPath(txtPath);
            iterate++;
        }
    }
*/
}

/*
void QDRuler::drawMousePosTick(QPainter *painter)
{
    if (m_MouseTracking)
    {
        QPoint starPt = m_CursorPos;
        QPoint endPt;
        if (RulerType::Horizontal == m_RulerType)
        {
            starPt.setY(this->rect().top());
            endPt.setX(starPt.x());
            endPt.setY(this->rect().bottom());
        }
        else
        {
            starPt.setX(this->rect().left());
            endPt.setX(this->rect().right());
            endPt.setY(starPt.y());
        }
        painter->drawLine(starPt,endPt);
    }
}
*/

qreal QDRuler::toMM()
{
    if(m_dpi != 0)
        return 25.4 / m_dpi;
    return 0.0;
}

void QDRuler::calcAdvance()
{
    m_advance = 1 * m_RulerZoom / toMM();
    //m_advance = m_advance * 2.54;
}
