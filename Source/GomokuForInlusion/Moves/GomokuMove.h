// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GomokuTypes.h"
#include "GomokuBoard.h"

#include "JsonObject.h"
#include "JsonWriter.h"
#include "JsonSerializer.h"


#include "GomokuMove.generated.h"


UCLASS(Blueprintable)
class GOMOKUFORINLUSION_API UGomokuMove : public UObject
{
	GENERATED_BODY()

public:

	UGomokuMove();
	
	UPROPERTY(EditAnywhere, Blueprintreadwrite, Category = "Gameplay")
		float ApplicationTime;

	UPROPERTY(EditAnywhere, Blueprintreadwrite, Category = "Gameplay")
		FName Name;

	UPROPERTY(EditAnywhere, Blueprintreadwrite, Category = "Gameplay")
		ECell CurrentPlayer;


	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual bool ApplyTo(AGomokuBoard* Board) { return false; };
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual bool Undo(AGomokuBoard* Board) { return false; };
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual FString ToJson() { return ""; };

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual void FromJson(FString Text) {};


};
