#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module implementation for the YT Widget Utility plugin
 */
class FYT_WidgetUtilityModule : public IModuleInterface
{
//////////
// Core //
//////////
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
