#include "animation.h"

Animation::Animation() {

}

Animation::Animation(int width,int height ) : width(width), height(height) {
	auto newFrame = SpriteFrame(width,height);
	insertFrame(0,newFrame);
}

/**
 * Inserts the given frame at the given position.
 * If no SpriteFrame object is given, an empty SpriteFrame is added.
 */
void Animation::insertFrame(int index, SpriteFrame frame){
    auto iterator = frames.begin();
    frames.insert(iterator + index, frame);
}

void Animation::removeFrame(int index){
    auto iterator = frames.begin();
    frames.erase(iterator + index);
}

void Animation::moveFrame(int oldIndex, int newIndex){
    SpriteFrame frame = frames[oldIndex];
    removeFrame(oldIndex);
    insertFrame(newIndex, frame);
}

SpriteFrame& Animation::getFrame(int index){
    return frames[index];
}

int Animation::length() {
    return frames.size();
}
