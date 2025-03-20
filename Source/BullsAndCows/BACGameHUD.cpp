#include "BACGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void ABACGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ChattingClass)
	{
		ChatWidget = CreateWidget<UChatting>(GetWorld(), ChattingClass);
		if (ChatWidget)
		{
			ChatWidget->AddToViewport();
			ChatWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABACGameHUD::AddChatMessage(const FString& Message, EChatMessageType MessageType)
{
	if (!ChatWidget)
	{
		ChatWidget = CreateWidget<UChatting>(GetWorld(), ChattingClass);
		if (!ChatWidget)
		{
			return;
		}

		ChatWidget->AddToViewport();
		ChatWidget->SetVisibility(ESlateVisibility::Visible);
	}

	ChatWidget->AddChatMessage(Message, MessageType);
}
