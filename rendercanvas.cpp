#include "rendercanvas.h"
#include "drawingtools.h"
#include <iostream>
#include <QMouseEvent>
#include <QPointF>
#include <math.h>

using namespace std;

/**
 * @brief RenderCanvas
 * Qt constructor
 */
RenderCanvas::RenderCanvas(QWidget *parent) : QWidget(parent) {

//    currentFrame().load("://drhenrykillinger");
//	currentFrame().load("://images/black48p.png");
//	transparentBackground.load("://background");

}

/**
 * @brief paintEvent
 * Qt method called whenever a paint event occurs
 * @param event
 */
void RenderCanvas::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);


	paintGreyWorkspace(painter);
	paintBackground(painter);

	paintImage(painter);
}

void RenderCanvas::setModel(SpriteModel& newModel) {
    model = &newModel;
}

/**
 * @brief RenderCanvas::currentFrame
 * Used to route references of the current frame through one spot,
 * where we could set it to something for testing.
 * @return
 */
SpriteFrame& RenderCanvas::currentFrame() {
    return model->getCurrentFrame();
}

/**
 * @brief canvasPointToImagePoint
 * Converts a position in the canvas into a pixel position in the current frame.
 * Ensures that the position is actually within the current frame.
 * Takes a canvas point and a reference to a QPoint where the result pixel position
 * should be returned into.
 * @return true if the position is within the bounds of the current frame, false otherwise
 */
bool RenderCanvas::canvasPointToImagePoint(QPoint canvasPoint, QPoint& imagePoint) {
	QRect imageRect = getImageBounds();

	if ( imageRect.contains(canvasPoint) ) {
		float x = static_cast<float>(canvasPoint.x()) - static_cast<float>(imageRect.x());
		x /= static_cast<float>(imageRect.width());
		x *= currentFrame().width();
		float y = static_cast<float>(canvasPoint.y()) - static_cast<float>(imageRect.y());
		y /= static_cast<float>(imageRect.height());
		y *= currentFrame().height();
		imagePoint.setX(static_cast<int>(x));
		imagePoint.setY(static_cast<int>(y));
		return true;
	} else {
		return false;
	}
}

/**
 * @brief RenderCanvas::fitImageToFrame
 * Scales the current spriteframe to fit within the canvas area.
 */
void RenderCanvas::fitImageToFrame(){
	QSize canvasSize = this->size();
	float xScale = canvasSize.width()/currentFrame().width();
	float yScale = canvasSize.height()/currentFrame().height();
	scale = min(xScale,yScale);
	translation = QPoint(0,0);
	repaint();
}

/**
 * @brief getImageBounds
 * Returns the screen rectangle where the current image will be drawn within the canvas area.
 * Uses the current translation and scale.
 * @return
 */
QRect RenderCanvas::getImageBounds() {
	QSize size = getScaledImageSize();
	return QRect( translation.x(), translation.y(), size.width(), size.height() );
}

/**
 * @brief paintGreyWorkspace
 * Draws a grey workspace background across the entire canvas
 */
void RenderCanvas::paintGreyWorkspace(QPainter& painter) {
	QSize canvasSize = this->size();
	QRect canvasRect( 0,0, canvasSize.width(), canvasSize.height() );
	painter.fillRect( canvasRect, backgroundColor );
}


/**
 * @brief paintBackground
 * Draws the checkered workspace background across the sprite area
 */
void RenderCanvas::paintBackground(QPainter& painter) {
    QSize canvasSize = this->size();

	QSize scaledSize = getScaledImageSize();
    QRect imageBounds = getImageBounds();
    painter.fillRect(imageBounds, checkeredWorkspaceWhiteColor);

    float scaledCheckerSize = scaledInt(checkeredWorkspaceSize );

    bool offset = true;
    for( int x = 0; x < scaledSize.width(); x+= scaledCheckerSize ) {
        for( int y = offset? 0: scaledCheckerSize;
                y < scaledSize.height(); y+= scaledCheckerSize * 2 ) {
            int width = scaledCheckerSize;
            if (scaledSize.width() - x < scaledCheckerSize) {
                width = scaledSize.width() - x;
            }
            int height = scaledCheckerSize;
            if (scaledSize.height() - y < scaledCheckerSize) {
                height = scaledSize.height() - y;
            }
            QRect checker(x + translation.x(),y + translation.y(),
                          width,height);
            painter.fillRect(checker, checkeredWorkspaceGreyColor);
        }
        offset = !offset;
    }

    //painter.drawPixmap(translation.x(), translation.y(), scaledSize.width(),  scaledSize.height(), transparentBackground);

//	QSize canvasSize = this->size();

//	QSize backgroundSize = transparentBackground.size();

//	for( int x = 0; x < canvasSize.width(); x += backgroundSize.width() ) {
//		for( int y = 0; y < canvasSize.height(); y += backgroundSize.height() ) {
//			painter.drawPixmap(x,y,transparentBackground);
//		}
//	}
}

