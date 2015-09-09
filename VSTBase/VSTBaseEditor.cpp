#include <VSTBase/VSTBaseEditor.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

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
    for (std::vector<VstInt32>::iterator it = mProperties.mBackgroundBitmaps.begin(); it != mProperties.mBackgroundBitmaps.end(); it++)
        bitmapManager.addBitmap(*it, new CBitmap(*it));

    // init the size of the plugin
    mActivePage = 0;
    rect.left   = 0;
    rect.top    = 0;

#if AU
    rect.right  = (short)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getWidth();
    rect.bottom = (short)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getHeight();
#else
    rect.right  = (VstInt16)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getWidth();
    rect.bottom = (VstInt16)bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getHeight();
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
    CRect size(0, 0, bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getWidth(), bitmapManager.getBitmap(mProperties.mBackgroundBitmaps[mActivePage])->getHeight());
    CFrame* lFrame = new CFrame(size, ptr, this);

    // setup editor pages
    for (std::vector<VstInt32>::iterator it = mProperties.mBackgroundBitmaps.begin(); it != mProperties.mBackgroundBitmaps.end(); it++)
        mEditorPage.push_back(new CViewContainer(size, lFrame, bitmapManager.getBitmap(*it)));

    lFrame->addView(mEditorPage[mActivePage]);

    // load all control bitmaps
    for (std::vector<VstInt32>::iterator it = mProperties.mControlBitmaps.begin(); it != mProperties.mControlBitmaps.end(); it++)
        bitmapManager.addBitmap(*it, new CBitmap(*it));

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

    // clear all editor pages
    mEditorPage.clear();

    mKickButtons.clear();

    // forget all control bitmaps
    for (std::vector<VstInt32>::iterator it = mProperties.mControlBitmaps.begin(); it != mProperties.mControlBitmaps.end(); it++)
        bitmapManager.forgetBitmap(*it);

    if (frame)
        delete frame;
    frame = 0;
}

void VSTBaseEditor::idle()
{
    //ModuleLogger::print(LOG_CLASS_VSTBASE, "idle");

    AEffGUIEditor::idle();
}

CView* VSTBaseEditor::addControl(EditorParameter::ControlType type, CControlListener *listener, CPoint position, VstInt32 tag, VstInt32 numBitmaps, VstInt32 bitmapId, CPoint handle)
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

    return 0;
}

void VSTBaseEditor::setControlValue(EditorParameter::ControlType type, VstInt32 tag, double value)
{
    switch (type)
    {
        case EditorParameter::MovieBitmap:
            if (mMovieBitmap.count(tag) != 0)
            {
                mMovieBitmap[tag]->setValue(value);
            }
            break;
    }
}

void VSTBaseEditor::attachToPage(VstInt32 pageIndex, CView* control)
{
    if (pageIndex < mEditorPage.size())
        mEditorPage[pageIndex]->addView(control);
}

CBitmap* VSTBaseEditor::getBitmap(const VstInt32 bitmapId)
{
    return bitmapManager.getBitmap(bitmapId);
}
