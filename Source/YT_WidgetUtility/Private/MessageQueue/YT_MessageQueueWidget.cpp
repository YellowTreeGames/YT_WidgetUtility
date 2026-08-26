#include "MessageQueue/YT_MessageQueueWidget.h"
#include "MessageQueue/YT_MessageQueueMessageWidget.h"
#include "MessageQueue/MessageQueueManager.h"
#include "YT_WidgetUtilityLogging.h"
#include "Components/PanelWidget.h"

UYT_MessageQueueWidget::UYT_MessageQueueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MessageWidgetClass = UYT_MessageQueueMessageWidget::StaticClass();
}

void UYT_MessageQueueWidget::HandleMessageInFocus(const FMessageQueueMessage& Message)
{
	if (!MessageContainer || !MessageWidgetClass)
	{
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Warning, TEXT("MessageContainer or MessageWidgetClass is null in HandleMessageInFocus"));
		return;
	}

	UYT_MessageQueueMessageWidget* MessageWidget = CreateWidget<UYT_MessageQueueMessageWidget>(this, MessageWidgetClass);
	if (MessageWidget)
	{
		MessageWidget->MessageDetails = Message;
		MessageContainer->AddChild(MessageWidget);
		ActiveMessageWidgets.Add(Message.MessageId, MessageWidget);
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Log, TEXT("Added message widget for MessageId: %s"), *Message.MessageId.ToString());
	}
}

void UYT_MessageQueueWidget::HandleMessageOutFocus(const FMessageQueueMessage& Message)
{
	if (!MessageContainer)
	{
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Warning, TEXT("MessageContainer is null in HandleMessageOutFocus"));
		return;
	}

	UYT_MessageQueueMessageWidget** WidgetPtr = ActiveMessageWidgets.Find(Message.MessageId);
	if (WidgetPtr && *WidgetPtr)
	{
		MessageContainer->RemoveChild(*WidgetPtr);
		ActiveMessageWidgets.Remove(Message.MessageId);
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Log, TEXT("Removed message widget for MessageId: %s"), *Message.MessageId.ToString());
	}
}

void UYT_MessageQueueWidget::BindManagerEvents()
{
	if (MessageQueueManager)
	{
		MessageQueueManager->OnMessageInFocus.AddDynamic(this, &UYT_MessageQueueWidget::HandleMessageInFocus);
		MessageQueueManager->OnMessageOutFocus.AddDynamic(this, &UYT_MessageQueueWidget::HandleMessageOutFocus);
		UE_LOG(LogYT_WidgetUtility_MessageQueue, Log, TEXT("Bound to MessageQueueManager events."));
	}
}

void UYT_MessageQueueWidget::SetMessageQueueManager(UMessageQueueManager* NewManager)
{
	MessageQueueManager = NewManager;
	BindManagerEvents();
}

void UYT_MessageQueueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindManagerEvents();
}
