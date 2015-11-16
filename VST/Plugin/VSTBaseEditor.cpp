#include <VST/Plugin/VSTBaseEditor.h>

using namespace eLibV2;
using namespace eLibV2::VST::Plugin;

VSTBaseEditor::VSTBaseEditor(VSTBaseClass *effect) : AEffGUIEditor(effect)
{
    mEffect = effect;
}

VSTBaseClass* VSTBaseEditor::getEffect()
{
    return mEffect;
}

void VSTBaseEditor::setupEditor(const EditorProperties properties)
{
    mProperties = properties;

    // load the page background bitmaps
    for (std::map<UInt8, UInt16>::iterator it = properties.mBackgroundBitmaps.begin(); it != properties.mBackgroundBitmaps.end(); ++it)
    {
        if (it->second != 0)
            bitmapManager.addBackgroundBitmap(it->first, new CBitmap(it->second));
    }

    // load all control bitmaps
    for (std::map<std::string, UInt16>::iterator it = mProperties.mControlBitmaps.begin(); it != mProperties.mControlBitmaps.end(); it++)
    {
        if ((it->first != "") && (it->second != 0))
            bitmapManager.addBitmap(it->first, new CBitmap(it->second));
    }

    // init the size of the plugin
    mActivePage = 0;
    rect.left   = 0;
    rect.top    = 0;

#if AU
    rect.right  = (short)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getWidth();
    rect.bottom = (short)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getHeight();
#else
    rect.right  = (SInt16)bitmapManager.getBackgroundBitmap(mActivePage)->getWidth();
    rect.bottom = (SInt16)bitmapManager.getBackgroundBitmap(mActivePage)->getHeight();
#endif
}

//-----------------------------------------------------------------------------
bool VSTBaseEditor::open(void *ptr)
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "open editor");

#if VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR == 0
    // !!! always call this !!!
    AEffGUIEditor::open(ptr);
#endif

    //--init background frame-----------------------------------------------
    // We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
    // If all GUI objects are created we assign our class member to this one. See bottom of this method.
    CRect size(0, 0, bitmapManager.getBackgroundBitmap(mActivePage)->getWidth(), bitmapManager.getBackgroundBitmap(mActivePage)->getHeight());
    CFrame* lFrame = new CFrame(size, ptr, this);

    // setup editor pages
    for (std::map<UInt8, UInt16>::iterator it = mProperties.mBackgroundBitmaps.begin(); it != mProperties.mBackgroundBitmaps.end(); it++)
        mEditorPage.push_back(new CViewContainer(size, lFrame, bitmapManager.getBackgroundBitmap(it->first)));

    lFrame->addView(mEditorPage[mActivePage]);

    // init things in open method of derived class (design pattern "template method" ;-) )
    openInvoked();

    // switch frame to locally created after initialization
    frame = lFrame;
    frame->setDirty(true);

    return true;
}

void VSTBaseEditor::close()
{
    ModuleLogger::print(LOG_CLASS_VSTBASE, "close editor");

#if VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR == 0
    // !!! always call this !!!
    AEffGUIEditor::close();
#endif

    if (frame)
    {
        // add all non-child container
        for (std::vector<CViewContainer*>::iterator it = mEditorPage.begin(); it != mEditorPage.end(); ++it)
        {
            if (!frame->isChild((*it)))
                frame->addView((*it));
        }
        delete frame;
    }

    // remove controls and editor pages
    removeControls();
    mEditorPage.clear();
}

void VSTBaseEditor::idle()
{
    //ModuleLogger::print(LOG_CLASS_VSTBASE, "idle");

    AEffGUIEditor::idle();
}

CView* VSTBaseEditor::addControl(const EditorParameter::ControlType type, CControlListener *listener, const CPoint position, const UInt16 tag, const UInt16 numBitmaps, const std::string bitmapId, CPoint handle)
{
    CPoint bitmapSize(getBitmap(bitmapId)->getWidth(), getBitmap(bitmapId)->getHeight() / numBitmaps);
    CRect tempSize(position.x, position.y, bitmapSize.x + position.x, bitmapSize.y + position.y);

    switch (type)
    {
        case EditorParameter::OnOffButton:
            mOnOffButtons[tag] = new COnOffButton(tempSize, listener, tag, getBitmap(bitmapId));
            return mOnOffButtons[tag];

        case EditorParameter::KickButton:
            mKickButtons[tag] = new CKickButton(tempSize, listener, tag, CCoord(bitmapSize.y), getBitmap(bitmapId), handle);
            return mKickButtons[tag];

        case EditorParameter::AnimKnob:
            mAnimKnobs[tag] = new CAnimKnob(tempSize, listener, tag, numBitmaps, getBitmap(bitmapId)->getHeight() / numBitmaps, getBitmap(bitmapId), handle);
            return mAnimKnobs[tag];

        case EditorParameter::MovieBitmap:
            mMovieBitmap[tag] = new CMovieBitmap(tempSize, listener, tag, numBitmaps, getBitmap(bitmapId)->getHeight() / numBitmaps, getBitmap(bitmapId), handle);
            return mMovieBitmap[tag];
    }
    return NULL;
}

void VSTBaseEditor::setControlValue(const EditorParameter::ControlType type, const UInt16 tag, const double value)
{
    switch (type)
    {
        case EditorParameter::OnOffButton:
            if (mOnOffButtons.count(tag) != 0)
                mOnOffButtons[tag]->setValue(value);
            break;

        case EditorParameter::KickButton:
            if (mKickButtons.count(tag) != 0)
                mKickButtons[tag]->setValue(value);
            break;

        case EditorParameter::AnimKnob:
            if (mAnimKnobs.count(tag) != 0)
                mAnimKnobs[tag]->setValue(value);
            break;

        case EditorParameter::MovieBitmap:
            if (mMovieBitmap.count(tag) != 0)
                mMovieBitmap[tag]->setValue(value);
            break;
    }
}

void VSTBaseEditor::removeControls()
{
    if (mOnOffButtons.size())
        mOnOffButtons.clear();
    if (mKickButtons.size())
        mKickButtons.clear();
    if (mTextLabels.size())
        mTextLabels.clear();
    if (mOptionMenus.size())
        mOptionMenus.clear();
    if (mAnimKnobs.size())
        mAnimKnobs.clear();
    if (mMovieBitmap.size())
        mMovieBitmap.clear();
}

void VSTBaseEditor::attachToPage(const UInt8 pageIndex, CView* control)
{
    if (pageIndex < mEditorPage.size())
        mEditorPage[pageIndex]->addView(control);
}

CBitmap* VSTBaseEditor::getBitmap(const std::string bitmapId)
{
    return bitmapManager.getBitmap(bitmapId);
}
