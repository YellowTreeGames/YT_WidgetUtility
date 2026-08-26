#include "MessageQueue/MessageQueueTypes.h"

FMessageQueueMessage::FMessageQueueMessage()
	: MessageId(FGuid::NewGuid())
	, Message("")
{
}

FMessageQueueMessage::FMessageQueueMessage(const FString& InMessage)
	: MessageId(FGuid::NewGuid())
	, Message(InMessage)
{
}
