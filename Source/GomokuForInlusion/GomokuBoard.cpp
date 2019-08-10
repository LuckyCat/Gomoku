// Fill out your copyright notice in the Description page of Project Settings.


#include "GomokuBoard.h"
#include "Moves/GomokuMove.h"
#include "GomokuPawn.h"

// Sets default values
AGomokuBoard::AGomokuBoard()
{
	PrimaryActorTick.bCanEverTick = true;
	BoardSize = 8;
	bGameEnd = false;
	BoardInstancesInterval = 800.0f;

	CurrentPlayer = ECell::Cell_FirstPlayer;

	FirstPlayerTokenTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));
	SecondPlayerTokenTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));

	auto SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	VisualBoard = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("VisualBoard"));
	VisualBoard->SetupAttachment(RootComponent);

	FirstPlayerTokens = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FirstPlayerTokens"));
	FirstPlayerTokens->SetupAttachment(RootComponent);

	SecondPlayerTokens = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SecondPlayerTokens"));
	SecondPlayerTokens->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGomokuBoard::BeginPlay()
{
	Super::BeginPlay();
	GetCurrentPawn()->isActive = true;
	
}

void AGomokuBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Coultdown > 0.0f) 
	{
		Coultdown -= DeltaTime;
		if (Coultdown <= 0.0f) 
		{
			ChangePlayer();
		}
	}
}


void AGomokuBoard::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == "BoardSize" || PropertyChangedEvent.GetPropertyName() == "BoardInstancesInterval")
	{
		ResizeBoard();
	}
}

void AGomokuBoard::SetBoardSize(int NewBoardSize)
{
	BoardSize = NewBoardSize;
}

int AGomokuBoard::GetBoardSize()
{
	return BoardSize;
}

TMap<FIntPoint, ECell> AGomokuBoard::GetData()
{
	return BoardData;
}

UGomokuMove * AGomokuBoard::GetMove(int Number)
{
	return Moves[Number];
}

UGomokuMove * AGomokuBoard::GetLastMove()
{
	return Moves.Last();
}

ECell AGomokuBoard::GetCell(FIntPoint Cell)
{
	if (BoardData.Contains(Cell)) return BoardData[Cell];
	return ECell::Cell_Empty;
}

void AGomokuBoard::SetCell(FIntPoint Cell, ECell Token)
{
	if (BoardData.Contains(Cell))
		BoardData[Cell] = Token;
	else
		BoardData.Add(Cell, Token);
}

void AGomokuBoard::SetCellWithCheck(FIntPoint Cell, ECell Token)
{
	SetCell(Cell, Token);
	Win = CheckWin(Cell);
	if (Win != ECell::Cell_Empty)
	{
		OnWin(Win);
		GetCurrentPawn()->OnWin(Win);
		GetOppositePawn()->OnWin(Win);
		bGameEnd = true;
	}
}


bool AGomokuBoard::Undo()
{
	if (Moves.Num() < 2 || Moves.Num() % 2 > 0) return false;

	auto LastMove = Moves.Pop();
	auto SuccessUndo1 = LastMove->Undo(this);
	LastMove->ConditionalBeginDestroy();

	LastMove = Moves.Pop();
	auto SuccessUndo2 = LastMove->Undo(this);
	LastMove->ConditionalBeginDestroy();

	if (Win != ECell::Cell_Empty)
	{
		Win = ECell::Cell_Empty;
		ChangePlayer();
	}
	return SuccessUndo1 && SuccessUndo2;
}


void AGomokuBoard::Clear()
{
}

void AGomokuBoard::Save(FName GameName)
{
}

void AGomokuBoard::Load(FName GameName)
{
}


TArray<FTokensSequence> AGomokuBoard::GetForks()
{
	return TArray<FTokensSequence>();
}

TArray<FTokensSequence> AGomokuBoard::GetFourInARow()
{
	return TArray<FTokensSequence>();
}


void AGomokuBoard::Collect(int X, int Y, ECell & Current, int & Count)
{
	auto Cell = GetCell(FIntPoint(X, Y));
	if (Cell == ECell::Cell_Empty)
	{
		Current = ECell::Cell_Empty;
		Count = 0;
		return;
	}
	if (Cell == Current)
	{
		Count++;
		return;
	}	
	Current = Cell;
	Count = 1;
		
}

ECell AGomokuBoard::CheckWin(FIntPoint Point)
{
	//Check Horizontal
	auto Current = ECell::Cell_Empty;
	int Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y, Current, Count);
		if(Count == 5) return Current;
	}

	//Check Vertical
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X, Point.Y + i, Current, Count);
		if (Count == 5) return Current;
	}

	//Check Main diagonal
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y + i, Current, Count);
		if (Count == 5) return Current;
	}

	//Check Anti diagonal
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y - i, Current, Count);
		if (Count == 5) return Current;
	}

	return ECell::Cell_Empty;
}

bool AGomokuBoard::ApplyMove(UGomokuMove* Move)
{
	auto Pawn = GetCurrentPawn();
	if (Pawn->isActive) 
	{
		Pawn->isActive = false;
		Move->CurrentPlayer = CurrentPlayer;
		auto Success = Move->ApplyTo(this);
		if (Success)
		{
			Moves.Add(Move);
			Coultdown = Move->ApplicationTime;
			GetOppositePawn()->OnEnemyMoveComplete();

		}
		return Success;
	}
	return false;
}


