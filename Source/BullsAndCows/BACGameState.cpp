#include "BACGameState.h"
#include "BACLibrary.h"
#include "Net/UnrealNetwork.h"

void ABACGameState::InitializeGame()
{
	Answer = UBACLibrary::GenerateRandomNumber();
	WinnerName.Empty();

	for (UPlayerGameData* Data : PlayersData)
	{
		if (Data)
		{
			Data->ConditionalBeginDestroy();
		}
	}

	PlayersData.Empty();
}

void ABACGameState::UpdatePlayerAttempt(const FString& PlayerName)
{
	for (UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->PlayerName == PlayerName)
		{
			PlayerData->Attemps++;

			if (PlayerData->Attemps >= 3)
			{
				PlayerData->bIsOut = true;
			}
			return;
		}
	}
}

bool ABACGameState::IsPlayerOut(const FString& PlayerName)
{
	for (const UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->PlayerName == PlayerName)
		{
			return PlayerData->bIsOut;
		}
	}
	return false;
}

void ABACGameState::SetPlayerOut(const FString& PlayerName)
{
	for (UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->PlayerName == PlayerName)
		{
			PlayerData->bIsOut = true;
			return;
		}
	}
}

void ABACGameState::DeclareWinner(const FString& PlayerName)
{
	WinnerName = PlayerName;
}

bool ABACGameState::EvaluateGameOver(FString& OutWinner)
{
	int32 OutCount = 0;
	int32 ActivePlayers = 0;

	for (const UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->bIsOut || PlayerData->Attemps >= 3)
		{
			OutCount++;
		}
		else
		{
			ActivePlayers++;
		}
	}

	if (!WinnerName.IsEmpty())
	{
		OutWinner = WinnerName;
		return true;
	}

	if (OutCount == PlayersData.Num() || ActivePlayers == 0)
	{
		OutWinner = "Draw";
		return true;
	}

	return false;
}

void ABACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABACGameState, Answer);
	DOREPLIFETIME(ABACGameState, WinnerName);
	DOREPLIFETIME(ABACGameState, PlayersData);
}

void UPlayerGameData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerGameData, PlayerName);
	DOREPLIFETIME(UPlayerGameData, Attemps);
	DOREPLIFETIME(UPlayerGameData, bIsOut);
}

void ABACGameState::OnRep_Answer()
{
	UE_LOG(LogTemp, Warning, TEXT("New Answer Received: %s"), *Answer);
}
