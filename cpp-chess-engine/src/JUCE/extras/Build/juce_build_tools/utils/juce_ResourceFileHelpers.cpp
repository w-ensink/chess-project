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
namespace build_tools
{
    uint64 calculateStreamHashCode (InputStream& in)
    {
        uint64 t = 0;

        const int bufferSize = 4096;
        HeapBlock<uint8> buffer;
        buffer.malloc (bufferSize);

        for (;;)
        {
            auto num = in.read (buffer, bufferSize);

            if (num <= 0)
                break;

            for (int i = 0; i < num; ++i)
                t = t * 65599 + buffer[i];
        }

        return t;
    }

    uint64 calculateFileHashCode (const File& file)
    {
        std::unique_ptr<FileInputStream> stream (file.createInputStream());
        return stream != nullptr ? calculateStreamHashCode (*stream) : 0;
    }

    uint64 calculateMemoryHashCode (const void* data, size_t numBytes)
    {
        uint64 t = 0;

        for (size_t i = 0; i < numBytes; ++i)
            t = t * 65599 + static_cast<const uint8*> (data)[i];

        return t;
    }

    bool overwriteFileWithNewDataIfDifferent (const File& file, const void* data, size_t numBytes)
    {
        if (file.getSize() == (int64) numBytes
            && calculateMemoryHashCode (data, numBytes) == calculateFileHashCode (file))
            return true;

        if (file.exists())
            return file.replaceWithData (data, numBytes);

        return file.getParentDirectory().createDirectory() && file.appendData (data, numBytes);
    }

    bool overwriteFileWithNewDataIfDifferent (const File& file, const MemoryOutputStream& newData)
    {
        return overwriteFileWithNewDataIfDifferent (file, newData.getData(), newData.getDataSize());
    }

    bool overwriteFileWithNewDataIfDifferent (const File& file, const String& newData)
    {
        const char* const utf8 = newData.toUTF8();
        return overwriteFileWithNewDataIfDifferent (file, utf8, strlen (utf8));
    }
}
}
