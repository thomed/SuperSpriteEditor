#include "spritemodel.h"
#include <iostream>
#include <QDebug>
SpriteModel::SpriteModel(QObject *parent) : QObject(parent)
{
    //Default size
    animation = Animation(300,300);
    SpriteFrame newFrame;
    newFrame.load("://drhenrykillinger");
    animation.insertFrame(0, newFrame);
}

Animation& SpriteModel::getAnimation() {
    return animation;
}

void SpriteModel::setAnimation(Animation anim){
    animation = anim;
    currentIndex = 0;
    previewIndex = 0;
    notifyOfFrameChange();
	emit animationChanged();
}

void SpriteModel::setCurrentFrame(int index) {
    currentIndex = index;
    notifyOfFrameChange();
}

SpriteFrame& SpriteModel::getCurrentFrame() {
    return animation.getFrame(currentIndex);
}

SpriteFrame& SpriteModel::getFrame(int index) {
    return animation.getFrame(index);
}

int SpriteModel::getAnimationLength() {
    return animation.length();
}

void SpriteModel::notifyOfFrameChange() {
    emit currentFrameChanged();
}

void SpriteModel::flipCurrentFrameHorizontally() {
    SpriteFrame& frame = getCurrentFrame();
    QImage image = frame.getImage().mirrored(true, false);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::flipCurrentFrameVertically() {
    SpriteFrame& frame = getCurrentFrame();
    QImage image = frame.getImage().mirrored(false, true);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::rotateCurrentFrameClockWise() {
    SpriteFrame& frame = getCurrentFrame();
    QTransform transform;
    transform.rotate(90);
    QImage image = frame.getImage().transformed(transform);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::rotateCurrentFrameAntiClockWise() {
    SpriteFrame& frame = getCurrentFrame();
    QTransform transform;
    transform.rotate(-90);
    QImage image = frame.getImage().transformed(transform);
    frame.setImage( image);
    notifyOfFrameChange();
}

void SpriteModel::createNewAnimation(int width, int height) {
	auto animation = Animation(width, height);
	setAnimation(animation);
    emit animationChanged();
}

/**
 * @brief SpriteModel::getAndIncrementPreviewIndex
 * @param shouldIncrement
 * @return
 *
 * Returns either the index of the next sprite frame to display, or the index
 * of the currently displayed sprite based on the boolean parameter.
 */
int SpriteModel::getAndIncrementPreviewIndex(bool shouldIncrement){
    if(shouldIncrement){
        int animationLength = getAnimationLength();
        if(animationLength <= 1){
            previewIndex = 0;
            //Cannot mod zero
            return previewIndex;
        }
        previewIndex = (++previewIndex) % (animationLength - 1);
        return previewIndex;
    }
    else{
        return previewIndex;
    }
}

int SpriteModel::getCurrentIndex(){
    return currentIndex;
}
