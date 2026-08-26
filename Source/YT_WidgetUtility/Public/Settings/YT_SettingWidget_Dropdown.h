#pragma once

#include "CoreMinimal.h"
#include "Settings/YT_SettingWidgetBase.h"
#include "YT_SettingWidget_Dropdown.generated.h"

class UComboBoxString;

/**
 * Dropdown widget for categorical settings.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_SettingWidget_Dropdown : public UYT_SettingWidgetBase
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
	UComboBoxString* SettingComboBox;

/////////////
// Methods //
/////////////
protected:
	UFUNCTION()
	void OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
