// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic/CGUndo.h"
#include "GameFramework/GameState.h"
#include "GameLogic/CGSide.h"
#include "CGGameState.generated.h"

class ACGChessBoard;
class UDataTable;


UCLASS()
class CHESS_API ACGGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	UDataTable* Skins;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chess setup")
	UMaterialInstance* DefaultBlack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chess setup")
	UMaterialInstance* DefaultWhite;

	UPROPERTY(ReplicatedUsing=ColorsChanged, EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UMaterialInstance* BlackMaterial;

	UPROPERTY(ReplicatedUsing=ColorsChanged, EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	UMaterialInstance* WhiteMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	TArray<TSubclassOf<class ACGPiece>> PieceTemplates;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, ReplicatedUsing=ResultNotify, Category = "Chess setup")
	TEnumAsByte<EGameResult> GameResult { EGameResult::NOT_FINISHED };

	ACGGameState();

	void EndGameBecauseOfDisconnect();

	UFUNCTION()
	void ResultNotify();

	void UseSkin(const FString& iName, bool iIsBlack);

	UFUNCTION()
	void ColorsChanged();

	void HandleMatchHasStarted() override;

	void HandleMatchHasEnded() override;

	void HandleMatchIsWaitingToStart() override;

	UFUNCTION(BlueprintCallable, Category= "Chess")
	bool IsMatchInProgress() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
