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

#pragma once

#include "../Project/jucer_Project.h"

//==============================================================================
class NewFileWizard
{
public:
    //==============================================================================
    NewFileWizard();
    ~NewFileWizard();

    //==============================================================================
    class Type
    {
    public:
        Type() {}
        virtual ~Type()  {}

        //==============================================================================
        virtual String getName() = 0;
        virtual void createNewFile (Project&, Project::Item projectGroupToAddTo) = 0;

    protected:
        //==============================================================================
        File askUserToChooseNewFile (const String& suggestedFilename, const String& wildcard,
                                     const Project::Item& projectGroupToAddTo);

        static void showFailedToWriteMessage (const File& file);
    };

    //==============================================================================
    void addWizardsToMenu (PopupMenu&) const;
    bool runWizardFromMenu (int chosenMenuItemID, Project&,
                            const Project::Item& projectGroupToAddTo) const;

    void registerWizard (Type*);

private:
    OwnedArray<Type> wizards;
};