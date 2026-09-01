#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/Confirmation/YT_ConfirmationReceiverInterface.h"
#include "UI/Confirmation/YT_ConfirmationTypes.h"
#include "YT_ConfirmationMenuInterface.generated.h"

/**
 * Implemented by any widget that can serve as a confirmation dialog.
 * Enforces no base class — implement on any widget hierarchy.
 * UYT_ConfirmationWidget provides a ready-made base for projects without their own widget hierarchy.
 */
UINTERFACE(Blueprintable, MinimalAPI)
class UYT_ConfirmationMenuInterface : public UInterface
{
    GENERATED_BODY()
};

class YT_WIDGETUTILITY_API IYT_ConfirmationMenuInterface
{
    GENERATED_BODY()

/////////////
// Methods //
/////////////
public:
    /** Configure the dialog with display data and the object to notify on choice.
     *  Implementations should store the receiver and call ReceiveConfirmationResult
     *  when a choice is made, then remove themselves from the viewport.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YT Helpers|Confirmation")
    void SetupConfirmation(const FYT_ConfirmationConfig& Config, const TScriptInterface<IYT_ConfirmationReceiverInterface>& Receiver);
};