ECell AGomokuBoard::GetInvertPlayer(ECell Player)
{
	if (Player == ECell::Cell_FirstPlayer) return ECell::Cell_SecondPlayer;
	if (Player == ECell::Cell_SecondPlayer) return ECell::Cell_FirstPlayer;
	return ECell::Cell_Empty;
}

void AGomokuBoard::ChangePlayer()
{
	if (Win != ECell::Cell_Empty) return;
	CurrentPlayer = GetInvertPlayer(CurrentPlayer);
	GetCurrentPawn()->isActive = true;
	GetCurrentPawn()->OnReceiveMove();
}


AGomokuPawn* AGomokuBoard::GetCurrentPawn()
{
	if (CurrentPlayer == ECell::Cell_FirstPlayer) return FirstPlayerPawn;
	return SecondPlayerPawn;
}


AGomokuPawn* AGomokuBoard::GetOppositePawn()
{
	if (CurrentPlayer == ECell::Cell_FirstPlayer) return SecondPlayerPawn;
	return FirstPlayerPawn;
}



bool AGomokuBoard::isInsideBoard(FIntPoint Cell) 
{
	if (BoardSize)
	{
		return (Cell.X >= 0) && (Cell.X < BoardSize) && (Cell.Y >= 0) && (Cell.Y < BoardSize);
	}
	return true; //infinite board
}

void AGomokuBoard::AddIfEmpty(FIntPoint Cell, FTokensSequence& Sequence)
{
	if (!isInsideBoard(Cell)) return;
	if (Sequence.TokensData.Contains(Cell)) return;
	if (GetCell(Cell) == ECell::Cell_Empty)	Sequence.TokensData.Add(Cell);
}

FTokensSequence AGomokuBoard::GetFreeCells()
{
	FTokensSequence FreeCells;
	if (BoardData.Num() == 0) FreeCells.TokensData.Add(FIntPoint(0, 0));
	for (auto Cell : BoardData) 
	if(Cell.Value != ECell::Cell_Empty)
	{
		AddIfEmpty(FIntPoint(Cell.Key.X +1, Cell.Key.Y), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X -1, Cell.Key.Y), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X, Cell.Key.Y +1), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X, Cell.Key.Y -1), FreeCells);

		AddIfEmpty(FIntPoint(Cell.Key.X +1, Cell.Key.Y +1), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X -1, Cell.Key.Y -1), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X +1, Cell.Key.Y -1), FreeCells);
		AddIfEmpty(FIntPoint(Cell.Key.X -1, Cell.Key.Y +1), FreeCells);
	}
	return FreeCells;
}

void AGomokuBoard::ResizeBoard()
{
	VisualBoard->ClearInstances();
	for (int i = 0; i < BoardSize; i++)
		for (int j = 0; j < BoardSize; j++)
		{
			FTransform Transform;
			Transform.SetLocation(FVector(i * BoardInstancesInterval, j * BoardInstancesInterval, 0.0f));
			Transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
			Transform.SetRotation(FRotator::ZeroRotator.Quaternion());
			VisualBoard->AddInstance(Transform);
		}
}

FVector AGomokuBoard::BoardPositionToField(FIntPoint BoardPoint)
{
	auto BaseNumber = BoardPoint.X * BoardSize + BoardPoint.Y;
	FTransform Transform;
	VisualBoard->GetInstanceTransform(BaseNumber, Transform, true);
	return Transform.GetLocation();
}

void AGomokuBoard::AddToken(FIntPoint Point, ECell Player)
{
	FTransform Transform;
	Transform.SetLocation(BoardPositionToField(Point));

	if (Player == ECell::Cell_FirstPlayer) 
	{
		Transform.SetScale3D(FirstPlayerTokenTransform.GetScale3D());
		Transform.SetRotation(FirstPlayerTokenTransform.GetRotation());
		auto InstanceNumber = FirstPlayerTokens->AddInstanceWorldSpace(Transform);
		FirstPlayerTokensMap.Add(Point, InstanceNumber);
		return;
	}
	if (Player == ECell::Cell_SecondPlayer)
	{
		Transform.SetScale3D(SecondPlayerTokenTransform.GetScale3D());
		Transform.SetRotation(SecondPlayerTokenTransform.GetRotation());
		auto InstanceNumber = SecondPlayerTokens->AddInstanceWorldSpace(Transform);
		SecondPlayerTokensMap.Add(Point, InstanceNumber);
		return;
	}
}

void AGomokuBoard::RemoveToken(FIntPoint Point, ECell Player)
{
	if (Player == ECell::Cell_FirstPlayer)
	{
		if (FirstPlayerTokensMap.Contains(Point))
		{
			int InstanceNumbers = FirstPlayerTokensMap[Point];
			FirstPlayerTokens->RemoveInstance(InstanceNumbers);
			FirstPlayerTokensMap.Remove(Point);
		}
		return;
	}
	if (Player == ECell::Cell_SecondPlayer)
	{
		if (SecondPlayerTokensMap.Contains(Point))
		{
			int InstanceNumbers = SecondPlayerTokensMap[Point];
			SecondPlayerTokens->RemoveInstance(InstanceNumbers);
			SecondPlayerTokensMap.Remove(Point);
		}
		return;
	}
}