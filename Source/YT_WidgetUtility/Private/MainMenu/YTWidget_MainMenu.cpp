#include "MainMenu/YTWidget_MainMenu.h"

UYTWidget_MainMenu::UYTWidget_MainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UYTWidget_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UYTWidget_MainMenu::RequestExit()
{
	OnExitRequested.Broadcast();
}
