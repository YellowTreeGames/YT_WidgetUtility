#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "MessageQueue/YT_MessageQueueWidget.h"
#include "Components/VerticalBox.h"
#include "YT_Helpers/Public/YT_TestHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FYTMessageQueueWidgetInstantiationTest, "YT.WidgetUtility.MessageQueue.QueueWidget.Instantiation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FYTMessageQueueWidgetInstantiationTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_WIDGET(UYT_MessageQueueWidget, QueueWidget);

    // Test that manager can be assigned
    MAKE_OBJECT(UMessageQueueManager, Manager);
    QueueWidget->SetMessageQueueManager(Manager);

    // Test that container can be assigned (simulated)
    // In real usage, this would be bound via BindWidget
    TestEqual("ActiveMessageWidgets should be empty initially", QueueWidget->ActiveMessageWidgets.Num(), 0);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FYTMessageQueueWidgetHandleMessageInFocusTest, "YT.WidgetUtility.MessageQueue.QueueWidget.HandleMessageInFocus", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FYTMessageQueueWidgetHandleMessageInFocusTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_WIDGET(UYT_MessageQueueWidget, QueueWidget);

    // Create a mock panel container
    UVerticalBox* MockContainer = NewObject<UVerticalBox>(QueueWidget);
    QueueWidget->MessageContainer = MockContainer;

    // Create a test message
    FMessageQueueMessage TestMessage("Test Message");
    FGuid OriginalGuid = TestMessage.MessageId;

    // Initially should have no children and no active widgets
    TestEqual("Container should have no children initially", MockContainer->GetChildrenCount(), 0);
    TestEqual("ActiveMessageWidgets should be empty initially", QueueWidget->ActiveMessageWidgets.Num(), 0);

    // Call HandleMessageInFocus
    QueueWidget->HandleMessageInFocus(TestMessage);

    // Verify widget was created and added
    TestEqual("Container should have 1 child after HandleMessageInFocus", MockContainer->GetChildrenCount(), 1);
    TestEqual("ActiveMessageWidgets should contain 1 widget", QueueWidget->ActiveMessageWidgets.Num(), 1);

    // Verify the widget is tracked by the correct GUID
    TestTrue("ActiveMessageWidgets should contain the message GUID", QueueWidget->ActiveMessageWidgets.Contains(OriginalGuid));

    // Get the created widget and verify its properties
    UYT_MessageQueueMessageWidget** WidgetPtr = QueueWidget->ActiveMessageWidgets.Find(OriginalGuid);
    TestNotNull("Widget should exist in ActiveMessageWidgets", WidgetPtr);
    if (WidgetPtr)
    {
        UYT_MessageQueueMessageWidget* CreatedWidget = *WidgetPtr;
        TestEqual("Created widget should have correct message details", CreatedWidget->MessageDetails.Message, FString("Test Message"));
        TestEqual("Created widget should have correct GUID", CreatedWidget->MessageDetails.MessageId, OriginalGuid);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FYTMessageQueueWidgetHandleMessageOutFocusTest, "YT.WidgetUtility.MessageQueue.QueueWidget.HandleMessageOutFocus", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FYTMessageQueueWidgetHandleMessageOutFocusTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_WIDGET(UYT_MessageQueueWidget, QueueWidget);

    // Create a mock panel container
    UVerticalBox* MockContainer = NewObject<UVerticalBox>(QueueWidget);
    QueueWidget->MessageContainer = MockContainer;

    // Create and add a message widget manually to simulate it being in focus
    FMessageQueueMessage TestMessage("Test Message");
    FGuid MessageGuid = TestMessage.MessageId;

    // Create a message widget manually
    UYT_MessageQueueMessageWidget* MessageWidget = CreateWidget<UYT_MessageQueueMessageWidget>(QueueWidget, QueueWidget->MessageWidgetClass);
    MessageWidget->MessageDetails = TestMessage;

    // Add to container and tracking
    MockContainer->AddChild(MessageWidget);
    QueueWidget->ActiveMessageWidgets.Add(MessageGuid, MessageWidget);

    // Verify initial state
    TestEqual("Container should have 1 child initially", MockContainer->GetChildrenCount(), 1);
    TestEqual("ActiveMessageWidgets should contain 1 widget", QueueWidget->ActiveMessageWidgets.Num(), 1);
    TestTrue("ActiveMessageWidgets should contain the message GUID", QueueWidget->ActiveMessageWidgets.Contains(MessageGuid));

    // Call HandleMessageOutFocus
    QueueWidget->HandleMessageOutFocus(TestMessage);

    // Verify widget was removed
    TestEqual("Container should have no children after HandleMessageOutFocus", MockContainer->GetChildrenCount(), 0);
    TestEqual("ActiveMessageWidgets should be empty after HandleMessageOutFocus", QueueWidget->ActiveMessageWidgets.Num(), 0);
    TestFalse("ActiveMessageWidgets should not contain the message GUID", QueueWidget->ActiveMessageWidgets.Contains(MessageGuid));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FYTMessageQueueWidgetHandleMessageOutFocusNonExistentTest, "YT.WidgetUtility.MessageQueue.QueueWidget.HandleMessageOutFocusNonExistent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FYTMessageQueueWidgetHandleMessageOutFocusNonExistentTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_WIDGET(UYT_MessageQueueWidget, QueueWidget);

    // Create a mock panel container
    UVerticalBox* MockContainer = NewObject<UVerticalBox>(QueueWidget);
    QueueWidget->MessageContainer = MockContainer;

    // Create a message that doesn't exist in the active widgets
    FMessageQueueMessage NonExistentMessage("Non-existent Message");

    // Verify initial state
    TestEqual("Container should have no children initially", MockContainer->GetChildrenCount(), 0);
    TestEqual("ActiveMessageWidgets should be empty initially", QueueWidget->ActiveMessageWidgets.Num(), 0);

    // Call HandleMessageOutFocus with non-existent message
    QueueWidget->HandleMessageOutFocus(NonExistentMessage);

    // Verify nothing changed (graceful handling of non-existent messages)
    TestEqual("Container should still have no children", MockContainer->GetChildrenCount(), 0);
    TestEqual("ActiveMessageWidgets should still be empty", QueueWidget->ActiveMessageWidgets.Num(), 0);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS