#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageQueueTypes.h"
#include "YT_MessageQueueWidget.generated.h"

class UYT_MessageQueueMessageWidget;
class UMessageQueueManager;

UCLASS()
class YT_WIDGETUTILITY_API UYT_MessageQueueWidget : public UUserWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYT_MessageQueueWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

/////////////
// Methods //
/////////////
public:
	/** Handle when a message comes into focus */
	UFUNCTION()
	void HandleMessageInFocus(const FMessageQueueMessage& Message);

	/** Handle when a message goes out of focus */
	UFUNCTION()
	void HandleMessageOutFocus(const FMessageQueueMessage& Message);

	/** Bind events to the message queue manager */
	UFUNCTION()
	void BindManagerEvents();

	/** Set the message queue manager and automatically bind events */
	UFUNCTION(BlueprintCallable, Category = "Message Queue")
	void SetMessageQueueManager(UMessageQueueManager* NewManager);

////////////////
// Components //
////////////////
public:
	/** Container panel for message widgets */
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* MessageContainer;

////////////////
// Attributes //
////////////////
public:
	/** The widget class to use for individual messages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Queue")
	TSubclassOf<UYT_MessageQueueMessageWidget> MessageWidgetClass;

	/** The message queue manager to bind events to */
	UPROPERTY(BlueprintReadWrite, Category = "Message Queue")
	UMessageQueueManager* MessageQueueManager;

	/** Map of active message widgets keyed by message ID */
	UPROPERTY(VisibleAnywhere, Category = "Message Queue")
	TMap<FGuid, UYT_MessageQueueMessageWidget*> ActiveMessageWidgets;
};
