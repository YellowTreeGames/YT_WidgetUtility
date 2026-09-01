#include "UI/Confirmation/YT_ConfirmationWidget.h"
#include "YT_WidgetUtilityLogging.h"

// CORE

UYT_ConfirmationWidget::UYT_ConfirmationWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , ActiveContextToken(NAME_None)
{
}

// METHODS

void UYT_ConfirmationWidget::HandleChoice(FName ChoiceId)
{
    UE_LOG(LogYT_WidgetUtility, Log, TEXT("Confirmation choice made: Context='%s' Choice='%s'"), *ActiveContextToken.ToString(), *ChoiceId.ToString());

    if (ActiveReceiver.GetObject() && ActiveReceiver.GetObject()->Implements<UYT_ConfirmationReceiverInterface>())
    {
        IYT_ConfirmationReceiverInterface::Execute_ReceiveConfirmationResult(ActiveReceiver.GetObject(), ActiveContextToken, ChoiceId);
    }
    else
    {
        UE_LOG(LogYT_WidgetUtility, Warning, TEXT("HandleChoice: No valid receiver for context '%s'"), *ActiveContextToken.ToString());
    }

    RemoveFromParent();
}

// INTERFACES

void UYT_ConfirmationWidget::SetupConfirmation_Implementation(const FYT_ConfirmationConfig& Config, const TScriptInterface<IYT_ConfirmationReceiverInterface>& Receiver)
{
    ActiveContextToken = Config.ContextToken;
    ActiveReceiver = Receiver;

    UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("SetupConfirmation: Context='%s' Options=%d"), *Config.ContextToken.ToString(), Config.Options.Num());

    OnConfirmationSetup(Config);
}
