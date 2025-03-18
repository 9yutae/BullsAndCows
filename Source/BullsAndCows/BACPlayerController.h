#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BACPlayerController.generated.h"

UCLASS()
class BULLSANDCOWS_API ABACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void SendMessageToServer(const FString& Msg);

	UFUNCTION(Client, Reliable)
	void GotBroadCast(const FString& Msg);

};
