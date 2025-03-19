	#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessage.generated.h"

UCLASS()
class BULLSANDCOWS_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMessageTextBlock;

	void SetChatMessage(const FString& Message);
};
