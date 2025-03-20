#include "BACGameState.h"
#include "BACLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

//  게임 초기화
void ABACGameState::InitializeGame()
{
	// 새로운 난수 생성
	Answer = UBACLibrary::GenerateRandomNumber();
	WinnerName.Empty();

	UE_LOG(LogTemp, Warning, TEXT("New Game Started! Answer: %s"), *Answer);

	// 플레이어 데이터 초기화
	for (UPlayerGameData* Data : PlayersData)
	{
		if (Data)
		{
			Data->ConditionalBeginDestroy();
		}
	}

	PlayersData.Empty();

	// 현재 세션의 모든 플레이어 정보 추가
	for (APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		UPlayerGameData* NewPlayerData = NewObject<UPlayerGameData>(this);
		NewPlayerData->PlayerName = PlayerState->GetPlayerName();
		PlayersData.Add(NewPlayerData);

		UE_LOG(LogTemp, Warning, TEXT("Player %s added to PlayersData"), *NewPlayerData->PlayerName);
	}
}

// 플레이어 OUT 카운트 증가
void ABACGameState::RegisterOut(const FString& PlayerName)
{
	for (UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->PlayerName == PlayerName)
		{
			PlayerData->OutCount++;
			return;
		}
	}
}

// 시도 횟수 증가 및 제한
bool ABACGameState::UpdatePlayerAttempt(const FString& PlayerName)
{
	for (UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->PlayerName == PlayerName)
		{
			if (PlayerData->Attempts >= 9)
			{
				return false;
			}

			PlayerData->Attempts++;
			return true;
		}
	}
	return false;
}

// 승리자 설정
void ABACGameState::DeclareWinner(const FString& PlayerName)
{
	WinnerName = PlayerName;
}

// 게임 종료 조건
bool ABACGameState::EvaluateGameOver(FString& OutWinner)
{
	// 승자가 이미 있다면 종료
	if (!WinnerName.IsEmpty())
	{
		OutWinner = WinnerName;
		UE_LOG(LogTemp, Warning, TEXT("[BACGameState] EvaluateGameOver: Winner Name is Valid. %s"), *OutWinner);
		return true;
	}

	int32 TotalAttempts = 0;
	int32 OutCount = 0;

	for (const UPlayerGameData* PlayerData : PlayersData)
	{
		TotalAttempts += PlayerData->Attempts;
		if (PlayerData->OutCount >= 3)
		{
			OutCount++;
		}
	}

	// 모든 플레이어가 OUT이면 승자 판별
	if (OutCount == PlayersData.Num() - 1)
	{
		for (const UPlayerGameData* PlayerData : PlayersData)
		{
			OutWinner = PlayerData->PlayerName;
			UE_LOG(LogTemp, Warning, TEXT("[BACGameState] EvaluateGameOver: All Other Players are Died."), *OutWinner);
			return true;
		}
	}

	// 모든 플레이어들의 시도 횟수가 9회 이상일 시 무승부
	bool bAllPlayersReacehdMaxAttempts = true;
	for (const UPlayerGameData* PlayerData : PlayersData)
	{
		if (PlayerData->Attempts < 9)
		{
			bAllPlayersReacehdMaxAttempts = false;
			break;
		}
	}

	if (bAllPlayersReacehdMaxAttempts)
	{
		OutWinner = "Draw";
		return true;
	}

	return false;
}

void ABACGameState::SelectFirstTurnPlayer()
{
	if (PlayersData.Num() == 0)
	{
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, PlayersData.Num());
	CurrentTurnPlayer = PlayersData[RandomIndex]->PlayerName;
}

void ABACGameState::SetNextTurn()
{
	if (PlayersData.Num() == 0)
	{
		return;
	}

	int32 CurrentIndex = PlayersData.IndexOfByPredicate(
		[&](UPlayerGameData* PlayerData) {
			return PlayerData->PlayerName == CurrentTurnPlayer;
		}
	);

	int32 NextIndex = (CurrentIndex + 1) % PlayersData.Num();
	CurrentTurnPlayer = PlayersData[NextIndex]->PlayerName;
}

void ABACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABACGameState, Answer);
	DOREPLIFETIME(ABACGameState, WinnerName);
	DOREPLIFETIME(ABACGameState, CurrentTurnPlayer);
	DOREPLIFETIME(ABACGameState, PlayersData);
}

void UPlayerGameData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerGameData, PlayerName);
	DOREPLIFETIME(UPlayerGameData, OutCount);
	DOREPLIFETIME(UPlayerGameData, Attempts);
}
