#include "drawingtools.h"
#include <iostream>
#include <QPainter>

QColor DrawingTools::toolColor = QColor(0,0,0);
DrawingTools::ToolType DrawingTools::currentTool = DrawingTools::PEN;
unsigned int DrawingTools::penWidth = 1;

/**
 * A driver method to minimize the code needed to use the current
 * tool from other classes.
 */
void DrawingTools::useCurrentTool(QImage& image, QPoint& imagePoint) {
    switch(currentTool) {
        case PEN: {
            usePen(image, imagePoint);
            break;
        }
        case FILL: {
            QColor targetColor = image.pixelColor(imagePoint);
            useFill(image, imagePoint, targetColor);
            break;
        }
        case ERASE: {
            useErase(image, imagePoint);
            break;
        }
    }
}

/**
  * The pen tool simply sets the color of the pixel at this point.
 */
void DrawingTools::usePen(QImage& image, QPoint& imagePoint) {
    if (penWidth == 1) {
        image.setPixelColor(imagePoint, toolColor);
    } else {
        QPainter p(&image);
        // use the offset to keep large pencil sizes centered on the mouse
        int offset = penWidth / 2;
        p.fillRect(imagePoint.x() - offset, imagePoint.y() - offset, penWidth, penWidth, toolColor);
    }
}

/**
 * The fill tool implements the floodfill algorithm. Commonly referred
 * to as a "paint bucket" tool.
 */
void DrawingTools::useFill(QImage& image, QPoint imagePoint, QColor& target) {
    // return if point outside image, not equal to target, or already toolColor
    if (!isPointInImage(image, imagePoint) ||
        target == toolColor ||
        image.pixelColor(imagePoint) != target) {
        return;
    }

    image.setPixelColor(imagePoint, toolColor);

    useFill(image, QPoint(imagePoint.x(), imagePoint.y() + 1), target);
    useFill(image, QPoint(imagePoint.x(), imagePoint.y() - 1), target);
    useFill(image, QPoint(imagePoint.x() - 1, imagePoint.y()), target);
    useFill(image, QPoint(imagePoint.x() + 1, imagePoint.y()), target);
}

/**
  * The pen tool simply sets the color of the pixel at this point.
 */
void DrawingTools::useErase(QImage& image, QPoint& imagePoint) {
    if (penWidth == 1) {
        image.setPixelColor(imagePoint, QColor(0,0,0,0) );
    } else {
        QPainter p(&image);
        //p.setBackgroundMode(Qt::TransparentMode);
        p.setCompositionMode(QPainter::CompositionMode_Clear);
        int offset = penWidth / 2;
        p.eraseRect(imagePoint.x() - offset, imagePoint.y() - offset, penWidth, penWidth);
    }
}

/**
 * Simple helper that returns whether or not the point is in the image.
 */
bool DrawingTools::isPointInImage(QImage& image, QPoint& imagePoint) {
    return imagePoint.x() >= 0 && imagePoint.y() >= 0 && imagePoint.x() < image.width() && imagePoint.y() < image.height();
}
