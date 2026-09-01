#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "YT_SettingsScreen.generated.h"

class UYT_ButtonBase;

/**
 * Top-level settings screen hosting any number of UYT_SettingsContainer tabs found in its own
 * widget tree. Save applies every container it finds then saves once, so adding a tab means
 * dropping another UYT_SettingsContainer into the tree - no new properties on this class.
 * Activatable so CommonUI/CommonInput route gamepad focus and back-action to it; bIsBackHandler
 * defaults on so a gamepad Back action closes the screen the same way the Back button does.
 */
UCLASS(Blueprintable, BlueprintType)
class YT_WIDGETUTILITY_API UYT_SettingsScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYT_SettingsScreen(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool NativeOnHandleBackAction() override;
	virtual void NativeConstruct() override;

//////////////
// Settings //
//////////////
public:
	/** Event triggered when the Back button (or a gamepad Back action) requests this screen be closed. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsClosed);

	/** BlueprintAssignable event for Settings Closed */
	UPROPERTY(BlueprintAssignable, Category = "Settings|Events")
	FOnSettingsClosed OnSettingsClosed;

	/** Applies every UYT_SettingsContainer found in this screen's widget tree, then saves once. Bind to the Save button. */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void RequestSave();

	/** Fires OnSettingsClosed. Bind to the Back button. */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void RequestClose();

protected:
	/** Optional Apply button; if bound, clicking it calls RequestSave. */
	UPROPERTY(BlueprintReadOnly, Category = "Settings", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> ApplyButton;

	/** Optional Back button; if bound, clicking it calls RequestClose. */
	UPROPERTY(BlueprintReadOnly, Category = "Settings", meta = (BindWidgetOptional))
	TObjectPtr<UYT_ButtonBase> BackButton;

private:
	void HandleApplyButtonClicked();
	void HandleBackButtonClicked();
};
