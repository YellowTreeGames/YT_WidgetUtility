#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "YT_SettingsContainer.generated.h"

/**
 * Container widget for a group of setting widgets.
 * Walks its own widget tree and applies every UYT_SettingWidgetBase found within it.
 * Activatable so CommonUI/CommonInput route initial gamepad/keyboard focus to the first setting control.
 */
UCLASS(Blueprintable, BlueprintType)
class YT_WIDGETUTILITY_API UYT_SettingsContainer : public UCommonActivatableWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYT_SettingsContainer(const FObjectInitializer& ObjectInitializer);

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

/////////////
// Methods //
/////////////
public:
	/** Applies every setting widget found within this container's widget tree. */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplySettings();
};
