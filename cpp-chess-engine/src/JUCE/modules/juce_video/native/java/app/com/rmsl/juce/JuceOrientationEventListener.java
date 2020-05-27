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

package com.rmsl.juce;

import android.view.OrientationEventListener;
import android.content.Context;

public class JuceOrientationEventListener extends OrientationEventListener
{
    private native void deviceOrientationChanged (long host, int orientation);

    public JuceOrientationEventListener (long hostToUse, Context context, int rate)
    {
        super (context, rate);

        host = hostToUse;
    }

    @Override
    public void onOrientationChanged (int orientation)
    {
        deviceOrientationChanged (host, orientation);
    }

    private long host;
}
