// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic/CGUndo.h"
#include "ChessLogic/CGSquareCoord.h"
#include "CGPiece.generated.h"

class UCGAnimInstance;
class ACGChessPlayerController;
class ACGChessBoard;
class ACGTile;


//UENUM()
struct EPieceFlags
{
	enum EPieceFlagsValues
	{
		IsBlack = 1 << 0,			//bit0
		CanCastle = 1 << 1,			//bit1
		Captured = 1 << 2,			//bit2
		Moved = 1 << 3,				//bit3
		EnPassantCaptured = 1 << 4,	//bit4
		DummyCaptured = 1 << 5,		//bit5
	};
};

UCLASS()
class CHESS_API ACGPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACGPiece();
	ACGPiece(ACGChessBoard* pBoard, uint8 pFlags = 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	TSubclassOf<class UCGAnimInstance> AnimTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	UStaticMeshComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	ACGChessPlayerController* OwningPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = SnapToPlace, Category = "Chess setup")
	FCGSquareCoord Position;

	UPROPERTY(Replicated)
	ACGChessBoard* Board;

	UPROPERTY(Replicated)
	ACGTile* Tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	uint8 Flags {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	int Value;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostInitializeComponents() override;

public:	

	virtual void Destroyed() override;

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual const FString GetFenChars() const { return ""; }

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual const FString GetUnicode() const { return ""; }

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual const bool IsValidForPromotion() const { return false; }

	virtual void RefreshMaterial();

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetColor(bool isWhite);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void ServerGrab(bool isGrabbed);

	UFUNCTION()
	virtual void SnapToPlace();
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Chess")
	virtual void ClientSnapToPlace();
	virtual void ClientSnapToPlace_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveToTile(ACGTile* pTile);

	virtual void MoveToTileInternal(ACGTile* pTile, FCGUndo& undo, bool pEvents = true);

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual bool IsBlack() const { return (Flags & EPieceFlags::IsBlack) == EPieceFlags::IsBlack; }

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual bool IsWhite() const { return (Flags & EPieceFlags::IsBlack) != EPieceFlags::IsBlack; }

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual bool IsMoved() const { return (Flags & EPieceFlags::Moved) == EPieceFlags::Moved; }

	virtual TSet<ACGTile*> AvailableMoves();
	virtual bool HasAvailableMoves();
	virtual void FillAttackMap();
	virtual void Capture(bool pAddToCaptured = true);
	virtual void UnCapture();
	virtual ACGChessPlayerController* GetCGController();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual const bool IsCaptured() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess setup")
	void OnPieceGrabbed(bool isGrabbed);

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess events")
	void OnPieceCaptured();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess events")
	void OnPieceMoved();

	UFUNCTION(Client, Reliable)
	void ClientOnPieceMoved();
	void ClientOnPieceMoved_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess events")
	void OnPieceSpawned();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess events")
	void OnInvalidMove();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
