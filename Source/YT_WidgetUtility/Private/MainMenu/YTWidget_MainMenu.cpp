#include "MainMenu/YTWidget_MainMenu.h"

#include "Settings/YT_SettingsScreen.h"
#include "UI/Button/YT_ButtonBase.h"
#include "UI/MenuScreen/YT_MenuScreenLibrary.h"

UYTWidget_MainMenu::UYTWidget_MainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UYTWidget_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (SettingsButton)
	{
		SettingsButton->OnClicked().AddUObject(this, &UYTWidget_MainMenu::HandleSettingsButtonClicked);
	}

	if (SettingsScreen)
	{
		SettingsScreen->OnSettingsClosed.AddDynamic(this, &UYTWidget_MainMenu::HandleSettingsClosed);
	}

	UYT_MenuScreenLibrary::BindSubScreenFocusRestoration(ScreenSwitcher, this);
}

void UYTWidget_MainMenu::RequestExit()
{
	OnExitRequested.Broadcast();
}

// SETTINGS

void UYTWidget_MainMenu::HandleSettingsButtonClicked()
{
	UYT_MenuScreenLibrary::PushSubScreen(ScreenSwitcher, SettingsScreen);
}

void UYTWidget_MainMenu::HandleSettingsClosed()
{
	UYT_MenuScreenLibrary::PopSubScreen(ScreenSwitcher);
}

bool UYTWidget_MainMenu::TryCloseSettings()
{
	if (SettingsScreen && SettingsScreen->IsActivated())
	{
		SettingsScreen->RequestClose();
		return true;
	}

	return false;
}
