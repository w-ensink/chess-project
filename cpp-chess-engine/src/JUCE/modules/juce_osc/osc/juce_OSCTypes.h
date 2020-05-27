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
/** The type used for OSC type tags. */
using OSCType = char;


/** The type used for OSC type tag strings. */
using OSCTypeList = Array<OSCType>;

//==============================================================================
/** The definitions of supported OSC types and their associated OSC type tags,
    as defined in the OpenSoundControl 1.0 specification.

    Note: This implementation does not support any additional type tags that
    are not part of the specification.

    @tags{OSC}
*/
class JUCE_API  OSCTypes
{
public:
    static const OSCType int32;
    static const OSCType float32;
    static const OSCType string;
    static const OSCType blob;
    static const OSCType colour;

    static bool isSupportedType (OSCType type) noexcept
    {
        return type == OSCTypes::int32
            || type == OSCTypes::float32
            || type == OSCTypes::string
            || type == OSCTypes::blob
            || type == OSCTypes::colour;
    }
};


//==============================================================================
/**
    Holds a 32-bit RGBA colour for passing to and from an OSCArgument.
    @see OSCArgument, OSCTypes::colour
    @tags{OSC}
*/
struct OSCColour
{
    uint8 red, green, blue, alpha;

    uint32 toInt32() const;
    static OSCColour fromInt32 (uint32);
};


//==============================================================================
/** Base class for exceptions that can be thrown by methods in the OSC module.

    @tags{OSC}
*/
struct OSCException  : public std::exception
{
    OSCException (const String& desc)
        : description (desc)
    {
       #if ! JUCE_UNIT_TESTS
        DBG ("OSCFormatError: " + description);
       #endif
    }

    String description;
};

//==============================================================================
/** Exception type thrown when the OSC module fails to parse something because
    of a data format not compatible with the OpenSoundControl 1.0 specification.

    @tags{OSC}
*/
struct OSCFormatError : public OSCException
{
    OSCFormatError (const String& desc) : OSCException (desc) {}
};

//==============================================================================
/** Exception type thrown in cases of unexpected errors in the OSC module.

    Note: This should never happen, and all the places where this is thrown
    should have a preceding jassertfalse to facilitate debugging.

    @tags{OSC}
*/
struct OSCInternalError : public OSCException
{
    OSCInternalError (const String& desc) : OSCException (desc) {}
};

} // namespace juce
