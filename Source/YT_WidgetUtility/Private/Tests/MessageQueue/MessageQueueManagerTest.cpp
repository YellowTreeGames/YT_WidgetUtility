#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "MessageQueue/MessageQueueManager.h"
#include "YT_Helpers/Public/YT_TestHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerInstantiationTest, "YT.WidgetUtility.MessageQueue.Manager.Instantiation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerInstantiationTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Test default values
    TestEqual("MessageFocusDuration should be default value", Manager->MessageFocusDuration, 3.0f);
    TestEqual("NextMessageDelay should be default value", Manager->NextMessageDelay, 1.0f);
    TestEqual("MaxMessagesInFocus should be default value", Manager->MaxMessagesInFocus, 3);

    // Test initial state
    TestEqual("MessageQueue should be empty initially", Manager->GetMessageQueue().Num(), 0);
    TestEqual("InFocusMessages should be empty initially", Manager->GetInFocusMessages().Num(), 0);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerAddMessageTest, "YT.WidgetUtility.MessageQueue.Manager.AddMessage", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerAddMessageTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Create a test message
    FMessageQueueMessage TestMessage("Test Message");

    // Add the message
    Manager->AddMessage(TestMessage);

    // Test that the message was added to the queue
    TestEqual("MessageQueue should contain 1 message", Manager->GetMessageQueue().Num(), 0); // Should be 0 since it gets processed immediately
    TestEqual("InFocusMessages should contain 1 message", Manager->GetInFocusMessages().Num(), 1);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerProcessQueueTest, "YT.WidgetUtility.MessageQueue.Manager.ProcessQueue", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerProcessQueueTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Create test messages
    FMessageQueueMessage Message1("Message 1");
    FMessageQueueMessage Message2("Message 2");

    // Add messages
    Manager->AddMessage(Message1);
    Manager->AddMessage(Message2);

    // Test state after adding messages
    TestEqual("InFocusMessages should contain messages up to MaxMessagesInFocus", Manager->GetInFocusMessages().Num(), 2);
    TestEqual("MessageQueue should be empty since all messages fit in focus", Manager->GetMessageQueue().Num(), 0);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerMaxMessagesInFocusTest, "YT.WidgetUtility.MessageQueue.Manager.MaxMessagesInFocus", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerMaxMessagesInFocusTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Set max messages in focus to 2
    Manager->MaxMessagesInFocus = 2;

    // Create 4 test messages
    FMessageQueueMessage Message1("Message 1");
    FMessageQueueMessage Message2("Message 2");
    FMessageQueueMessage Message3("Message 3");
    FMessageQueueMessage Message4("Message 4");

    // Add all messages
    Manager->AddMessage(Message1);
    Manager->AddMessage(Message2);
    Manager->AddMessage(Message3);
    Manager->AddMessage(Message4);

    // Test that only max messages are in focus
    TestEqual("InFocusMessages should contain MaxMessagesInFocus messages", Manager->GetInFocusMessages().Num(), 2);
    TestEqual("MessageQueue should contain remaining messages", Manager->GetMessageQueue().Num(), 2);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerQueueStateTest, "YT.WidgetUtility.MessageQueue.Manager.QueueState", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerQueueStateTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Test initial empty state
    TestEqual("MessageQueue should be empty initially", Manager->GetMessageQueue().Num(), 0);
    TestEqual("InFocusMessages should be empty initially", Manager->GetInFocusMessages().Num(), 0);

    // Add a message
    FMessageQueueMessage TestMessage("Test Message");
    Manager->AddMessage(TestMessage);

    // Test state after adding
    TestEqual("InFocusMessages should contain the message", Manager->GetInFocusMessages().Num(), 1);
    TestEqual("MessageQueue should be empty", Manager->GetMessageQueue().Num(), 0);

    // Test that the message content is correct
    TestEqual("InFocusMessages[0] should contain correct message", Manager->GetInFocusMessages()[0].Message, FString("Test Message"));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueManagerQueueMovementTest, "YT.WidgetUtility.MessageQueue.Manager.QueueMovement", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueManagerQueueMovementTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    MAKE_OBJECT(UMessageQueueManager, Manager);

    // Create 6 test messages
    FMessageQueueMessage Messages[6] = {
        FMessageQueueMessage("Message 1"),
        FMessageQueueMessage("Message 2"),
        FMessageQueueMessage("Message 3"),
        FMessageQueueMessage("Message 4"),
        FMessageQueueMessage("Message 5"),
        FMessageQueueMessage("Message 6")
    };

    // Add all messages to the queue
    for (int i = 0; i < 6; ++i)
    {
        Manager->AddMessage(Messages[i]);
    }

    // Initially: first 3 messages should be in focus (MaxMessagesInFocus = 3)
    TestEqual("InFocusMessages should contain 3 messages initially", Manager->GetInFocusMessages().Num(), 3);
    TestEqual("MessageQueue should contain 3 messages initially", Manager->GetMessageQueue().Num(), 3); // All fit in focus

    // Verify the first 3 messages are in focus
    TestTrue("Message 1 should be in focus", Manager->GetInFocusMessages().Contains(Messages[0]));
    TestTrue("Message 2 should be in focus", Manager->GetInFocusMessages().Contains(Messages[1]));
    TestTrue("Message 3 should be in focus", Manager->GetInFocusMessages().Contains(Messages[2]));

    // Simulate first timeout - Message 1 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[0]);

    // After first timeout: Message 1 removed, Messages 2,3,4 should be in focus
    TestEqual("InFocusMessages should contain 3 messages after first timeout", Manager->GetInFocusMessages().Num(), 3);
    TestFalse("Message 1 should not be in focus", Manager->GetInFocusMessages().Contains(Messages[0]));
    TestTrue("Message 2 should be in focus", Manager->GetInFocusMessages().Contains(Messages[1]));
    TestTrue("Message 3 should be in focus", Manager->GetInFocusMessages().Contains(Messages[2]));
    TestTrue("Message 4 should be in focus", Manager->GetInFocusMessages().Contains(Messages[3]));

    // Simulate second timeout - Message 2 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[1]);

    // After second timeout: Messages 3,4,5 should be in focus
    TestEqual("InFocusMessages should contain 3 messages after second timeout", Manager->GetInFocusMessages().Num(), 3);
    TestFalse("Message 2 should not be in focus", Manager->GetInFocusMessages().Contains(Messages[1]));
    TestTrue("Message 3 should be in focus", Manager->GetInFocusMessages().Contains(Messages[2]));
    TestTrue("Message 4 should be in focus", Manager->GetInFocusMessages().Contains(Messages[3]));
    TestTrue("Message 5 should be in focus", Manager->GetInFocusMessages().Contains(Messages[4]));

    // Simulate third timeout - Message 3 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[2]);

    // After third timeout: Messages 4,5,6 should be in focus
    TestEqual("InFocusMessages should contain 3 messages after third timeout", Manager->GetInFocusMessages().Num(), 3);
    TestFalse("Message 3 should not be in focus", Manager->GetInFocusMessages().Contains(Messages[2]));
    TestTrue("Message 4 should be in focus", Manager->GetInFocusMessages().Contains(Messages[3]));
    TestTrue("Message 5 should be in focus", Manager->GetInFocusMessages().Contains(Messages[4]));
    TestTrue("Message 6 should be in focus", Manager->GetInFocusMessages().Contains(Messages[5]));

    // Simulate fourth timeout - Message 4 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[3]);

    // After fourth timeout: Still Messages 5,6 should be in focus (no more messages in queue)
    TestEqual("InFocusMessages should contain 2 messages after fourth timeout", Manager->GetInFocusMessages().Num(), 2);        
    TestFalse("Message 4 should not be in focus", Manager->GetInFocusMessages().Contains(Messages[3]));
    TestTrue("Message 5 should be in focus", Manager->GetInFocusMessages().Contains(Messages[4]));
    TestTrue("Message 6 should be in focus", Manager->GetInFocusMessages().Contains(Messages[5]));

    // Simulate fifth timeout - Message 5 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[4]);

    // After fifth timeout: Only Message 6 should be in focus
    TestEqual("InFocusMessages should contain 1 message after fifth timeout", Manager->GetInFocusMessages().Num(), 1);
    TestFalse("Message 5 should not be in focus", Manager->GetInFocusMessages().Contains(Messages[4]));
    TestTrue("Message 6 should be in focus", Manager->GetInFocusMessages().Contains(Messages[5]));

    // Simulate sixth timeout - Message 6 should go out of focus
    Manager->OnMessageFocusTimeout(Messages[5]);

    // After sixth timeout: No messages should be in focus
    TestEqual("InFocusMessages should be empty after sixth timeout", Manager->GetInFocusMessages().Num(), 0);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
