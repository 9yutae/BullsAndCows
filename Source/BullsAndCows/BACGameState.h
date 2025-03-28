#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BACGameState.generated.h"

UCLASS(BlueprintType)
class BULLSANDCOWS_API UPlayerGameData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 OutCount = 0;

	UPROPERTY(Replicated)
	int32 Attempts = 0;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

UCLASS()
class BULLSANDCOWS_API ABACGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	FString Answer;

	UPROPERTY(Replicated)
	FString WinnerName;

	UPROPERTY(Replicated)
	FString CurrentTurnPlayer;

	UPROPERTY(Replicated)
	TArray<UPlayerGameData*> PlayersData;

	void InitializeGame();
	void RegisterOut(const FString& PlayerName);
	bool UpdatePlayerAttempt(const FString& PlayerName);
	void DeclareWinner(const FString& PlayerName);
	bool EvaluateGameOver(FString& OutWinner);

	void SelectFirstTurnPlayer();
	void SetNextTurn();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
