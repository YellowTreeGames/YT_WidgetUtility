#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "YTWidget_MainMenu.generated.h"

class UCommonActivatableWidgetSwitcher;

/**
 * Main menu widget for YT_WidgetUtility plugin
 * Provides a basic main menu interface with exit functionality.
 * Activatable so CommonUI/CommonInput route gamepad focus and back-action to it.
 */
UCLASS(BlueprintType, Blueprintable)
class YT_WIDGETUTILITY_API UYTWidget_MainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYTWidget_MainMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	/** Optional switcher hosting a pushed sub-screen (e.g. Settings) over this menu's own buttons. Index 0 is this menu's base content. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu", meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetSwitcher> ScreenSwitcher;

////////////////////////
// Events & Delegates //
////////////////////////
public:
	/** Event triggered when exit is requested from the main menu */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitRequested);

	/** BlueprintAssignable event for Exit Requested */
	UPROPERTY(BlueprintAssignable, Category = "Main Menu|Events")
	FOnExitRequested OnExitRequested;

/////////////
// Methods //
/////////////
public:
	/** Request exit from the main menu */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void RequestExit();
};
