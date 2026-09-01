#include "Settings/YT_SettingsScreen.h"
#include "YT_WidgetUtilityLogging.h"

#include "Blueprint/WidgetTree.h"
#include "GameUserSettings/YT_GameUserSettings.h"
#include "Settings/YT_SettingsContainer.h"

UYT_SettingsScreen::UYT_SettingsScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsBackHandler = true;
}

bool UYT_SettingsScreen::NativeOnHandleBackAction()
{
	RequestClose();
	return true;
}

void UYT_SettingsScreen::RequestSave()
{
	if (WidgetTree)
	{
		TArray<UWidget*> ChildWidgets;
		WidgetTree->GetAllWidgets(ChildWidgets);

		UE_LOG(LogYT_WidgetUtility_Settings, Log, TEXT("UYT_SettingsScreen::RequestSave - Scanning %d widgets in %s for setting containers"), ChildWidgets.Num(), *GetName());

		int32 AppliedCount = 0;
		for (UWidget* Child : ChildWidgets)
		{
			if (UYT_SettingsContainer* Container = Cast<UYT_SettingsContainer>(Child))
			{
				Container->ApplySettings();
				AppliedCount++;
			}
		}

		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("UYT_SettingsScreen::RequestSave - Applied %d setting containers in %s"), AppliedCount, *GetName());
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility_Settings, Warning, TEXT("UYT_SettingsScreen::RequestSave - WidgetTree is null for %s"), *GetName());
	}

	if (UYT_GameUserSettings* Settings = UYT_GameUserSettings::GetYTGameUserSettings())
	{
		Settings->SaveSettings();
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility_Settings, Warning, TEXT("UYT_SettingsScreen::RequestSave - No YT_GameUserSettings instance found"));
	}
}

void UYT_SettingsScreen::RequestClose()
{
	UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("UYT_SettingsScreen::RequestClose - Broadcasting OnSettingsClosed"));
	OnSettingsClosed.Broadcast();
}
