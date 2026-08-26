#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MessageQueueTypes.h"
#include "MessageQueueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageFocusEvent, const FMessageQueueMessage&, Message);

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class YT_WIDGETUTILITY_API UMessageQueueManager : public UObject
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	/** Constructor */
	UMessageQueueManager();

/////////////
// Methods //
/////////////
public:
	/** Create a new message with auto-generated GUID */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Message Queue")
	static FMessageQueueMessage CreateMessage(const FString& MessageText);

	/** Add a message to the queue, immediately processes it if possible */
	UFUNCTION(BlueprintCallable, Category = "Message Queue")
	void AddMessage(const FMessageQueueMessage& Message);

	/** Process the message queue, adding the next message to focus if possible */
	UFUNCTION(BlueprintCallable, Category = "Message Queue")
	void ProcessQueue();

	/** Handle message focus timeout, removing the message from focus and processing the next message if possible */
	UFUNCTION()
	void OnMessageFocusTimeout(FMessageQueueMessage Message);

///////////////
// Accessors //
///////////////
public:
	/** Returns the messages currently in focus */
	UFUNCTION(BlueprintCallable, Category = "Message Queue")
	TArray<FMessageQueueMessage> GetInFocusMessages() const;

	/** Returns the messages in the queue */
	UFUNCTION(BlueprintCallable, Category = "Message Queue")
	TArray<FMessageQueueMessage> GetMessageQueue() const;

////////////////
// Attributes //
////////////////
public:
	/** Time in seconds a message stays in focus */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Queue")
	float MessageFocusDuration;

	/** Delay in seconds before processing next message after current goes out of focus */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Queue")
	float NextMessageDelay;

	/** Maximum number of messages that can be in focus at the same time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Queue")
	int32 MaxMessagesInFocus;

/////////////
// Runtime //
/////////////
private:
	/** Queue of pending messages */
	UPROPERTY(VisibleAnywhere, Category = "Message Queue")
	TArray<FMessageQueueMessage> MessageQueue;

	/** Messages currently in focus */
	UPROPERTY(VisibleAnywhere, Category = "Message Queue")
	TArray<FMessageQueueMessage> InFocusMessages;

	/** Timer handles for each message in focus */
	UPROPERTY(VisibleAnywhere, Category = "Message Queue")
	TMap<FMessageQueueMessage, FTimerHandle> MessageFocusTimers;

//////////////////////////
// Events and Delegates //
//////////////////////////
public:
	/** Event fired when a message comes into focus */
	UPROPERTY(BlueprintAssignable, Category = "Message Queue")
	FMessageFocusEvent OnMessageInFocus;

	/** Event fired when a message goes out of focus */
	UPROPERTY(BlueprintAssignable, Category = "Message Queue")
	FMessageFocusEvent OnMessageOutFocus;
};
