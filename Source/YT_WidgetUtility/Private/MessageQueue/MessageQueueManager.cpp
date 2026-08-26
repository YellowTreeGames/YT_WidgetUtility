#include "MessageQueue/MessageQueueManager.h"
#include "TimerManager.h"
#include "YT_WidgetUtilityLogging.h"
#include "Engine/World.h"

UMessageQueueManager::UMessageQueueManager()
	: MessageFocusDuration(3.0f)
	, NextMessageDelay(1.0f)
	, MaxMessagesInFocus(3)
{
}

TArray<FMessageQueueMessage> UMessageQueueManager::GetInFocusMessages() const
{
	return InFocusMessages;
}

TArray<FMessageQueueMessage> UMessageQueueManager::GetMessageQueue() const
{
	return MessageQueue;
}

FMessageQueueMessage UMessageQueueManager::CreateMessage(const FString& MessageText)
{
	return FMessageQueueMessage(MessageText);
}

void UMessageQueueManager::AddMessage(const FMessageQueueMessage& Message)
{
	MessageQueue.Add(Message);

	UE_LOG(LogYT_WidgetUtility_MessageQueue, Log, TEXT("MessageQueue: Adding message: %s"), *Message.MessageId.ToString());
	ProcessQueue();
}

void UMessageQueueManager::ProcessQueue()
{
	if (MessageQueue.Num() > 0 && InFocusMessages.Num() < MaxMessagesInFocus)
	{
		// Grab the next message
		FMessageQueueMessage NextMessage = MessageQueue[0];
		MessageQueue.RemoveAt(0);

		// Add to in-focus messages
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Verbose, TEXT("MessageQueue: Adding message to in-focus messages: %s"), *NextMessage.MessageId.ToString());
		InFocusMessages.Add(NextMessage);
		OnMessageInFocus.Broadcast(NextMessage);

		// Start the focus timer for this specific message
		FTimerHandle& TimerHandle = MessageFocusTimers.Add(NextMessage);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &UMessageQueueManager::OnMessageFocusTimeout, NextMessage),
			MessageFocusDuration,
			false
		);
	}
	else
	{
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Verbose, TEXT("MessageQueue: No messages to process"));
	}
}

void UMessageQueueManager::OnMessageFocusTimeout(FMessageQueueMessage Message)
{
	UE_LOG(LogYT_WidgetUtility_MessageQueue, Verbose, TEXT("MessageQueue: Message focus timeout: %s"), *Message.MessageId.ToString());

	// Remove the message from in-focus messages
	InFocusMessages.Remove(Message);
	MessageFocusTimers.Remove(Message);
	OnMessageOutFocus.Broadcast(Message);

	// Process the next message in queue
	ProcessQueue();
}
