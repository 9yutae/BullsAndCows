#include "ChatMessage.h"
#include "Components/TextBlock.h"

void UChatMessage::SetChatMessage(const FString& Message)
{
	if (ChatMessageTextBlock)
	{
		ChatMessageTextBlock->SetText(FText::FromString(Message));
		ChatMessageTextBlock->Font.Size = 16;
	}
}
