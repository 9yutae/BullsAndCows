	#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessage.generated.h"

UENUM(BlueprintType)
enum class EChatMessageType : uint8
{
	ECT_GENERAL    UMETA(DisplayName = "General"),
	ECT_SYSTEM     UMETA(DisplayName = "System"),
	ECT_WARNING    UMETA(DisplayName = "Warning"),
	ECT_ERROR      UMETA(DisplayName = "Error"),
};

UCLASS()
class BULLSANDCOWS_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMessageTextBlock;

	void SetChatMessage(const FString& Message, EChatMessageType MessageType);
};
