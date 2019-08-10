// Fill out your copyright notice in the Description page of Project Settings.


#include "GomokuAIController.h"
#include "GomokuPawn.h"


AGomokuAIController::AGomokuAIController() 
{
	MaxThinkingTime = 0.5f;
}


void AGomokuAIController::OnReceiveControls_Implementation() 
{
	auto Board = GetGomokuPawn()->Board;
	auto Moves = GetSmartMoves(3, Board->CurrentPlayer);

	//auto Moves = GetDummyMoves();
	ApplyRandomMove(Moves);
	bMoveStart = false;
}


void AGomokuAIController::OnEnemyMoveComplete_Implementation() 
{
	bMoveStart = true;
}

AGomokuPawn * AGomokuAIController::GetGomokuPawn()
{
	return Cast<AGomokuPawn>(GetPawn());
}

FTokensSequence AGomokuAIController::GetDummyMoves()
{
	auto GomokuPawn = GetGomokuPawn();
	auto Board = GomokuPawn->Board;
	return Board->GetFreeCells();
}


//Minimax Algorithm 
FTokensSequence AGomokuAIController::GetSmartMoves(int Deep, ECell Player)
{
	ThinkingTime = FPlatformTime::Seconds();
	FTokensSequence Result;
	auto Board = GetGomokuPawn()->Board;
	auto PossibleMoves = Board->GetFreeCells();
	TMap<FIntPoint, float> MovesValues;
	for (auto Move : PossibleMoves.TokensData) 
	{
		Board->SetCell(Move, Player);
		auto Win = Board->CheckWin(Move);
		if (Win == Player) 
		{
			Result.TokensData.Add(Move);
			Board->SetCell(Move, ECell::Cell_Empty);
			return Result;
		}
		else 
		{
			if (GetHeuristicScore(Move) > 0.0f)
				MovesValues.Add(Move, Evaluate(Deep - 1, Board->GetInvertPlayer(Player), false));
			else
				MovesValues.Add(Move, -1.0f);
		}
		Board->SetCell(Move, ECell::Cell_Empty);
	}
	//Find Maximum
	auto Max = -1.0f;
	for (auto Move : MovesValues) 
	{
		if (Move.Value > Max) Max = Move.Value;
	}
	//Add all Maximums to result
	for (auto Move : MovesValues)
	{
		if (Move.Value == Max) Result.TokensData.Add(Move.Key);
	}
	ThinkingTime = FPlatformTime::Seconds() - ThinkingTime;
	UE_LOG(LogTemp, Warning, TEXT("ThinkingTime: %f"), ThinkingTime);
	return Result;
}

float AGomokuAIController::Evaluate(int Deep, ECell Player, bool Max) 
{
	auto Board = GetGomokuPawn()->Board;
	auto PossibleMoves = Board->GetFreeCells();
	auto PossibleMovesCount = PossibleMoves.TokensData.Num();
	
	TMap<FIntPoint, float> MovesValues;
	for (auto Move : PossibleMoves.TokensData) 
	{
		Board->SetCell(Move, Player);
		auto Win = Board->CheckWin(Move);
		if (Win == Player)
		{
			Board->SetCell(Move, ECell::Cell_Empty);
			return Max ? 1.0f : -1.0f;
		}
		else
		{
			auto NewScore = (Max) ? GetHeuristicScore(Move) : -GetHeuristicScore(Move);
			MovesValues.Add(Move, NewScore);
		}
		Board->SetCell(Move, ECell::Cell_Empty);
	}

	if (Deep == 0 || (FPlatformTime::Seconds() - ThinkingTime) > MaxThinkingTime)
	{
		auto Result = Max ? -1.0f : 1.0f;
		for (auto Move : MovesValues) 
		{
			Result = Max ? FMath::Max(Result, Move.Value) : FMath::Min(Result, Move.Value);
			if (Max)
			{
				if (Result == 1.0f) return Result;
			}
			else {
				if (Result == -1.0f) return Result;
			}
		}
		return Result;
	}


	FTokensSequence HeuristicPossibleMoves;
	for (auto Move : MovesValues) 
	{
		if (Move.Value != 0.0f)
			HeuristicPossibleMoves.TokensData.Add(Move.Key);
	}

	auto Result = Max ? -1.0f : 1.0f;
	for (auto Move : HeuristicPossibleMoves.TokensData)
	{
		Board->SetCell(Move, Player);
		auto NewScore = Evaluate(Deep - 1, Board->GetInvertPlayer(Player), !Max);
		Result = Max ? FMath::Max(Result, NewScore) : FMath::Min(Result, NewScore);
		Board->SetCell(Move, ECell::Cell_Empty);
		if (Max) 
		{
			if (Result == 1.0f) return Result;
		}
		else {
			if (Result == -1.0f) return Result;
		}
	}
	return Result;
}



void AGomokuAIController::Collect(int X, int Y, ECell & Current, int & Count)
{
	auto Board = GetGomokuPawn()->Board;

	auto Cell = Board->GetCell(FIntPoint(X, Y));
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

float AGomokuAIController::GetHeuristicScore(FIntPoint Point) 
{
	float Score = 0.0f;
	//Check Horizontal
	auto Current = ECell::Cell_Empty;
	int Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y, Current, Count);
		if (Count == 3) Score += 0.07;
	}

	//Check Vertical
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X, Point.Y + i, Current, Count);
		if (Count == 3) Score += 0.07;
	}

	//Check Main diagonal
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y + i, Current, Count);
		if (Count == 3) Score += 0.07;
	}

	//Check Anti diagonal
	Current = ECell::Cell_Empty;
	Count = 0;
	for (int i = -4; i <= 4; i++)
	{
		Collect(Point.X + i, Point.Y - i, Current, Count);
		if (Count == 3) Score += 0.07;
	}

	return Score;

}




void AGomokuAIController::ApplyRandomMove(FTokensSequence Moves)
{
	auto GomokuPawn = GetGomokuPawn();
	auto MovesCount = Moves.TokensData.Num();
	if (MovesCount > 0)
	{
		auto RandomFreeCell = Moves.TokensData.Array()[FMath::Rand() % MovesCount];
		GomokuPawn->SimpleMove(RandomFreeCell);
	}
}
