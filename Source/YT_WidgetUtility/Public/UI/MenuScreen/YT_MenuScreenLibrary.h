#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "YT_MenuScreenLibrary.generated.h"

class UCommonActivatableWidget;
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

	/**
	 * Binds OwningWidget to re-request focus once Switcher's animated transition back to index 0
	 * actually completes. UCommonActivatableWidgetSwitcher activates/deactivates children
	 * synchronously when the active index is set, but the transition animation - and so the base
	 * content's actual place in the live widget path - only finishes asynchronously; CommonUI's
	 * own focus restoration runs on the synchronous event and silently fails to find a path to the
	 * not-yet-live widget. Call once per OwningWidget (e.g. from NativeConstruct).
	 */
	UFUNCTION(BlueprintCallable, Category = "YT Helpers|Menu Screen")
	static void BindSubScreenFocusRestoration(UCommonActivatableWidgetSwitcher* Switcher, UCommonActivatableWidget* OwningWidget);
};
