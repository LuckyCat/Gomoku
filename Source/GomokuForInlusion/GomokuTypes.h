// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GomokuTypes.generated.h"

UENUM(BlueprintType)
enum class ECell : uint8
{
	Cell_Empty		     UMETA(DisplayName = "Empty"),
	Cell_FirstPlayer   	 UMETA(DisplayName = "First Player"),
	Cell_SecondPlayer	 UMETA(DisplayName = "Second Player")
};

//Sequence of tokens
USTRUCT(BlueprintType)
struct FTokensSequence
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TSet<FIntPoint> TokensData;
};