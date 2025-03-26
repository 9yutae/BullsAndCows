#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BACGameState.h"
#include "ChatMessage.h"
#include "TimerManager.h"
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
	void BroadCastMessage(const FString& Msg, EChatMessageType MessageType);
	void ProcessChatMessage(const FString& PlayerName, const FString& Message);
	void HandlePlayerInput(const FString& PlayerName, const FString& Input);
	void ResetGame();

	void StartTurnTimer();
	void HandleTurnTimeout();
	void EndTurn(bool bIsTimeout);

	UPROPERTY(EditDefaultsOnly, Category = "Turn")
	int32 TurnTimeLimit = 30;

private:
	bool CheckGameEndCondition();
	void StartGame();
	bool bHasBroadcastedWaitingMessage = false;
	FTimerHandle CheckPlayersCountTimer;
	FTimerHandle TurnTimerHandle;

};
