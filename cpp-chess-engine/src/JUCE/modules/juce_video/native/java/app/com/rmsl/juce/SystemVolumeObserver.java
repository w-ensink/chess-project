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

import android.database.ContentObserver;
import android.app.Activity;
import android.net.Uri;

//==============================================================================
public class SystemVolumeObserver extends ContentObserver
{
    private native void mediaSessionSystemVolumeChanged (long host);

    SystemVolumeObserver (Activity activityToUse, long hostToUse)
    {
        super (null);

        activity = activityToUse;
        host = hostToUse;
    }

    void setEnabled (boolean shouldBeEnabled)
    {
        if (shouldBeEnabled)
            activity.getApplicationContext ().getContentResolver ().registerContentObserver (android.provider.Settings.System.CONTENT_URI, true, this);
        else
            activity.getApplicationContext ().getContentResolver ().unregisterContentObserver (this);
    }

    @Override
    public void onChange (boolean selfChange, Uri uri)
    {
        if (uri.toString ().startsWith ("content://settings/system/volume_music"))
            mediaSessionSystemVolumeChanged (host);
    }

    private Activity activity;
    private long host;
}
