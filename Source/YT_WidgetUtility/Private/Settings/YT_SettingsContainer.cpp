#include "Settings/YT_SettingsContainer.h"
#include "YT_WidgetUtilityLogging.h"

#include "Blueprint/WidgetTree.h"
#include "Settings/YT_SettingWidgetBase.h"

UYT_SettingsContainer::UYT_SettingsContainer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWidget* UYT_SettingsContainer::NativeGetDesiredFocusTarget() const
{
	if (WidgetTree)
	{
		TArray<UWidget*> ChildWidgets;
		WidgetTree->GetAllWidgets(ChildWidgets);

		for (UWidget* Child : ChildWidgets)
		{
			if (UYT_SettingWidgetBase* SettingsWidget = Cast<UYT_SettingWidgetBase>(Child))
			{
				if (UWidget* FocusTarget = SettingsWidget->GetFocusTarget())
				{
					return FocusTarget;
				}
			}
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UYT_SettingsContainer::ApplySettings()
{
	if (WidgetTree)
	{
		TArray<UWidget*> ChildWidgets;
		WidgetTree->GetAllWidgets(ChildWidgets);

		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("UYT_SettingsContainer::ApplySettings - Scanning %d widgets in %s"), ChildWidgets.Num(), *GetName());

		int32 AppliedCount = 0;
		for (UWidget* Child : ChildWidgets)
		{
			if (UYT_SettingWidgetBase* SettingsWidget = Cast<UYT_SettingWidgetBase>(Child))
			{
				if (SettingsWidget->SettingDefinition)
				{
					UE_LOG(LogYT_WidgetUtility_Settings, Log, TEXT("UYT_SettingsContainer::ApplySettings - Applying setting for widget: %s (Tag: %s)"), *SettingsWidget->GetName(), *SettingsWidget->SettingDefinition->SettingTag.ToString());
					SettingsWidget->SettingDefinition->Apply(GetOwningPlayer());
					AppliedCount++;
				}
			}
		}

		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("UYT_SettingsContainer::ApplySettings - Finished applying %d settings in %s"), AppliedCount, *GetName());
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility_Settings, Warning, TEXT("UYT_SettingsContainer::ApplySettings - WidgetTree is null for %s"), *GetName());
	}
}
