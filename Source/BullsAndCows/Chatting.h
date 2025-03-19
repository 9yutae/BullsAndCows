#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlateCore.h"
#include "Chatting.generated.h"

UCLASS()
class BULLSANDCOWS_API UChatting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ChatText;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	UFUNCTION()
	void ActivateChat();

	UFUNCTION()
	void AddChatMessage(const FString& Message);

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Chat")
	TSubclassOf<class UChatMessage> ChatMessageClass;
};
