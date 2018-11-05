#include "spriteframe.h"
#include <string>

using namespace std;

SpriteFrame::SpriteFrame() {

}

/**
 * @brief SpriteFrame::load
 * @param path
 */
void SpriteFrame::load(QString path ) {
	image.load(path);

	_width = image.width();
	_height = image.height();

	invalidatePixmap();
}

/**
 * @brief SpriteFrame::GetImage
 * Grants access to this frame's QImage for manipulation.
 * @return
 */
QImage& SpriteFrame::getImage() {
	invalidatePixmap();
	return image;
}

/**
 * @brief SpriteFrame::GetPixMap
 * @return The current pixmap. It is made if needed.
 */
QPixmap& SpriteFrame::getPixMap() {
	validatePixmapMaybe();
	return pixmap;
}

/**
 * @brief SpriteFrame::InvalidatePixmap
 * Flags the pixmap as requiring an update the next time it is needed.
 */
void SpriteFrame::invalidatePixmap() {
	pixmapValid = false;
}

/**
 * @brief SpriteFrame::ValidatePixmapMaybe
 * Uses the QImage to create the pixmap, but only
 * if it needs to.
 */
void SpriteFrame::validatePixmapMaybe() {
	if ( pixmapValid ) {
		return;
	}

	pixmap = QPixmap::fromImage(image);
	pixmapValid = true;
}

float SpriteFrame::width() {
	return _width;
}

float SpriteFrame::height() {
	return _height;
}