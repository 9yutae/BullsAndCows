#include "BACGameMode.h"
#include "BACGameState.h"
#include "BACLibrary.h"
#include "BACPlayerController.h"
#include "Kismet/GameplayStatics.h"


void ABACGameMode::BeginPlay()
{
	Super::BeginPlay();
	ResetGame();
}

ABACGameMode::ABACGameMode()
{
	PlayerControllerClass = ABACPlayerController::StaticClass();
}

void ABACGameMode::GotMessageFromClient(const FString& PlayerName, const FString& Msg)
{
	if (Msg.StartsWith(TEXT("/")))
	{
		ProcessChatMessage(PlayerName, Msg);
	}
	else
	{
		BroadCastMessage(FString::Printf(TEXT("%s: %s"), *PlayerName, *Msg));
	}
}

void ABACGameMode::BroadCastMessage(const FString& Msg)
{
	TArray<AActor*> PlayerController;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABACPlayerController::StaticClass(), PlayerController);

	for (AActor* Controller : PlayerController)
	{
		if (ABACPlayerController* PC = Cast<ABACPlayerController>(Controller))
		{
			PC->GotBroadCast(Msg);
		}
	}
}


void ABACGameMode::ProcessChatMessage(const FString& PlayerName, const FString& Message)
{
	FString Input = Message.RightChop(1);
	if (Input.Len() != 3 || !Input.IsNumeric() || Input.Contains(TEXT("0")))
	{
		BroadCastMessage(FString::Printf(TEXT("%s: [OUT] 잘못된 입력입니다."), *PlayerName));
		GetGameState<ABACGameState>()->SetPlayerOut(PlayerName);
		return;
	}

	HandlePlayerInput(PlayerName, Input);
}

void ABACGameMode::HandlePlayerInput(const FString& PlayerName, const FString& Input)
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();

	if (CurrentGameState->IsPlayerOut(PlayerName))
	{
		return;
	}

	FBACResult Result = UBACLibrary::CheckInputValue(CurrentGameState->Answer, Input);
	CurrentGameState->UpdatePlayerAttempt(PlayerName);

	// 3 Strikes
	if (Result.Strikes == 3)
	{
		CurrentGameState->DeclareWinner(PlayerName);
	}

	BroadCastMessage(FString::Printf(TEXT("%s: %dS %dB"), *PlayerName, Result.Strikes, Result.Balls));

	if (CheckGameEndCondition())
	{
		ResetGame();
	}
}

void ABACGameMode::ResetGame()
{
	GetGameState<ABACGameState>()->InitializeGame();
	BroadCastMessage("Game Start!");
}

bool ABACGameMode::CheckGameEndCondition()
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();

	FString Winner;
	bool bIsGameOver = CurrentGameState->EvaluateGameOver(Winner);
	
	if (bIsGameOver)
	{
		if (Winner == "Draw")
		{
			BroadCastMessage(TEXT("Draw! Restarting game..."));
		}
		else
		{
			BroadCastMessage(FString::Printf(TEXT("%s Wins!! Restarting game..."), *Winner));
		}
	}

	return bIsGameOver;
}
