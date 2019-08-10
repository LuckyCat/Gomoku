// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Moves/GomokuMove.h"
#include "GomokuSimpleMove.generated.h"

/**
 * 
 */
UCLASS()
class GOMOKUFORINLUSION_API UGomokuSimpleMove : public UGomokuMove
{
	GENERATED_BODY()
	
public:

	UGomokuSimpleMove();

	UPROPERTY(Blueprintreadwrite, Category = "Gameplay")
		FIntPoint MovePoint;
	
	virtual bool ApplyTo(AGomokuBoard* Board) override;
	virtual bool Undo(AGomokuBoard* Board) override;
	virtual FString ToJson() override;
	virtual void FromJson(FString Text) override;
};


