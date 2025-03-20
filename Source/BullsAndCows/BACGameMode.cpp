#include "BACGameMode.h"
#include "BACGameState.h"
#include "BACLibrary.h"
#include "BACPlayerController.h"
#include "BACGameHUD.h"
#include "Kismet/GameplayStatics.h"


void ABACGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		CheckPlayersCountTimer,
		this,
		&ABACGameMode::StartGame,
		1.0f,
		true
	);
}

void ABACGameMode::StartGame()
{
	if (GetNumPlayers() >= 2)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckPlayersCountTimer);
		ResetGame();
	}
	else
	{
		if (!bHasBroadcastedWaitingMessage)
		{
			BroadCastMessage("Waiting for more players...");
			bHasBroadcastedWaitingMessage = true;
		}
	}
}

ABACGameMode::ABACGameMode()
{
	PlayerControllerClass = ABACPlayerController::StaticClass();
	HUDClass = ABACGameHUD::StaticClass();
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
		BroadCastMessage(FString::Printf(TEXT("%s: [OUT] Wrong Input."), *PlayerName));
		GetGameState<ABACGameState>()->RegisterOut(PlayerName);
		CheckGameEndCondition();
		return;
	}

	HandlePlayerInput(PlayerName, Input);
}

void ABACGameMode::HandlePlayerInput(const FString& PlayerName, const FString& Input)
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();
	if (!CurrentGameState)
	{
		return;
	}

	if (!CurrentGameState->UpdatePlayerAttempt(PlayerName))
	{
		BroadCastMessage(FString::Printf(TEXT("%s: Maximum attempts reached! No more guesses allowed."), *PlayerName));
		return;
	}

	FBACResult Result = UBACLibrary::CheckInputValue(CurrentGameState->Answer, Input);

	if (Result.bIsOut)
	{
		BroadCastMessage(FString::Printf(TEXT("%s: [%s] -> OUT!"), *PlayerName, *Input));
		CurrentGameState->RegisterOut(PlayerName);
	}
	else
	{
		BroadCastMessage(FString::Printf(TEXT("%s: [%s] -> %dS %dB"), *PlayerName, *Input, Result.Strikes, Result.Balls));

		// 3 Strikes
		if (Result.Strikes == 3)
		{
			CurrentGameState->DeclareWinner(PlayerName);
		}
	}

	if (CheckGameEndCondition())
	{
		ResetGame();
	}
}

bool ABACGameMode::CheckGameEndCondition()
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();

	FString Winner;
	if (CurrentGameState->EvaluateGameOver(Winner))
	{
		if (Winner == "Draw")
		{
			BroadCastMessage(TEXT("Draw! Restarting game..."));
		}
		else
		{
			BroadCastMessage(FString::Printf(TEXT("%s Wins!! Restarting game..."), *Winner));
		}

		return true;
	}

	return false;
}


void ABACGameMode::ResetGame()
{
	GetGameState<ABACGameState>()->InitializeGame();
	BroadCastMessage("Game Start!");
}
