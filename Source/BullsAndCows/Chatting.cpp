#include "Chatting.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "ChatMessage.h"
#include "GameFramework/PlayerController.h"
#include "BACPlayerController.h"

void UChatting::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatText)
	{
		ChatText->OnTextCommitted.AddDynamic(this, &UChatting::OnTextCommitted);
	}

	ChatText->SetIsEnabled(false);
}

void UChatting::ActivateChat()
{
	if (ChatText)
	{
		ChatText->SetIsEnabled(true);
		ChatText->SetFocus();
	}
}

void UChatting::AddChatMessage(const FString& Message, EChatMessageType MessageType)
{
	if (ChatScrollBox)
	{
		UChatMessage* ChatMessageWidget = CreateWidget<UChatMessage>(GetWorld(), ChatMessageClass);
		if (ChatMessageWidget)
		{
			ChatMessageWidget->SetChatMessage(Message, MessageType);
			ChatScrollBox->AddChild(ChatMessageWidget);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void UChatting::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString Message = Text.ToString().TrimStartAndEnd();
		if (!Message.IsEmpty())
		{
			if (APlayerController* PlayerController = GetOwningPlayer())
			{
				if (ABACPlayerController* BACPlayerController = Cast<ABACPlayerController>(PlayerController))
				{
					BACPlayerController->SendMessageToServer(Message);
				}
			}

			ChatText->SetText(FText::GetEmpty());
			ActivateChat();
		}
	}
}
