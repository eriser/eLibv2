/*
 * VSTBaseEditor
 */

#include <VSTBase/VSTBaseEditor.h>

using namespace eLibV2;
using namespace eLibV2::VSTBase;

VSTBaseEditor::VSTBaseEditor(AudioEffect *effect) : AEffGUIEditor(effect)
{
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
    AEffGUIEditor::idle();
}

CView* VSTBaseEditor::addControl(ControlType type, CControlListener *listener, CPoint size, CPoint offset, VstInt32 tag, VstInt32 bitmapId, CPoint handle)
{
    VstInt32 newId = 0;
    CRect tempSize;
    CPoint bitmapSize(getBitmap(bitmapId)->getWidth(), getBitmap(bitmapId)->getHeight() / 2);

    switch (type)
    {
        case kKickButton:
            newId = mKickButtons.size();
            tempSize(offset.x, offset.y, bitmapSize.x + offset.x, bitmapSize.y + offset.y);
            mKickButtons.push_back(new CKickButton(tempSize, listener, tag, CCoord(bitmapSize.y), getBitmap(bitmapId), handle));
            return mKickButtons[newId - 1];
    }

    return 0;
}

void VSTBaseEditor::attachToPage(VstInt32 pageIndex, CView* control)
{
    if (mEditorPage.size() < pageIndex)
        mEditorPage[pageIndex]->addView(control);
}

CBitmap* VSTBaseEditor::getBitmap(const VstInt32 bitmapId)
{
    return bitmapManager.getBitmap(bitmapId);
}
