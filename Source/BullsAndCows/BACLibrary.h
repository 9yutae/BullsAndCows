#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BACLibrary.generated.h"

USTRUCT(BlueprintType)
struct FBACResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Strikes;

	UPROPERTY(BlueprintReadOnly)
	int32 Balls;

	UPROPERTY(BlueprintReadOnly)
	bool bIsOut;
};

UCLASS()
class BULLSANDCOWS_API UBACLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// 난수 생성 함수
	UFUNCTION(BlueprintCallable, Category = "BAC")
	static FString GenerateRandomNumber();

	// 입력 값 판정
	UFUNCTION(BlueprintCallable, Category = "BAC")
	static FBACResult CheckInputValue(const FString& Answer, const FString& Input);
};
