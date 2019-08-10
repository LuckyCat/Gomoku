// Fill out your copyright notice in the Description page of Project Settings.


#include "GomokuPawn.h"

// Sets default values
AGomokuPawn::AGomokuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);

	LivesCount = 3;
}

// Called when the game starts or when spawned
void AGomokuPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGomokuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGomokuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGomokuPawn::OnReceiveMove()
{
	auto GomokuController = GetController();
	if (GomokuController->Implements<UGomokuControllerInterface>())
	{
		IGomokuControllerInterface::Execute_OnReceiveControls(GomokuController);
	}
}

void AGomokuPawn::OnEnemyMoveComplete()
{
	auto GomokuController = GetController();
	if (GomokuController->Implements<UGomokuControllerInterface>())
	{
		IGomokuControllerInterface::Execute_OnEnemyMoveComplete(GomokuController);
	}
}