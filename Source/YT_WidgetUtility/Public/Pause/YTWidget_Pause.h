#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "YTWidget_Pause.generated.h"

class UCommonActivatableWidgetSwitcher;
class UYT_ButtonBase;
class UYT_SettingsScreen;

/**
 * Pause menu widget for YT_WidgetUtility plugin.
 * Provides resume / exit-to-menu / exit-to-desktop, all optionally bound so a console
 * project can omit the desktop-only button.
 * Activatable so CommonUI/CommonInput route gamepad focus and back-action to it; bIsBackHandler
 * defaults on so a gamepad Back action resumes play the same way the Resume button does. When
 * the Settings sub-screen is open it is the topmost back-handler and takes the action first.
 */
UCLASS(BlueprintType, Blueprintable)
class YT_WIDGETUTILITY_API UYTWidget_Pause : public UCommonActivatableWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYTWidget_Pause(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual bool NativeOnHandleBackAction() override;

///////////
// Pause //
///////////
public:
	/** Event triggered when Resume is requested from the pause menu */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeRequested);

	/** BlueprintAssignable event for Resume Requested */
	UPROPERTY(BlueprintAssignable, Category = "Pause|Events")
	FOnResumeRequested OnResumeRequested;

	/** Event triggered when Exit to Menu is requested from the pause menu */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitToMenuRequested);

	/** BlueprintAssignable event for Exit To Menu Requested */
	UPROPERTY(BlueprintAssignable, Category = "Pause|Events")
	FOnExitToMenuRequested OnExitToMenuRequested;

	/** Event triggered when Exit Desktop is requested from the pause menu */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitDesktopRequested);

	/** BlueprintAssignable event for Exit Desktop Requested */
	UPROPERTY(BlueprintAssignable, Category = "Pause|Events")
	FOnExitDesktopRequested OnExitDesktopRequested;

	/** Request resuming play from the pause menu */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void RequestResume();

	/** Request exiting to the main menu from the pause menu */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void RequestExitToMenu();

	/** Request exiting to desktop from the pause menu */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void RequestExitDesktop();

protected:
	/** Default gamepad focus target on activation. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> ResumeButton;

	/** Leaves the current session and returns to the main menu. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> ExitToMenuButton;

	/** Quits the game entirely. Bind only on platforms that allow quitting to desktop. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> ExitDesktopButton;

	/** Optional switcher hosting a pushed sub-screen (e.g. Settings) over this menu's own buttons. Index 0 is this menu's base content. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UCommonActivatableWidgetSwitcher> ScreenSwitcher;

private:
	void HandleResumeButtonClicked();
	void HandleExitToMenuButtonClicked();
	void HandleExitDesktopButtonClicked();

	//////////////
	// Settings //
	//////////////
public:
	/** Closes the Settings sub-screen if it is currently open. Returns true if it was open and closed. */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	bool TryCloseSettings();

protected:
	/** Opens the Settings sub-screen over this menu's own buttons. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> SettingsButton;

	/** Settings sub-screen instance pushed/popped on ScreenSwitcher. */
	UPROPERTY(BlueprintReadOnly, Category = "Pause", meta = (BindWidgetOptional))
	TObjectPtr<UYT_SettingsScreen> SettingsScreen;

private:
	void HandleSettingsButtonClicked();

	UFUNCTION()
	void HandleSettingsClosed();
};
