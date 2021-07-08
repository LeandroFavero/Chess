// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CGChessPlayerController.generated.h"

class ACGPiece;
class ACGBoardTile;


UCLASS()
class CHESS_API ACGChessPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACGChessPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Chess setup")
	int SelectedSkinId{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	int PreferredSide;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerMoveToTile(ACGPiece* pPiece, ACGBoardTile* pTile);
	void ServerMoveToTile_Implementation(ACGPiece* pPiece, ACGBoardTile* pTile);

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "Chess")
	void ServerUpdateGrab(ACGPiece* pPiece, FVector_NetQuantize Location);
	void ServerUpdateGrab_Implementation(ACGPiece* pPiece, FVector_NetQuantize pLocation);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerGrab(ACGPiece* pPiece, bool pGrab);
	void ServerGrab_Implementation(ACGPiece* pPiece, bool pGrab);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerUndoTo(int pMoveNum);
	void ServerUndoTo_Implementation(int pMoveNum);

	virtual void BeginPlayingState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
