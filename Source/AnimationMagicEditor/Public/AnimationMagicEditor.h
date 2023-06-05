// Copyright 2023 Fengzz. All Rights Reserved.




#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAnimationMagicEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/**
	* Override this to set whether your module is allowed to be unloaded on the fly
	*
	* @return	Whether the module supports shutdown separate from the rest of the engine.
	*/
	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};
