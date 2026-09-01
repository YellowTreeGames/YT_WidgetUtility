#include "UI/Button/YT_ButtonBase.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/TextBlock.h"

// CORE

UYT_ButtonBase::UYT_ButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UYT_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonText);
}

// METHODS

void UYT_ButtonBase::SetButtonText(const FText& InText)
{
	ButtonText = InText;

	if (ButtonLabel)
	{
		ButtonLabel->SetText(InText);
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("SetButtonText called on %s with no bound ButtonLabel"), *GetNameSafe(this));
	}
}
