#include "ChatMessage.h"
#include "Components/TextBlock.h"

void UChatMessage::SetChatMessage(const FString& Message, EChatMessageType MessageType)
{
	if (ChatMessageTextBlock)
	{
		ChatMessageTextBlock->SetText(FText::FromString(Message));
		
        FLinearColor MessageColor;
        switch (MessageType)
        {
        case EChatMessageType::ECT_SYSTEM:
            MessageColor = FLinearColor(0.5f, 0.7f, 1.0f, 1.0f); // �Ķ���
            break;
        case EChatMessageType::ECT_WARNING:
            MessageColor = FLinearColor(1.0f, 0.9f, 0.6f, 1.0f); // �����
            break;
        case EChatMessageType::ECT_ERROR:
            MessageColor = FLinearColor(1.0f, 0.7f, 0.7f, 1.0f); // ������
            break;
        case EChatMessageType::ECT_GENERAL:
        default:
            MessageColor = FLinearColor::White; // �⺻ ���
            break;
        }

        ChatMessageTextBlock->SetColorAndOpacity(FSlateColor(MessageColor));
	}
}
