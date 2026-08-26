#include "Settings/YT_SettingWidgetBase.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/TextBlock.h"
#include "GameUserSettings/YT_GameUserSettings.h"

void UYT_SettingWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (UYT_GameUserSettings* Settings = UYT_GameUserSettings::GetYTGameUserSettings())
	{
		Settings->OnSettingChanged.RemoveDynamic(this, &UYT_SettingWidgetBase::OnSettingChanged);
		Settings->OnSettingChanged.AddDynamic(this, &UYT_SettingWidgetBase::OnSettingChanged);
		Settings->OnSettingPendingChanged.RemoveDynamic(this, &UYT_SettingWidgetBase::OnSettingPendingChanged);
		Settings->OnSettingPendingChanged.AddDynamic(this, &UYT_SettingWidgetBase::OnSettingPendingChanged);
	}

	UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("%s bound to GameUserSettings change events."), *GetName());

	UpdateDisplayName();
	UpdateWidgetVisuals();
}

void UYT_SettingWidgetBase::NativeDestruct()
{
	if (UYT_GameUserSettings* Settings = UYT_GameUserSettings::GetYTGameUserSettings())
	{
		Settings->OnSettingChanged.RemoveDynamic(this, &UYT_SettingWidgetBase::OnSettingChanged);
		Settings->OnSettingPendingChanged.RemoveDynamic(this, &UYT_SettingWidgetBase::OnSettingPendingChanged);
	}
	Super::NativeDestruct();
}

void UYT_SettingWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateDisplayName();
}

void UYT_SettingWidgetBase::UpdateDisplayName()
{
	if (DisplayNameText && SettingDefinition)
	{
		DisplayNameText->SetText(SettingDefinition->DisplayName);
	}
}

void UYT_SettingWidgetBase::UpdateWidgetVisuals_Implementation()
{
	// Base implementation does nothing
}

void UYT_SettingWidgetBase::OnSettingPendingChanged(FGameplayTag SettingTag)
{
	if (SettingDefinition && SettingDefinition->SettingTag == SettingTag)
	{
		UpdateWidgetVisuals();
	}
}

void UYT_SettingWidgetBase::OnSettingChanged(FGameplayTag SettingTag)
{
	if (SettingDefinition && SettingDefinition->SettingTag == SettingTag)
	{
		UpdateWidgetVisuals();
	}
}
