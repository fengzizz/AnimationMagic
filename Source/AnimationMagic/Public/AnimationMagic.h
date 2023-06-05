// Copyright 2023 Fengzz. All Rights Reserved.




#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAnimationMagicModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
