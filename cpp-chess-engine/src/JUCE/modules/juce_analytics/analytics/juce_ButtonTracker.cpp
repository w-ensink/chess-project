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

ButtonTracker::ButtonTracker (Button& buttonToTrack,
                              const String& triggeredEventName,
                              const StringPairArray& triggeredEventParameters,
                              int triggeredEventType)
    : button (buttonToTrack),
      eventName (triggeredEventName),
      eventParameters (triggeredEventParameters),
      eventType (triggeredEventType)
{
    button.addListener (this);
}

ButtonTracker::~ButtonTracker()
{
    button.removeListener (this);
}

void ButtonTracker::buttonClicked (Button* b)
{
    if (b == &button)
    {
        auto params = eventParameters;

        if (button.getClickingTogglesState())
            params.set ("ButtonState", button.getToggleState() ? "On" : "Off");

        Analytics::getInstance()->logEvent (eventName, params, eventType);
    }
}


} // namespace juce
