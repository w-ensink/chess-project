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
    A Windows-specific class that can create and embed an ActiveX control inside
    itself.

    To use it, create one of these, put it in place and make sure it's visible in a
    window, then use createControl() to instantiate an ActiveX control. The control
    will then be moved and resized to follow the movements of this component.

    Of course, since the control is a heavyweight window, it'll obliterate any
    JUCE components that may overlap this component, but that's life.

    @tags{GUI}
*/
class JUCE_API  ActiveXControlComponent   : public Component
{
public:
    //==============================================================================
    /** Create an initially-empty container. */
    ActiveXControlComponent();

    /** Destructor. */
    ~ActiveXControlComponent();

    /** Tries to create an ActiveX control and embed it in this peer.

        The peer controlIID is a pointer to an IID structure - it's treated
        as a void* because when including the JUCE headers, you might not always
        have included windows.h first, in which case IID wouldn't be defined.

        e.g. @code
        const IID myIID = __uuidof (QTControl);
        myControlComp->createControl (&myIID);
        @endcode
    */
    bool createControl (const void* controlIID);

    /** Deletes the ActiveX control, if one has been created.
    */
    void deleteControl();

    /** Returns true if a control is currently in use. */
    bool isControlOpen() const noexcept                 { return control != nullptr; }

    /** Does a QueryInterface call on the embedded control object.

        This allows you to cast the control to whatever type of COM object you need.

        The iid parameter is a pointer to an IID structure - it's treated
        as a void* because when including the JUCE headers, you might not always
        have included windows.h first, in which case IID wouldn't be defined, but
        you should just pass a pointer to an IID.

        e.g. @code
        const IID iid = __uuidof (IOleWindow);

        IOleWindow* oleWindow = (IOleWindow*) myControlComp->queryInterface (&iid);

        if (oleWindow != nullptr)
        {
            HWND hwnd;
            oleWindow->GetWindow (&hwnd);

            ...

            oleWindow->Release();
        }
        @endcode
    */
    void* queryInterface (const void* iid) const;

    /** Set this to false to stop mouse events being allowed through to the control.
    */
    void setMouseEventsAllowed (bool eventsCanReachControl);

    /** Returns true if mouse events are allowed to get through to the control.
    */
    bool areMouseEventsAllowed() const noexcept                 { return mouseEventsAllowed; }

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;

    /** @internal */
    intptr_t offerEventToActiveXControl (void*);
    static intptr_t offerEventToActiveXControlStatic (void*);

private:
    class Pimpl;
    std::unique_ptr<Pimpl> control;
    bool mouseEventsAllowed = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveXControlComponent)
};

#endif

} // namespace juce