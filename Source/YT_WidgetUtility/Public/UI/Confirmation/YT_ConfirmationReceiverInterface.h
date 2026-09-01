#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "YT_ConfirmationReceiverInterface.generated.h"

/**
 * Implemented by any object (widget, controller, game mode, etc.) that requests
 * a confirmation dialog and needs to receive the result. No base class is required.
 */
UINTERFACE(Blueprintable, MinimalAPI)
class UYT_ConfirmationReceiverInterface : public UInterface
{
    GENERATED_BODY()
};

class YT_WIDGETUTILITY_API IYT_ConfirmationReceiverInterface
{
    GENERATED_BODY()

//////////////////////////
// Events and Delegates //
//////////////////////////
public:
    /** Called when the user selects an option in a confirmation dialog.
     *  @param ContextToken  Identifies which dialog this result belongs to; set by the caller in FYT_ConfirmationConfig.
     *  @param ChoiceId      The OptionId of the chosen FYT_ConfirmationOption.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YT Helpers|Confirmation")
    void ReceiveConfirmationResult(FName ContextToken, FName ChoiceId);
};
