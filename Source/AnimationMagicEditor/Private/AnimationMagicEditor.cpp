// Copyright 2023 Fengzz. All Rights Reserved.




#include "AnimationMagicEditor.h"
#include "PropertyEditorModule.h"
#define LOCTEXT_NAMESPACE "FAnimationMagicEditorModule"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"

#endif

void FAnimationMagicEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAnimationMagicEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimationMagicEditorModule, AnimationMagicEditor)