#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "ChatMessage.h"
#include "BACPlayerController.generated.h"

UCLASS()
class BULLSANDCOWS_API ABACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void SendMessageToServer(const FString& Msg);

	UFUNCTION(Client, Reliable)
	void GotBroadCast(const FString& Msg, EChatMessageType MessageType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* OpenChatAction;

	void OpenChat();

};
