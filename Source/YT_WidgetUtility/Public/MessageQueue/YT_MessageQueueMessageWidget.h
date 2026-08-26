#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageQueueTypes.h"
#include "YT_MessageQueueMessageWidget.generated.h"

UCLASS()
class YT_WIDGETUTILITY_API UYT_MessageQueueMessageWidget : public UUserWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	/** Constructor */
	UYT_MessageQueueMessageWidget(const FObjectInitializer& ObjectInitializer);

////////////////
// Attributes //
////////////////
public:
	/** The message details for this widget */
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Message Queue")
	FMessageQueueMessage MessageDetails;
};
