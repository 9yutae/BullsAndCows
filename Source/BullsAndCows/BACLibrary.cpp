#include "BACLibrary.h"

// 세 자리의 랜덤한 숫자 생성 ( 중복 불허 )
FString UBACLibrary::GenerateRandomNumber()
{
	TArray<int32> AvailableNumbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	FString Result = TEXT("");

	for (int32 i = 0; i < 3; i++)
	{
		int32 RandIndex = FMath::RandRange(0, AvailableNumbers.Num() - 1);
		Result.Append(FString::FromInt(AvailableNumbers[RandIndex]));
		AvailableNumbers.RemoveAt(RandIndex);
	}

	return Result;
}

// 입력한 숫자 비교
FBACResult UBACLibrary::CheckInputValue(const FString& Answer, const FString& Input)
{
	// 결과값
	FBACResult Result;
	Result.Strikes = 0;
	Result.Balls = 0;
	Result.bIsOut = false;

	// 자릿수 검사
	if (Input.Len() != 3)
	{
		Result.bIsOut = true;
		return Result;
	}

	// 중복 숫자 검사
	TSet<TCHAR> UniqueDigits;
	for (TCHAR Char : Input)
	{
		if (UniqueDigits.Contains(Char))
		{
			Result.bIsOut = true;
			return Result;
		}
		UniqueDigits.Add(Char);
	}

	// 스트라이크 & 볼 판정
	for (int32 i = 0; i < 3; i++)
	{
		if (Answer[i] == Input[i])
		{
			Result.Strikes++;
		}
		else if (Answer.Contains(FString::Chr(Input[i])))
		{
			Result.Balls++;
		}
	}

	// 0S 0B일 경우 OUT
	if (Result.Strikes == 0 && Result.Balls == 0)
	{
		Result.bIsOut = true;
	}

	return Result;
}
