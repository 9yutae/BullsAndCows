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
            MessageColor = FLinearColor(0.5f, 0.7f, 1.0f, 1.0f); // 파란색
            break;
        case EChatMessageType::ECT_WARNING:
            MessageColor = FLinearColor(1.0f, 0.9f, 0.6f, 1.0f); // 노란색
            break;
        case EChatMessageType::ECT_ERROR:
            MessageColor = FLinearColor(1.0f, 0.7f, 0.7f, 1.0f); // 빨간색
            break;
        case EChatMessageType::ECT_GENERAL:
        default:
            MessageColor = FLinearColor::White; // 기본 흰색
            break;
        }

        ChatMessageTextBlock->SetColorAndOpacity(FSlateColor(MessageColor));
	}
}
