#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Confirmation/YT_ConfirmationMenuInterface.h"
#include "UI/Confirmation/YT_ConfirmationReceiverInterface.h"
#include "UI/Confirmation/YT_ConfirmationTypes.h"
#include "YT_ConfirmationWidget.generated.h"

/**
 * Optional convenience base for confirmation dialogs. Implements IYT_ConfirmationMenuInterface
 * and manages receiver notification and self-removal after a choice is made.
 *
 * Projects that have their own base widget class should implement IYT_ConfirmationMenuInterface
 * directly on that class instead of inheriting from this one.
 */
UCLASS(Abstract)
class YT_WIDGETUTILITY_API UYT_ConfirmationWidget : public UUserWidget, public IYT_ConfirmationMenuInterface
{
    GENERATED_BODY()

//////////
// Core //
//////////
public:
    UYT_ConfirmationWidget(const FObjectInitializer& ObjectInitializer);

//////////////////////////
// Events and Delegates //
//////////////////////////
public:
    /** Called after SetupConfirmation; populate buttons from Config here. */
    UFUNCTION(BlueprintImplementableEvent, Category = "YT Helpers|Confirmation")
    void OnConfirmationSetup(const FYT_ConfirmationConfig& Config);

/////////////
// Methods //
/////////////
protected:
    /** Call when the user selects an option. Notifies the receiver and removes this widget from the viewport. */
    UFUNCTION(BlueprintCallable, Category = "YT Helpers|Confirmation")
    void HandleChoice(FName ChoiceId);

/////////////
// Runtime //
/////////////
private:
    /** Token identifying this dialog instance, echoed back to the receiver. */
    UPROPERTY()
    FName ActiveContextToken;

    /** Object notified when a choice is made. */
    UPROPERTY()
    TScriptInterface<IYT_ConfirmationReceiverInterface> ActiveReceiver;

////////////////
// Interfaces //
////////////////
public:
    virtual void SetupConfirmation_Implementation(const FYT_ConfirmationConfig& Config, const TScriptInterface<IYT_ConfirmationReceiverInterface>& Receiver) override;
};
