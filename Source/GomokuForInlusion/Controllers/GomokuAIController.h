// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GomokuControllerInterface.h"
#include "GomokuTypes.h"

#include "GomokuAIController.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class GOMOKUFORINLUSION_API AGomokuAIController : public AAIController, public IGomokuControllerInterface
{
	GENERATED_BODY()

public:

	AGomokuAIController();

	UPROPERTY(BlueprintReadOnly)
		bool bMoveStart;

	virtual void OnReceiveControls_Implementation() override;
	virtual void OnEnemyMoveComplete_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MaxThinkingTime;

	UPROPERTY(BlueprintReadOnly)
		float ThinkingTime;

	UFUNCTION(BlueprintCallable, Category = "Pawn")
		AGomokuPawn* GetGomokuPawn();

	UFUNCTION(BlueprintCallable, Category = "Moves")
		FTokensSequence GetDummyMoves();

	float Evaluate(int Deep, ECell Player, bool Max);

	void Collect(int X, int Y, ECell & Current, int & Count);

	float GetHeuristicScore(FIntPoint Point);

	UFUNCTION(BlueprintCallable, Category = "Moves")
		FTokensSequence GetSmartMoves(int Deep, ECell Player);

	UFUNCTION(BlueprintCallable, Category = "Moves")
		void ApplyRandomMove(FTokensSequence Moves);

	
};
