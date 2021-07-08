// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGUndo.h"
#include "GameFramework/GameState.h"
#include "CGGameState.generated.h"

class ACGChessBoard;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidMoveSignature, struct FCGUndo, FCGUndo);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRollbackToSignature, int, FMove);

UCLASS()
class CHESS_API ACGGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	ACGChessBoard* Board;

	//UPROPERTY(BlueprintAssignable, Replicated, Category = "Chess")
	//FOnValidMoveSignature OnValidMove;

	//UPROPERTY(BlueprintAssignable, Replicated, Category = "Chess")
	//FOnRollbackToSignature OnRollbackTo;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
