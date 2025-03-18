#include "BACPlayerController.h"
#include "BACGameMode.h"

void ABACPlayerController::SendMessageToServer_Implementation(const FString& Msg)
{
	if (HasAuthority())
	{
		if (ABACGameMode* GameMode = GetWorld()->GetAuthGameMode<ABACGameMode>())
		{
			FString PlayerName = GetHumanReadableName();
			GameMode->GotMessageFromClient(PlayerName, Msg);
		}
	}
}

bool ABACPlayerController::SendMessageToServer_Validate(const FString& Msg)
{
	return !Msg.IsEmpty();
}


void ABACPlayerController::GotBroadCast_Implementation(const FString& Msg)
{
	UE_LOG(LogTemp, Log, TEXT("% s"), *Msg);
}
