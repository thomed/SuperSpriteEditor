#include "animationtimeline.h"

AnimationTimeline::AnimationTimeline(QVBoxLayout* layout, SpriteModel& model, QObject *parent)
    : timelineLayout(layout), model(&model)
{
    //Set up the first frame
    QPushButton *frameButton = new QPushButton;
    frameButton->setText("first");
    tempAddingCounter++;
    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);
    //Set icon
    setButtonIcon(0);
    //Add new frame
    frameButtons.push_back(frameButton);
    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);
    timelineLayout->addWidget(frameButton, Qt::AlignTop);
    frameButton->show();

    //Select new frame
    emit setSelectedFrame(0);

    QPushButton *plusButton = new QPushButton;
    plusButton->setText("+");
    plusButton->setFixedHeight(30);
    plusButton->setFixedWidth(30);
    QObject::connect(plusButton, &QPushButton::pressed, this,
                     &AnimationTimeline::addNewBlankFrame);
    timelineLayout->addWidget(plusButton, 0, Qt::AlignTop);

    frameButtons.push_back(plusButton);

}

AnimationTimeline::~AnimationTimeline(){
    size_t numFrames = frameButtons.size();
    for(size_t i = 0; i < numFrames; i++){
        delete frameButtons[i];
    }
}

void AnimationTimeline::duplicateFrame(int frameIndex){

}

void AnimationTimeline::addNewFrame(SpriteFrame newFrame){
    QPushButton *frameButton = new QPushButton;
    //Numbering for testing
    frameButton->setText(QString::number(tempAddingCounter));
    tempAddingCounter++;

    //Add frame to animation
    size_t newFrameIndex = model->getAnimation().length();
    model->getAnimation().insertFrame(newFrameIndex, newFrame);

    frameButton->setFixedWidth(buttonSize);
    frameButton->setFixedHeight(buttonSize);

    //Set in animation object
    setButtonIcon(newFrameIndex);

    //Add new frame
    frameButtons.insert(frameButtons.begin()+(newFrameIndex-1), frameButton);

    QObject::connect(frameButton, &QPushButton::pressed, this, &AnimationTimeline::selectFrame);

    timelineLayout->insertWidget(newFrameIndex-1, frameButton, Qt::AlignTop);
    frameButton->show();

    //Select new frame
    emit setSelectedFrame(newFrameIndex);

    std::cout<< "adding frame"<<std::endl;

}

/**
 * @brief Remove the button corresponding to the index, and the frame from the animation.
 * @param frameIndex
 */
void AnimationTimeline::deleteFrame(int frameIndex){
    QPushButton* remove = frameButtons.at(frameIndex);
    frameButtons.erase(frameButtons.begin() + frameIndex);

    timelineLayout->removeWidget(remove);
    //model->getAnimation().removeFrame(frameIndex);

    delete remove;
}

void AnimationTimeline::moveFrame(SpriteFrame frameToMove, int index){

}

/**
 * @brief Set the button at the given index 's icon to the corresponding
 * image in the animation object.
 * @param index
 */
void AnimationTimeline::setButtonIcon(size_t index){
    //Animation ani = emit getAnimation();

    SpriteFrame frame = model->getFrame(index);

    //Animation anim = model->getAnimation();
    //SpriteFrame frame2 = anim.getFrame(0);

    QPixmap pix = frame.getPixMap();
    QIcon ButtonIcon(pix);
    //frameButtons[index]->setIcon(ButtonIcon);

}

/**
 * @brief Add a new blank frame to this animation.
 */
void AnimationTimeline::addNewBlankFrame(){
    SpriteFrame newFrame(model->getAnimation().width, model->getAnimation().height);
    addNewFrame(newFrame);
}

/**
 * @brief Select the current frame (to change drawing area)
 * The button's pointer is passed in to identify the frame.
 * Emits setSelectedFrame to change which frame is selected inthe drawing
 * area.
 */
void AnimationTimeline::selectFrame(){
    QPushButton* send = qobject_cast<QPushButton*>(sender());
    selectedButton = send;

    size_t index = find(frameButtons.begin(), frameButtons.end(), send) - frameButtons.begin();

    emit setSelectedFrame(index);

    std::cout << "selecting " << index << std::endl;
}

