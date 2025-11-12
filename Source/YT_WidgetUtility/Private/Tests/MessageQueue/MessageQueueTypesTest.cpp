#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "MessageQueue/MessageQueueTypes.h"
#include "YT_Helpers/Public/YT_TestHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueMessageInstantiationTest, "YT.WidgetUtility.MessageQueue.MessageType.Instantiation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueMessageInstantiationTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    // Test default constructor
    FMessageQueueMessage Message1;
    TestTrue("Default constructed message should have valid GUID", Message1.MessageId.IsValid());

    // Test constructor with message
    FMessageQueueMessage Message2("Test Message");
    TestTrue("Message with text should have valid GUID", Message2.MessageId.IsValid());
    TestEqual("Message should contain correct text", Message2.Message, FString("Test Message"));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMessageQueueMessageGuidTest, "YT.WidgetUtility.MessageQueue.Message.Guid", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMessageQueueMessageGuidTest::RunTest(const FString& Parameters)
{
    GET_OR_CREATE_TEST_WORLD();

    // Test default constructor creates GUID
    FMessageQueueMessage Message1;
    TestTrue("Message1 should have valid GUID", Message1.MessageId.IsValid());

    // Test constructor with message creates GUID
    FMessageQueueMessage Message2("Test Message");
    TestTrue("Message2 should have valid GUID", Message2.MessageId.IsValid());
    TestEqual("Message2 should have correct message text", Message2.Message, FString("Test Message"));

    // Test GUID uniqueness
    FMessageQueueMessage Message3("Another Message");
    TestTrue("Message3 should have different GUID from Message1", Message1.MessageId != Message3.MessageId);
    TestTrue("Message3 should have different GUID from Message2", Message2.MessageId != Message3.MessageId);

    // Test equality operators
    TestTrue("Same message should be equal to itself", Message1 == Message1);
    TestFalse("Different messages should not be equal", Message1 == Message2);
    TestTrue("Different messages should be unequal", Message1 != Message2);

    // Test hash function works
    TMap<FMessageQueueMessage, int32> MessageMap;
    MessageMap.Add(Message1, 1);
    MessageMap.Add(Message2, 2);
    MessageMap.Add(Message3, 3);

    TestEqual("MessageMap should contain 3 entries", MessageMap.Num(), 3);
    TestEqual("Message1 should map to value 1", MessageMap[Message1], 1);
    TestEqual("Message2 should map to value 2", MessageMap[Message2], 2);
    TestEqual("Message3 should map to value 3", MessageMap[Message3], 3);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
