#pragma once

#include "CoreMinimal.h"
#include "Settings/YT_SettingWidgetBase.h"
#include "YT_SettingWidget_Checkbox.generated.h"

class UCheckBox;

/**
 * Checkbox widget for boolean settings.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_SettingWidget_Checkbox : public UYT_SettingWidgetBase
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
	UCheckBox* SettingCheckbox;

/////////////
// Methods //
/////////////
protected:
	UFUNCTION()
	void OnCheckboxStateChanged(bool bIsChecked);
};
