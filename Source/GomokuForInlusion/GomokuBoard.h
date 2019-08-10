// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GomokuTypes.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "GomokuBoard.generated.h"


class UGomokuMove;
class AGomokuPawn;

UCLASS(Blueprintable)
class GOMOKUFORINLUSION_API AGomokuBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGomokuBoard();

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

	void ResizeBoard();

	void ChangePlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
		UInstancedStaticMeshComponent* VisualBoard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board|FirstPlayer")
		UInstancedStaticMeshComponent* FirstPlayerTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board|FirstPlayer")
		FTransform FirstPlayerTokenTransform;

	TMap<FIntPoint, int> FirstPlayerTokensMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board|SecondPlayer")
		UInstancedStaticMeshComponent* SecondPlayerTokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board|SecondPlayer")
		FTransform SecondPlayerTokenTransform;

	TMap<FIntPoint, int> SecondPlayerTokensMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board", DisplayName = "BoardInstancesInterval")
		float BoardInstancesInterval;

	UPROPERTY(Blueprintreadonly, Category = "Board")
		ECell CurrentPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		AGomokuPawn* FirstPlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		AGomokuPawn* SecondPlayerPawn;

	UPROPERTY(Blueprintreadonly, Category = "Board")
		float Coultdown;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board", DisplayName = "BoardSize")
		int BoardSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		ECell Win;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Board")
		bool bGameEnd;

	// Sequence of players moves
	TArray<UGomokuMove*> Moves;
	//TArray<UGomokuMove*> RedoMoves;

	// Current move used for adding new token and undo redo
	int CurrentMoveIndex;

	TMap<FIntPoint, ECell> BoardData;

public:	

	// Set Board Size. If zero the board will be infinite
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void SetBoardSize(int NewBoardSize);

	// Get Board Size. Get current board size 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		int GetBoardSize();

	// Returns the board as a sequence of moves
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		TMap<FIntPoint, ECell> GetData();

	// Returns the requested by move number
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		UGomokuMove* GetMove(int Number);

	// Returns the last successfull move
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		UGomokuMove* GetLastMove();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		ECell GetCell(FIntPoint Cell);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void SetCell(FIntPoint Cell, ECell Token);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void SetCellWithCheck(FIntPoint Cell, ECell Token);


	// Returns all forks for current board state
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Checks")
		TArray<FTokensSequence> GetForks();

	// Returns all for in a row for current board state
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Checks")
		TArray<FTokensSequence> GetFourInARow();

	// Returns winning player or zero if no one wins yet
	UFUNCTION(BlueprintCallable, Category = "Checks")
		ECell CheckWin(FIntPoint Point);

	void Collect(int X, int Y, ECell& Current, int& Count);

	// Returns empty if field is infinite
	UFUNCTION(BlueprintCallable, Category = "Checks")
		FTokensSequence GetFreeCells();

	UFUNCTION(BlueprintCallable, Category = "Checks")
		void AddIfEmpty(FIntPoint Cell, FTokensSequence& Sequence);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		bool ApplyMove(UGomokuMove* Move);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		bool Undo();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Clear();

	UFUNCTION(BlueprintCallable, Category = "Serialization")
		void Save(FName GameName);

	UFUNCTION(BlueprintCallable, Category = "Serialization")
		void Load(FName GameName);

	UFUNCTION(BlueprintCallable, Category = "Visual")
		void AddToken(FIntPoint Point, ECell Player);
	
	UFUNCTION(BlueprintCallable, Category = "Visual")
		void RemoveToken(FIntPoint Point, ECell Player);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn")
		AGomokuPawn* GetCurrentPawn();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn")
		AGomokuPawn * GetOppositePawn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Board")
		bool isInsideBoard(FIntPoint Cell);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
		void OnWin(ECell Player);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Board")
		FVector BoardPositionToField(FIntPoint BoardPoint);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Board")
		ECell GetInvertPlayer(ECell Player);

};
