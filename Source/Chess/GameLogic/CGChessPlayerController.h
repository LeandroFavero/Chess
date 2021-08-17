// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameLogic/CGSide.h"
#include "CGChessPlayerController.generated.h"

class ACGPiece;
class ACGTile;
class ACGGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOverDelegate, EGameResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnPromotionDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDrawClaimableDelegate, bool, bIsClaimable);

UCLASS()
class CHESS_API ACGChessPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	TEnumAsByte<ESide> PreferredSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = SideChanged, Category = "Chess setup")
	bool bIsBlack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = DrawClaimableChanged, Category = "Chess setup")
	bool bIsDrawClaimable = false;

	ACGChessPlayerController();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerChangeSkin(const FString& Name);
	void ServerChangeSkin_Implementation(const FString& Name);
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerMoveToTile(ACGPiece* Piece, ACGTile* Tile);
	void ServerMoveToTile_Implementation(ACGPiece* Piece, ACGTile* Tile);

	UFUNCTION(BlueprintCallable, Server, Unreliable, Category = "Chess")
	void ServerUpdateGrab(ACGPiece* Piece, FVector_NetQuantize Location);
	void ServerUpdateGrab_Implementation(ACGPiece* Piece, FVector_NetQuantize Location);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerGrab(ACGPiece* Piece, bool IsGrab);
	void ServerGrab_Implementation(ACGPiece* Piece, bool IsGrab);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerUndoTo(int MoveNum);
	void ServerUndoTo_Implementation(int MoveNum);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerConcede();
	void ServerConcede_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerChoosePromotion(const FString& PieceType);
	void ServerChoosePromotion_Implementation(const FString& PieceType);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Chess")
	void ServerClaimDraw();
	void ServerClaimDraw_Implementation();

	UFUNCTION(Client, Reliable, Category = "Chess")
	void ClientBeginPromotion();
	void ClientBeginPromotion_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	void BackToMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void OnWin();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void OnLose();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
	void OnDraw();

	UPROPERTY(BlueprintAssignable, Category = "Chess")
	FOnMoveDelegate OnMove;

	UPROPERTY(BlueprintAssignable, Category = "Chess")
	FOnGameStartDelegate OnStart;

	UPROPERTY(BlueprintAssignable, Category = "Chess")
	FOnGameOverDelegate OnGameOver;

	UPROPERTY(BlueprintAssignable, Category = "Chess")
	FOnPawnPromotionDelegate OnPromotion;

	UPROPERTY(BlueprintAssignable, Category = "Chess")
	FOnDrawClaimableDelegate OnDrawClaimable;

	UFUNCTION()
	void SideChanged();

	UFUNCTION()
	void DrawClaimableChanged();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
