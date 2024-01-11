#include "ruler.h"
#include <QPainter>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QFont>
#include <QDebug>

Ruler::Ruler(QAbstractScrollArea *parent, RulerType rulerType)
    : QWidget{ parent }
    , m_rulerType{ rulerType }
    , m_offset{ 0 }
    , m_dpi{ 96 }
    , m_scaleFactor{ 1.0 }
{
    calcSize();
    if(m_rulerType == RulerType::vertical) {
        move(0, 40);
        connect(parent->verticalScrollBar(), &QScrollBar::valueChanged, this, &Ruler::setOffset);
    } else {
        move(40, 0);
        connect(parent->horizontalScrollBar(), &QScrollBar::valueChanged, this, &Ruler::setOffset);
    }
}

void Ruler::calcSize()
{
    if(m_rulerType == RulerType::vertical) {
        setFixedSize(40, qobject_cast<QAbstractScrollArea*>(parent())->height());
    } else {
        setFixedSize(qobject_cast<QAbstractScrollArea*>(parent())->width(), 40);
    }
}

void Ruler::setOffset(int value)
{
    m_offset = value;
    update();
}

void Ruler::setScale(qreal scaleFactor)
{
    m_scaleFactor = m_scaleFactor * scaleFactor;
    update();
    qobject_cast<QAbstractScrollArea*>(parent())->verticalScrollBar()->setValue(0);
    qobject_cast<QAbstractScrollArea*>(parent())->horizontalScrollBar()->setValue(0);
}

void Ruler::resetScale()
{
    m_scaleFactor = 1.0;
    update();
    qobject_cast<QAbstractScrollArea*>(parent())->verticalScrollBar()->setValue(0);
    qobject_cast<QAbstractScrollArea*>(parent())->horizontalScrollBar()->setValue(0);
}

void Ruler::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont font{ "Arial", 8 };

    painter.setFont(font);
    QFontMetrics fm(font);

    qreal const pixToMM = toMM();

    if(m_rulerType == RulerType::vertical) {
        painter.translate(0, -m_offset);
        int const heightMM = height() * pixToMM;

        qreal maxPos;
        int marcas;
        if(m_scaleFactor<1) {
            marcas = floor((1.0 - m_scaleFactor) * 10);
            maxPos = heightMM * 1/m_scaleFactor;
        } else {
            maxPos = heightMM;
            marcas = 1;
        }

        for (int position = 0; position < maxPos; ++position) {
            int const positionInPix = int(position / pixToMM * m_scaleFactor);

            //qDebug() << heightMM << position << positionInPix;
            if(position % marcas == 0) {
                if (position % 10 == 0) {
                    if (position != 0) {
                        QString const txt = QString::number(position);
                        QRect txtRect = fm.boundingRect(txt).translated(0, positionInPix);
                        txtRect.translate(2, txtRect.height()/2 - font.pointSize());
                        painter.drawText(txtRect, txt);
                    }
                    painter.drawLine(width() - 18, positionInPix, width(), positionInPix);
                } else {
                    painter.drawLine(width() - 10, positionInPix, width(), positionInPix);
                }
            }
        }
    } else {
        painter.translate(-m_offset, 0);
        int const widthMM = width() * pixToMM;
        qreal maxPos;
        int marcas;
        if(m_scaleFactor<1) {
            marcas = floor((1.0 - m_scaleFactor) * 10);
            maxPos = widthMM * 1/m_scaleFactor;
        } else {
            maxPos = widthMM;
            marcas = 1;
        }

        for (int position = 0; position < maxPos; ++position) {
            int const positionInPix = int(position / pixToMM * m_scaleFactor);
            if(position % marcas == 0) {
                if (position % 10 == 0) {
                    if (position != 0) {
                        QString const txt = QString::number(position);
                        QRect txtRect = fm.boundingRect(txt).translated(positionInPix, 0);
                        txtRect.translate(2, font.pointSize()+4);
                        painter.drawText(txtRect, txt);
                    }
                    painter.drawLine(positionInPix, height() - 18, positionInPix, height());
                } else {
                    painter.drawLine(positionInPix, height() - 10, positionInPix, height());
                }
            }
        }
    }
}

void Ruler::resizeEvent(QResizeEvent *event)
{

    if(m_rulerType == RulerType::vertical) {
        int const maximumMM = event->size().height() * toMM();
        QFontMetrics fm(font());
        int w = fm.horizontalAdvance(QString::number(maximumMM)) + 20;
        if (w != event->size().width())
        {
            QSize const newSize(w, event->size().height());
            emit sizeChanged(newSize);
            return setFixedSize(newSize);
        }
    } else {
        int const maximumMM = event->size().width() * toMM();
        QFontMetrics fm(font());
        int h = fm.horizontalAdvance(QString::number(maximumMM)) + 20;
        if (h != event->size().height())
        {
            QSize const newSize(event->size().width(), h);
            emit sizeChanged(newSize);
            return setFixedSize(newSize);
        }
    }

    return QWidget::resizeEvent(event);
}
