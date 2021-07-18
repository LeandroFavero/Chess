// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGUndo.h"
#include "GameFramework/GameState.h"
#include "CGGameState.generated.h"

class ACGChessBoard;
class UDataTable;

UENUM(BlueprintType)
enum EGameResult
{
	NOT_STARTED,
	NOT_FINISHED,
	BLACK_WINS,
	WHITE_WINS,
	DRAW,
	DISCONNECT
};

UCLASS()
class CHESS_API ACGGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	UDataTable* Skins;

	UPROPERTY(ReplicatedUsing=ColorsChanged, EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UMaterialInstance* BlackMaterial;

	UPROPERTY(ReplicatedUsing=ColorsChanged, EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UMaterialInstance* WhiteMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	ACGChessBoard* Board;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	TArray<TSubclassOf<class ACGPiece>> PieceTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	TEnumAsByte<EGameResult> GameState {EGameResult::NOT_STARTED};

	UFUNCTION(NetMulticast, Reliable)
	void ClientGameFinished(const EGameResult Result);
	void ClientGameFinished_Implementation(const EGameResult Result);

	UFUNCTION()
	void ColorsChanged();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
