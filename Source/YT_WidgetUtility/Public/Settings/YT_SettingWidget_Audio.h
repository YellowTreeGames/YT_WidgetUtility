#pragma once

#include "CoreMinimal.h"
#include "Settings/YT_SettingWidgetBase.h"
#include "YT_SettingWidget_Audio.generated.h"

class UAudioSlider;
class UTextBlock;

/**
 * Audio slider widget for volume settings, using the specialized UAudioSlider.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_SettingWidget_Audio : public UYT_SettingWidgetBase
{
	GENERATED_BODY()

//////////
// Core //
//////////
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void UpdateWidgetVisuals_Implementation() override;

////////////////
// Components //
////////////////
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	UAudioSlider* AudioSlider;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = true))
	UTextBlock* ValueText;

/////////////
// Methods //
/////////////
protected:
	UFUNCTION()
	void OnSliderValueChanged(float NewValue);
};
