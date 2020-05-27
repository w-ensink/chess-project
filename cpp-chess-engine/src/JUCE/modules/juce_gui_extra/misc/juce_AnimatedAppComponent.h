/*
  ==============================================================================

   This file is part of the JUCE 6 technical preview.
   Copyright (c) 2020 - Raw Material Software Limited

   You may use this code under the terms of the GPL v3
   (see www.gnu.org/licenses).

   For this technical preview, this file is not subject to commercial licensing.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

//==============================================================================
/**
    A base class for writing simple one-page graphical apps.

    A subclass can inherit from this and implement just a few methods such as
    paint() and mouse-handling. The base class provides some simple abstractions
    to take care of continuously repainting itself.

    @tags{GUI}
*/
class AnimatedAppComponent   : public Component,
                               private Timer
{
public:
    AnimatedAppComponent();

    /** Your subclass can call this to start a timer running which will
        call update() and repaint the component at the given frequency.
    */
    void setFramesPerSecond (int framesPerSecond);

    /** Called periodically, at the frequency specified by setFramesPerSecond().
        This is a the best place to do things like advancing animation parameters,
        checking the mouse position, etc.
    */
    virtual void update() = 0;

    /** Returns the number of times that update() has been called since the component
        started running.
    */
    int getFrameCounter() const noexcept        { return totalUpdates; }

    /** When called from update(), this returns the number of milliseconds since the
        last update call.
        This might be useful for accurately timing animations, etc.
    */
    int getMillisecondsSinceLastUpdate() const noexcept;

private:
    //==============================================================================
    Time lastUpdateTime;
    int totalUpdates;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatedAppComponent)
};

} // namespace juce
