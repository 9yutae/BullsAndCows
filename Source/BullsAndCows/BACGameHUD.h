#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Chatting.h"
#include "BACGameHUD.generated.h"

UCLASS()
class BULLSANDCOWS_API ABACGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Chat")
	TSubclassOf<UChatting> ChattingClass;

	UPROPERTY()
	UChatting* ChatWidget;

	void AddChatMessage(const FString& Message);

};
