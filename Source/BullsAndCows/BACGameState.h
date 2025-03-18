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
	int32 Attemps = 0;

	UPROPERTY(Replicated)
	bool bIsOut = false;

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
	TArray<UPlayerGameData*> PlayersData;

	UPROPERTY(Replicated)
	FString WinnerName;

	void InitializeGame();
	void UpdatePlayerAttempt(const FString& PlayerName);
	bool IsPlayerOut(const FString& PlayerName);
	void SetPlayerOut(const FString& PlayerName);
	void DeclareWinner(const FString& PlayerName);
	bool EvaluateGameOver(FString& OutWinner);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_Answer();
};
