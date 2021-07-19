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
	NOT_FINISHED	UMETA(DisplayName = ""),
	BLACK_WINS		UMETA(DisplayName = "Black wins!"),
	WHITE_WINS		UMETA(DisplayName = "White wins!"),
	DRAW			UMETA(DisplayName = "Draw!"),
	DISCONNECT		UMETA(DisplayName = "")
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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	//ACGChessBoard* Board;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	TArray<TSubclassOf<class ACGPiece>> PieceTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	TEnumAsByte<EGameResult> GameResult {EGameResult::NOT_FINISHED};

	UFUNCTION()
	void ResultNotify();

	UFUNCTION()
	void ColorsChanged();

	void HandleMatchHasStarted() override;
	void HandleMatchHasEnded() override;
	void HandleMatchIsWaitingToStart() override;
	UFUNCTION(BlueprintCallable, Category= "Chess")
	bool IsMatchInProgress() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
