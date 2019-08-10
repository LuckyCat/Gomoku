// Fill out your copyright notice in the Description page of Project Settings.


#include "GomokuSimpleMove.h"


UGomokuSimpleMove::UGomokuSimpleMove() 
{
	ApplicationTime = 0.4f;
	Name = "GomokuSimpleMove";
}

bool UGomokuSimpleMove::Undo(AGomokuBoard * Board)
{
	Board->SetCell(MovePoint, ECell::Cell_Empty);
	Board->RemoveToken(MovePoint, CurrentPlayer);
	return true;
}


FString UGomokuSimpleMove::ToJson()
{
	/*
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField("X", (float)MovePoint.X);
	JsonObject->SetNumberField("Y", (float)MovePoint.Y);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
	*/
	return "";
}

void UGomokuSimpleMove::FromJson(FString Text)
{
}

bool UGomokuSimpleMove::ApplyTo(AGomokuBoard* Board) 
{
	if (Board->GetCell(MovePoint) == ECell::Cell_Empty) 
	{
		Board->SetCellWithCheck(MovePoint, CurrentPlayer);
		Board->AddToken(MovePoint, CurrentPlayer);
		return true;
	}
	return false;
}
