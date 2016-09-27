//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef IMAGE_ASSET_H
#include "ImageAsset.h"
#endif

#ifndef _ASSET_MANAGER_H_
#include "assets/assetManager.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

#ifndef _TAML_
#include "persistence/taml/taml.h"
#endif

#ifndef _ASSET_PTR_H_
#include "assets/assetPtr.h"
#endif

// Debug Profiling.
#include "platform/profiler.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ImageAsset);

ConsoleType(ImageAssetPtr, TypeImageAssetPtr, ImageAsset, ASSET_ID_FIELD_PREFIX)

//-----------------------------------------------------------------------------

ConsoleGetType(TypeImageAssetPtr)
{
   // Fetch asset Id.
   return (*((AssetPtr<ImageAsset>*)dptr)).getAssetId();
}

//-----------------------------------------------------------------------------

ConsoleSetType(TypeImageAssetPtr)
{
   // Was a single argument specified?
   if (argc == 1)
   {
      // Yes, so fetch field value.
      const char* pFieldValue = argv[0];

      // Fetch asset pointer.
      AssetPtr<ImageAsset>* pAssetPtr = dynamic_cast<AssetPtr<ImageAsset>*>((AssetPtrBase*)(dptr));

      // Is the asset pointer the correct type?
      if (pAssetPtr == NULL)
      {
         // No, so fail.
         //Con::warnf("(TypeTextureAssetPtr) - Failed to set asset Id '%d'.", pFieldValue);
         return;
      }

      // Set asset.
      pAssetPtr->setAssetId(pFieldValue);

      return;
   }

   // Warn.
   Con::warnf("(TypeTextureAssetPtr) - Cannot set multiple args to a single asset.");
}

//-----------------------------------------------------------------------------

ImageAsset::ImageAsset() :
   mAcquireReferenceCount(0),
   mpOwningAssetManager(NULL),
   mAssetInitialized(false)
{
   // Generate an asset definition.
   mpAssetDefinition = new AssetDefinition();

   mImageFileName = StringTable->lookup("");

   mImage = NULL;
   mUseMips = true;
   mIsHDRImage = false;
}

//-----------------------------------------------------------------------------

ImageAsset::~ImageAsset()
{
   // If the asset manager does not own the asset then we own the
   // asset definition so delete it.
   if (!getOwned())
      delete mpAssetDefinition;
}

//-----------------------------------------------------------------------------

void ImageAsset::initPersistFields()
{
   // Call parent.
   Parent::initPersistFields();

   addField("imageFile", TypeString, Offset(mImageFileName, ImageAsset), "Unique Name of the component. Defines the namespace of the scripts for the component.");
   addField("useMips", TypeBool, Offset(mUseMips, ImageAsset), "Unique Name of the component. Defines the namespace of the scripts for the component.");
   addField("isHDRImage", TypeBool, Offset(mIsHDRImage, ImageAsset), "Unique Name of the component. Defines the namespace of the scripts for the component.");
}

//------------------------------------------------------------------------------

void ImageAsset::copyTo(SimObject* object)
{
   // Call to parent.
   Parent::copyTo(object);
}

void ImageAsset::initializeAsset()
{
   SAFE_DELETE(mImage);

   if (mImageFileName)
   {
      mImage = new GBitmap();

      mImage->load(mImageFileName);

      GFXFormat form = mIsHDRImage ? GFXFormatR16G16B16A16F : GFXFormatR8G8B8A8;

      mImage->allocateBitmap(0, 0, mUseMips, form);
   }
}