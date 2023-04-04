#include "wyswietlaczview.h"

WyswietlaczView::WyswietlaczView(QWidget * parent) : QGraphicsView(parent)
{
    this->setBackgroundBrush(QBrush(Qt::gray));
    this->fitInView(QRectF(0,0,1322,662),Qt::AspectRatioMode::KeepAspectRatio);
}

void WyswietlaczView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.15;

    if(event->angleDelta().y() > 0)
    {
        scale(scaleFactor,scaleFactor);
    }
    else
    {
        scale(1/scaleFactor,1/scaleFactor);
    }

}


