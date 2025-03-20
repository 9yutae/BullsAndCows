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

ABACGameMode::ABACGameMode()
{
	PlayerControllerClass = ABACPlayerController::StaticClass();
	HUDClass = ABACGameHUD::StaticClass();
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

void ABACGameMode::ResetGame()
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();
	if (!CurrentGameState)
	{
		return;
	}

	CurrentGameState->InitializeGame();
	CurrentGameState->SelectFirstTurnPlayer();

	BroadCastMessage(FString::Printf(TEXT("Game Start!\n%s's Turn."), *CurrentGameState->CurrentTurnPlayer));
	StartTurnTimer();
}

void ABACGameMode::StartTurnTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		TurnTimerHandle,
		this,
		&ABACGameMode::HandleTurnTimeout,
		10,
		false
	);
}

void ABACGameMode::HandleTurnTimeout()
{
	EndTurn(true);
}

void ABACGameMode::EndTurn(bool bIsTimeout)
{
	ABACGameState* CurrentGameState = GetGameState<ABACGameState>();
	if (!CurrentGameState)
	{
		return;
	}

	if (bIsTimeout)
	{
		BroadCastMessage(FString::Printf(TEXT("%s's turn timed out!"), *CurrentGameState->CurrentTurnPlayer));
	}

	CurrentGameState->SetNextTurn();
	BroadCastMessage(FString::Printf(TEXT("Now it's %s's turn!"), *CurrentGameState->CurrentTurnPlayer));

	StartTurnTimer();
}

void ABACGameMode::GotMessageFromClient(const FString& PlayerName, const FString& Msg)
{
	if (Msg.StartsWith(TEXT("/")))
	{
		ABACGameState* CurrentGameState = GetGameState<ABACGameState>();
		if (!CurrentGameState)
		{
			return;
		}

		if (CurrentGameState->CurrentTurnPlayer == PlayerName)
		{
			ProcessChatMessage(PlayerName, Msg);
		}
		else
		{
			BroadCastMessage(FString::Printf(TEXT("%s, It's %s's turn!"), *PlayerName, *CurrentGameState->CurrentTurnPlayer));
			return;
		}
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
		
		if (CheckGameEndCondition())
		{
			ResetGame();
			return;
		}

		EndTurn(false);
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
		return;
	}

	EndTurn(false);
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
