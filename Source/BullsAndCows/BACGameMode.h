#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BACGameState.h"
#include "BACGameMode.generated.h"

UCLASS()
class BULLSANDCOWS_API ABACGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	ABACGameMode();

	void GotMessageFromClient(const FString& PlayerName, const FString& Msg);
	void BroadCastMessage(const FString& Msg);
	void ProcessChatMessage(const FString& PlayerName, const FString& Message);
	void HandlePlayerInput(const FString& PlayerName, const FString& Input);
	void ResetGame();

private:
	bool CheckGameEndCondition();

};
