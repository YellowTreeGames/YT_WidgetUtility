#include "UI/MenuScreen/YT_MenuScreenLibrary.h"
#include "YT_WidgetUtilityLogging.h"

#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"

// METHODS

void UYT_MenuScreenLibrary::PushSubScreen(UCommonActivatableWidgetSwitcher* Switcher, UUserWidget* SubScreen)
{
	if (Switcher && SubScreen)
	{
		int32 ChildIndex = Switcher->GetChildIndex(SubScreen);
		if (ChildIndex == INDEX_NONE)
		{
			Switcher->AddChild(SubScreen);
			ChildIndex = Switcher->GetChildIndex(SubScreen);
			UE_LOG(LogYT_WidgetUtility, Log, TEXT("UYT_MenuScreenLibrary::PushSubScreen - Added %s to %s at index %d"), *SubScreen->GetName(), *Switcher->GetName(), ChildIndex);
		}

		Switcher->SetActiveWidgetIndex(ChildIndex);
		UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("UYT_MenuScreenLibrary::PushSubScreen - Activated %s on %s"), *SubScreen->GetName(), *Switcher->GetName());
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility, Warning, TEXT("UYT_MenuScreenLibrary::PushSubScreen - Switcher or SubScreen is null"));
	}
}

void UYT_MenuScreenLibrary::PopSubScreen(UCommonActivatableWidgetSwitcher* Switcher)
{
	if (Switcher)
	{
		Switcher->SetActiveWidgetIndex(0);
		UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("UYT_MenuScreenLibrary::PopSubScreen - Returned %s to index 0"), *Switcher->GetName());
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility, Warning, TEXT("UYT_MenuScreenLibrary::PopSubScreen - Switcher is null"));
	}
}

void UYT_MenuScreenLibrary::BindSubScreenFocusRestoration(UCommonActivatableWidgetSwitcher* Switcher, UCommonActivatableWidget* OwningWidget)
{
	if (Switcher && OwningWidget)
	{
		TWeakObjectPtr<UCommonActivatableWidget> WeakOwningWidget(OwningWidget);
		Switcher->OnActiveWidgetIndexChanged.AddLambda([WeakOwningWidget](UWidget* NewActiveWidget, int32 Index)
		{
			if (Index == 0)
			{
				if (UCommonActivatableWidget* OwningWidget = WeakOwningWidget.Get())
				{
					UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("UYT_MenuScreenLibrary::BindSubScreenFocusRestoration - Requesting focus refresh on %s after switcher returned to index 0"), *OwningWidget->GetName());
					OwningWidget->RequestRefreshFocus();
				}
			}
		});
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility, Warning, TEXT("UYT_MenuScreenLibrary::BindSubScreenFocusRestoration - Switcher or OwningWidget is null"));
	}
}
