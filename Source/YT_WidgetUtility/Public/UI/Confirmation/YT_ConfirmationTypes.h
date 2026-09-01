#pragma once

#include "CoreMinimal.h"
#include "YT_ConfirmationTypes.generated.h"

/** A single selectable option in a confirmation dialog. */
USTRUCT(BlueprintType)
struct YT_WIDGETUTILITY_API FYT_ConfirmationOption
{
    GENERATED_BODY()

    /** Display label shown on the button. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    FText Label;

    /** Identifier returned to the receiver when this option is chosen. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    FName OptionId;
};

/** Configuration passed to a confirmation dialog at show time. */
USTRUCT(BlueprintType)
struct YT_WIDGETUTILITY_API FYT_ConfirmationConfig
{
    GENERATED_BODY()

    /** Body text describing the choice being made. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    FText Body;

    /** Token echoed back in ReceiveConfirmationResult so the receiver can identify which dialog responded. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    FName ContextToken;

    /** Options presented as buttons. Display order matches array order. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    TArray<FYT_ConfirmationOption> Options;

    /** Title text displayed at the top of the dialog. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YT Helpers|Confirmation")
    FText Title;
};
