#include "BACLibrary.h"

// �� �ڸ��� ������ ���� ���� ( �ߺ� ���� )
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

// �Է��� ���� ��
FBACResult UBACLibrary::CheckInputValue(const FString& Answer, const FString& Input)
{
	// �����
	FBACResult Result;
	Result.Strikes = 0;
	Result.Balls = 0;
	Result.bIsOut = false;

	// �ڸ��� �˻�
	if (Input.Len() != 3)
	{
		Result.bIsOut = true;
		return Result;
	}

	// �ߺ� ���� �˻�
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

	// ��Ʈ����ũ & �� ����
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

	// 0S 0B�� ��� OUT
	if (Result.Strikes == 0 && Result.Balls == 0)
	{
		Result.bIsOut = true;
	}

	return Result;
}
