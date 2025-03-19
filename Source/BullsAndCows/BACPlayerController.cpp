#include "BACPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BACGameMode.h"
#include "BACGameHUD.h"
#include "Chatting.h"

void ABACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}

void ABACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (OpenChatAction)
		{
			EnhancedInputComp->BindAction(
				OpenChatAction,
				ETriggerEvent::Triggered,
				this,
				&ABACPlayerController::OpenChat
			);
		}
	}
}

void ABACPlayerController::OpenChat()
{
	if (ABACGameHUD* GameHUD = Cast<ABACGameHUD>(GetHUD()))
	{
		if (!GameHUD->ChatWidget)
		{
			return;
		}

		GameHUD->ChatWidget->ActivateChat();
	}
}

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
	if (ABACGameHUD* GameHUD = Cast<ABACGameHUD>(GetHUD()))
	{
		if (GameHUD->ChatWidget)
		{
			GameHUD->AddChatMessage(Msg);
		}
	}
}
