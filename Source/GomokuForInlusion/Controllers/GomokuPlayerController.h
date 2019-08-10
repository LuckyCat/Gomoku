// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GomokuControllerInterface.h"
#include "GomokuTypes.h"

#include "GomokuPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GOMOKUFORINLUSION_API AGomokuPlayerController : public APlayerController, public IGomokuControllerInterface
{
	GENERATED_BODY()

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void OnReceiveControls_Implementation() override {};
	virtual void OnEnemyMoveComplete_Implementation() override {};

};
