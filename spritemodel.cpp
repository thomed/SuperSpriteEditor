#include "spritemodel.h"

SpriteModel::SpriteModel(QObject *parent) : QObject(parent)
{
    SpriteFrame newFrame;
    newFrame.load("://drhenrykillinger");
    animation.insertFrame(0, newFrame);
}

Animation& SpriteModel::getAnimation() {
    return animation;
}

void SpriteModel::setAnimation(Animation* anim){
    animation = *anim;
    currentIndex = 0;
    notifyOfFrameChange();
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
