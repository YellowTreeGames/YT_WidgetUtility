#pragma once

#include "CoreMinimal.h"
#include "MessageQueueTypes.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FMessageQueueMessage
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	/** Default constructor */
	FMessageQueueMessage();

	/** Constructor with message text */
	FMessageQueueMessage(const FString& InMessage);

////////////////
// Attributes //
////////////////
public:
	/** Unique identifier for this message */
	UPROPERTY(BlueprintReadOnly, Category = "Message Queue")
	FGuid MessageId;

	/** The message to display */
	UPROPERTY(BlueprintReadWrite, Category = "Message Queue")
	FString Message;

/////////////
// Methods //
/////////////
public:
	/** Equality operator */
	bool operator==(const FMessageQueueMessage& Other) const
	{
		return MessageId == Other.MessageId;
	}

	/** Inequality operator */
	bool operator!=(const FMessageQueueMessage& Other) const
	{
		return !(*this == Other);
	}

	/** Hash function for use in containers */
	friend uint32 GetTypeHash(const FMessageQueueMessage& Message)
	{
		return GetTypeHash(Message.MessageId);
	}
};
