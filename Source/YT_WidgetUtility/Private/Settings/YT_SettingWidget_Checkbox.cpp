#include "Settings/YT_SettingWidget_Checkbox.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/CheckBox.h"
#include "GameUserSettings/Definitions/YT_SettingDef_Bool.h"

void UYT_SettingWidget_Checkbox::NativeConstruct()
{
	Super::NativeConstruct();

	if (UYT_SettingDef_Bool* BoolDef = Cast<UYT_SettingDef_Bool>(SettingDefinition))
	{
		if (SettingCheckbox)
		{
			SettingCheckbox->SetIsChecked(BoolDef->GetPendingValue());
			SettingCheckbox->OnCheckStateChanged.RemoveDynamic(this, &UYT_SettingWidget_Checkbox::OnCheckboxStateChanged);
			SettingCheckbox->OnCheckStateChanged.AddDynamic(this, &UYT_SettingWidget_Checkbox::OnCheckboxStateChanged);
		}
	}
}

void UYT_SettingWidget_Checkbox::NativeDestruct()
{
	if (SettingCheckbox)
	{
		SettingCheckbox->OnCheckStateChanged.RemoveDynamic(this, &UYT_SettingWidget_Checkbox::OnCheckboxStateChanged);
	}
	Super::NativeDestruct();
}

void UYT_SettingWidget_Checkbox::UpdateWidgetVisuals_Implementation()
{
	if (UYT_SettingDef_Bool* BoolDef = Cast<UYT_SettingDef_Bool>(SettingDefinition))
	{
		if (SettingCheckbox)
		{
			SettingCheckbox->SetIsChecked(BoolDef->GetPendingValue());
		}
	}
}

void UYT_SettingWidget_Checkbox::OnCheckboxStateChanged(bool bIsChecked)
{
	if (UYT_SettingDef_Bool* BoolDef = Cast<UYT_SettingDef_Bool>(SettingDefinition))
	{
		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("%s checkbox changed to %s."), *GetName(), bIsChecked ? TEXT("true") : TEXT("false"));
		BoolDef->SetPendingValue(bIsChecked);
	}
}

UWidget* UYT_SettingWidget_Checkbox::GetFocusTarget() const
{
	return SettingCheckbox;
}
