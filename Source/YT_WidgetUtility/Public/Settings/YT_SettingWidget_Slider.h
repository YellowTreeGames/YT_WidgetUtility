#pragma once

#include "CoreMinimal.h"
#include "Settings/YT_SettingWidgetBase.h"
#include "YT_SettingWidget_Slider.generated.h"

class USlider;
class UTextBlock;

/**
 * Slider widget for numeric settings.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_SettingWidget_Slider : public UYT_SettingWidgetBase
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
	USlider* SettingSlider;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget, OptionalWidget = true))
	UTextBlock* ValueText;

/////////////
// Methods //
/////////////
protected:
	UFUNCTION()
	void OnSliderValueChanged(float NewValue);
};
