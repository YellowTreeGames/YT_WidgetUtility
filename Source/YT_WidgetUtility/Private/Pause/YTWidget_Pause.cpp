#include "Pause/YTWidget_Pause.h"

#include "UI/Button/YT_ButtonBase.h"

UYTWidget_Pause::UYTWidget_Pause(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
}

UWidget* UYTWidget_Pause::NativeGetDesiredFocusTarget() const
{
	if (ResumeButton)
	{
		return ResumeButton;
	}

	return Super::NativeGetDesiredFocusTarget();
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
