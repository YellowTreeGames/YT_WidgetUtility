#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "YT_MenuScreenLibrary.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUserWidget;

/**
 * Static push/pop helpers for hosting a pluggable sub-screen (Settings, Credits, etc.) inside a
 * UCommonActivatableWidgetSwitcher owned by a menu widget. Any menu class exposes its own
 * ScreenSwitcher property and calls through here, so the push/pop mechanics live once instead of
 * per menu class. UCommonActivatableWidgetSwitcher (not the plain UWidgetSwitcher) is required so
 * the pushed/popped child's Activate()/Deactivate() and focus restoration fire automatically.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_MenuScreenLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

/////////////
// Methods //
/////////////
public:
	/** Adds SubScreen to Switcher if not already a child, then makes it the active widget. */
	UFUNCTION(BlueprintCallable, Category = "YT Helpers|Menu Screen")
	static void PushSubScreen(UCommonActivatableWidgetSwitcher* Switcher, UUserWidget* SubScreen);

	/** Returns Switcher to its base index (0), e.g. the menu's own button panel. */
	UFUNCTION(BlueprintCallable, Category = "YT Helpers|Menu Screen")
	static void PopSubScreen(UCommonActivatableWidgetSwitcher* Switcher);
};
