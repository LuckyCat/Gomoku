// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Controllers/GomokuAIController.h"
#include "Controllers/GomokuPlayerController.h"
#include "Controllers/GomokuControllerInterface.h"
#include "Moves/GomokuSimpleMove.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GomokuPawn.generated.h"


UCLASS(Blueprintable)
class GOMOKUFORINLUSION_API AGomokuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGomokuPawn();

	UPROPERTY(Category = "SkeletalMesh", VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* SkeletalMeshComponent;

	/** Camera SpringArm*/
	UPROPERTY(Category = "CameraVision", VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(Category = "CameraVision", VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AGomokuBoard* Board;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		int LivesCount;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		bool isActive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void OnReceiveMove();

	void OnEnemyMoveComplete();
		
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
		bool SimpleMove(FIntPoint Cell);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
		void OnWin(ECell Win);
};
