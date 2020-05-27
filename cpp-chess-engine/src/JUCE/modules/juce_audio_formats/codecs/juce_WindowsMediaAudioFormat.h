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

#if JUCE_WINDOWS || DOXYGEN

//==============================================================================
/**
    Audio format which uses the Windows Media codecs (Windows only).

    @tags{Audio}
*/
class WindowsMediaAudioFormat  : public AudioFormat
{
public:
    //==============================================================================
    WindowsMediaAudioFormat();
    ~WindowsMediaAudioFormat();

    //==============================================================================
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;
    bool isCompressed() override;

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails) override;

    AudioFormatWriter* createWriterFor (OutputStream*, double sampleRateToUse,
                                        unsigned int numberOfChannels, int bitsPerSample,
                                        const StringPairArray& metadataValues, int qualityOptionIndex) override;
    using AudioFormat::createWriterFor;
};

#endif

}