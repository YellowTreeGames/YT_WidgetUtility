#include "Settings/YT_SettingWidget_Dropdown.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/ComboBoxString.h"
#include "GameUserSettings/Definitions/YT_SettingDef_Categorical.h"

void UYT_SettingWidget_Dropdown::NativeConstruct()
{
	Super::NativeConstruct();

	if (UYT_SettingDef_Categorical* CategoricalDef = Cast<UYT_SettingDef_Categorical>(SettingDefinition))
	{
		if (SettingComboBox)
		{
			SettingComboBox->ClearOptions();
			for (const FText& Option : CategoricalDef->Options)
			{
				SettingComboBox->AddOption(Option.ToString());
			}

			int32 PendingIndex = CategoricalDef->GetPendingIndex();
			if (CategoricalDef->Options.IsValidIndex(PendingIndex))
			{
				SettingComboBox->SetSelectedIndex(PendingIndex);
				SettingComboBox->SetSelectedOption(CategoricalDef->Options[PendingIndex].ToString());
			}

			SettingComboBox->OnSelectionChanged.RemoveDynamic(this, &UYT_SettingWidget_Dropdown::OnSelectionChanged);
			SettingComboBox->OnSelectionChanged.AddDynamic(this, &UYT_SettingWidget_Dropdown::OnSelectionChanged);
		}
	}
}

void UYT_SettingWidget_Dropdown::NativeDestruct()
{
	if (SettingComboBox)
	{
		SettingComboBox->OnSelectionChanged.RemoveDynamic(this, &UYT_SettingWidget_Dropdown::OnSelectionChanged);
	}
	Super::NativeDestruct();
}

void UYT_SettingWidget_Dropdown::UpdateWidgetVisuals_Implementation()
{
	if (UYT_SettingDef_Categorical* CategoricalDef = Cast<UYT_SettingDef_Categorical>(SettingDefinition))
	{
		if (SettingComboBox)
		{
			int32 PendingIndex = CategoricalDef->GetPendingIndex();
			if (CategoricalDef->Options.IsValidIndex(PendingIndex))
			{
				SettingComboBox->SetSelectedIndex(PendingIndex);
				SettingComboBox->SetSelectedOption(CategoricalDef->Options[PendingIndex].ToString());
			}
		}
	}
}

void UYT_SettingWidget_Dropdown::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UYT_SettingDef_Categorical* CategoricalDef = Cast<UYT_SettingDef_Categorical>(SettingDefinition))
	{
		int32 NewIndex = SettingComboBox->GetSelectedIndex();
		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("%s dropdown selection changed to index %d (%s)."), *GetName(), NewIndex, *SelectedItem);
		CategoricalDef->SetPendingIndex(NewIndex);
	}
}
