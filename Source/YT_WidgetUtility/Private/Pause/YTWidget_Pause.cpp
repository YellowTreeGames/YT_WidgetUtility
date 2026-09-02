#include "Pause/YTWidget_Pause.h"

#include "Settings/YT_SettingsScreen.h"
#include "UI/Button/YT_ButtonBase.h"
#include "UI/MenuScreen/YT_MenuScreenLibrary.h"

UYTWidget_Pause::UYTWidget_Pause(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsBackHandler = true;
}

void UYTWidget_Pause::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked().AddUObject(this, &UYTWidget_Pause::HandleResumeButtonClicked);
	}

	if (ExitToMenuButton)
	{
		ExitToMenuButton->OnClicked().AddUObject(this, &UYTWidget_Pause::HandleExitToMenuButtonClicked);
	}

	if (ExitDesktopButton)
	{
		ExitDesktopButton->OnClicked().AddUObject(this, &UYTWidget_Pause::HandleExitDesktopButtonClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked().AddUObject(this, &UYTWidget_Pause::HandleSettingsButtonClicked);
	}

	if (SettingsScreen)
	{
		SettingsScreen->OnSettingsClosed.AddDynamic(this, &UYTWidget_Pause::HandleSettingsClosed);
	}

	UYT_MenuScreenLibrary::BindSubScreenFocusRestoration(ScreenSwitcher, this);
}

UWidget* UYTWidget_Pause::NativeGetDesiredFocusTarget() const
{
	if (ResumeButton)
	{
		return ResumeButton;
	}

	return Super::NativeGetDesiredFocusTarget();
}

bool UYTWidget_Pause::NativeOnHandleBackAction()
{
	RequestResume();
	return true;
}

void UYTWidget_Pause::HandleResumeButtonClicked()
{
	RequestResume();
}

void UYTWidget_Pause::HandleExitToMenuButtonClicked()
{
	RequestExitToMenu();
}

void UYTWidget_Pause::HandleExitDesktopButtonClicked()
{
	RequestExitDesktop();
}

void UYTWidget_Pause::RequestResume()
{
	OnResumeRequested.Broadcast();
}

void UYTWidget_Pause::RequestExitToMenu()
{
	OnExitToMenuRequested.Broadcast();
}

void UYTWidget_Pause::RequestExitDesktop()
{
	OnExitDesktopRequested.Broadcast();
}

// SETTINGS

void UYTWidget_Pause::HandleSettingsButtonClicked()
{
	UYT_MenuScreenLibrary::PushSubScreen(ScreenSwitcher, SettingsScreen);
}

void UYTWidget_Pause::HandleSettingsClosed()
{
	UYT_MenuScreenLibrary::PopSubScreen(ScreenSwitcher);
}

bool UYTWidget_Pause::TryCloseSettings()
{
	if (SettingsScreen && SettingsScreen->IsActivated())
	{
		SettingsScreen->RequestClose();
		return true;
	}

	return false;
}