/**
 * @brief paintImage
 * Draws the current frame being edited using the current scale and translation
 */
void RenderCanvas::paintImage(QPainter& painter) {

	QSize scaledSize = getScaledImageSize();

    painter.drawPixmap(translation.x(), translation.y(), scaledSize.width(),  scaledSize.height(), currentFrame().getPixMap());
}

/**
 * @brief getScaledImageSize
 * @return Calculates and returns the current frames size in pixels scaled by the current scale
 */
QSize RenderCanvas::getScaledImageSize() {
    int newWidth = scaledInt(currentFrame().width());
    int newHeight = scaledInt(currentFrame().height());
	return QSize(newWidth, newHeight);
}

/**
 * @brief scaledInt
 * @return Scales a int by the current scale.
 * Just to ensure that float math is used consistently.
 */
int RenderCanvas::scaledInt(int value) {
	return floor( (float)value * scale );
}

/**
 * @brief scaledInt
 * @return Scales a int by the given scalar.
 * Just to ensure that float math is used consistently.
 */
int RenderCanvas::scaledInt(int value, float scale) {
	return floor( (float)value * scale );
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse pressed event occurs
 * @param event
 */
void RenderCanvas::mousePressEvent(QMouseEvent *event) {
	if (event->button()==Qt::LeftButton){
		drawing = true;
		QPoint imagePoint;
		if ( canvasPointToImagePoint(event->pos(), imagePoint ) ) {
            QImage& image = currentFrame().getImage();
			DrawingTools::useCurrentTool(image, imagePoint);
			repaint();
		}
	} else
	if (event->button()==Qt::MiddleButton){
		translating = true;
		mouseStartTranslationPosition = event->pos();
		startTranslationPosition = translation;
	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse released event occurs
 * @param event
 */
void RenderCanvas::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button()==Qt::LeftButton){
		drawing = false;
	} else
	if (event->button()==Qt::MiddleButton){
		translating = false;
	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a mouse move event occurs
 * @param event
 */
void RenderCanvas::mouseMoveEvent(QMouseEvent *event) {
	if ( translating ) {
		translation = startTranslationPosition + event->pos() - mouseStartTranslationPosition;
		repaint();
	} else
	if ( drawing ) {
		QPoint imagePoint;
		if ( canvasPointToImagePoint(event->pos(), imagePoint ) ) {
//			cout << imagePoint.x() << " " << imagePoint.y() << endl;
            QImage& image = currentFrame().getImage();

//			image.setPixelColor(imagePoint, toolColor );
			DrawingTools::useCurrentTool(image, imagePoint);
            //repaint();
            // firing a frame change signal will repaint the canvas already
            model->notifyOfFrameChange();
		}

	}
}

/**
 * @brief mousePressEvent
 * Qt method called whenever a scroll wheel event occurs
 * @param event
 */
void RenderCanvas::wheelEvent(QWheelEvent *event) {
	QSizeF oldScaledSize = getScaledImageSize();

	scale = scale *(1+event->angleDelta().y() * scaleScrollSpeed);

	// Clamp the scale
	if ( scale < minScale ) {
		scale = minScale;
	} else if (scale > maxScale) {
		scale = maxScale;
	}

	// translate the image so that it appears to pivot around the center of the image
	QSizeF newScaledSize = getScaledImageSize();
	translation.setX( translation.x() - (newScaledSize.width() - oldScaledSize.width()) * 0.5f);

	translation.setY( translation.y() - (newScaledSize.height() - oldScaledSize.height()) * 0.5f);

	repaint();
}
