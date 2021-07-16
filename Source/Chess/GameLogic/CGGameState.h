// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGUndo.h"
#include "GameFramework/GameState.h"
#include "CGGameState.generated.h"

class ACGChessBoard;

UENUM(BlueprintType)
enum EGameResult
{
	BLACK_WINS = 0,
	WHITE_WINS = 1,
	DRAW = 2,
	DISCONNECT = 3,
};

UCLASS()
class CHESS_API ACGGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	ACGChessBoard* Board;

	UFUNCTION(NetMulticast, Reliable)
	void ClientGameFinished(const EGameResult pResult);
	void ClientGameFinished_Implementation(const EGameResult pResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
