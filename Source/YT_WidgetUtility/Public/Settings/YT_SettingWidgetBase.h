#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUserSettings/Definitions/YT_SettingDefinition.h"
#include "YT_SettingWidgetBase.generated.h"

class UTextBlock;

/**
 * Base class for setting widgets.
 * Completely "dumb" - only knows about its assigned Data Asset and the Gameplay Tag system.
 */
UCLASS(Abstract)
class YT_WIDGETUTILITY_API UYT_SettingWidgetBase : public UUserWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void SynchronizeProperties() override;

////////////////
// Components //
////////////////
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = true))
	UTextBlock* DisplayNameText;

////////////////
// Attributes //
////////////////
public:
	/** The setting definition this widget is responsible for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (ExposeOnSpawn = true))
	UYT_SettingDefinition* SettingDefinition;

/////////////
// Methods //
/////////////
protected:
	/** Updates the display name text if the widget is present */
	void UpdateDisplayName();

	/** Updates the widget's visuals based on the current pending value in GameUserSettings */
	UFUNCTION(BlueprintNativeEvent, Category = "Setting")
	void UpdateWidgetVisuals();

	/** Called when the setting's pending value changes */
	UFUNCTION()
	void OnSettingPendingChanged(FGameplayTag SettingTag);

	/** Called when the setting is officially applied */
	UFUNCTION()
	void OnSettingChanged(FGameplayTag SettingTag);

public:
	/** Returns the interactive control this setting wants focused when the settings menu activates, or nullptr if it has none bound. */
	virtual UWidget* GetFocusTarget() const { return nullptr; }
};
