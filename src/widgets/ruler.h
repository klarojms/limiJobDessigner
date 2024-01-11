#ifndef RULER_H
#define RULER_H

#include <QObject>
#include <QWidget>
#include <QAbstractScrollArea>
#include <QScrollBar>

class Ruler : public QWidget
{
    Q_OBJECT
public:
    enum class RulerType { vertical, horizontal };

    explicit Ruler(QAbstractScrollArea *parent, RulerType rulerType);

    void setOffset(int value);

    void setScale(qreal scaleFactor);

    void resetScale();

    void calcSize();

signals:
     void sizeChanged(QSize const&);

private:
    int m_offset;
    int m_dpi;
    RulerType m_rulerType;
    qreal m_scaleFactor;

    qreal toMM()
    {
        if(m_dpi != 0)
            return 25.4 / m_dpi;
    }

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // RULER_H
