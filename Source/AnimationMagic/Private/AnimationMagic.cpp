// Copyright 2023 Fengzz. All Rights Reserved.




#include "AnimationMagic.h"

#define LOCTEXT_NAMESPACE "FAnimationMagicModule"

void FAnimationMagicModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAnimationMagicModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAnimationMagicModule, AnimationMagic)