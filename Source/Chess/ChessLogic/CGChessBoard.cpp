// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessBoard.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGGameMode.h"
//#include "CGPiece.h"
#include "CGKing.h"
#include "GameLogic/CGBoardTile.h"
#include "GameLogic/CGChessPlayerController.h"
#include "GameLogic/CGCapturedPieces.h"
#include "UI/CGHUD.h"
#include "Net/UnrealNetwork.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

// Sets default values
ACGChessBoard::ACGChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bRunConstructionScriptOnDrag = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	bReplicates = true;
	bOnlyRelevantToOwner = false;
}

// Called when the game starts or when spawned
void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* w = GetWorld())
	{
		//GetGameState
		ACGGameState* gameState = Cast<ACGGameState>(w->GetGameState());
		if (gameState)
		{
			gameState->Board = this;
		}

		//TODO remove?
		StartGame(Cast<ACGChessPlayerController>(w->GetFirstPlayerController()));
	}
}

void ACGChessBoard::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	for (AActor* a : Board)
	{
		if (!a || a->IsActorBeingDestroyed())
		{
			//reset = true;
			Board.Empty();
			break;
		}
	}
	if (!TileTemplate)
	{
		return;
	}
	
	int32 tileCount = Size.X * Size.Y;

	//add or delete to match the tile count
	for (int32 i = Board.Num() - 1; i > tileCount; --i)
	{
		Board[i]->Destroy();
		Board.RemoveAt(i);
	}
	if (UWorld* world = GetWorld())
	{
		for (int32 i = Board.Num(); i < tileCount; ++i)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			ACGBoardTile* newTile = world->SpawnActor<ACGBoardTile>(TileTemplate, params);
			newTile->Board = this;
			Board.Add(newTile);
		}
	}
	else
	{
		//bail
		//TODO: log error
		return;
	}
	for (int32 i = 0; i < tileCount; ++i)
	{
		ACGBoardTile* tile = Board[i];
		if (tile)
		{
			FCGSquareCoord coord{ i / Size.Y,i % Size.Y };
			tile->SetCoord(coord);
			tile->SetActorTransform(CoordToTransform(coord));
		}
	}
	//set up neighbour references
	for (int32 i = 0; i < tileCount; ++i)
	{
		ACGBoardTile* tile = Board[i];

		int otherIdx = i + 1;
		tile->Neighbours[static_cast<int>(EDir::NORTH)] = (otherIdx % Size.Y == 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::NORTH_EAST)] = (otherIdx % Size.Y == 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::EAST)] = (otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::SOUTH_EAST)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];

		otherIdx = i - 1;
		tile->Neighbours[static_cast<int>(EDir::SOUTH)] = (otherIdx < 0 || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::SOUTH_WEST)] = (otherIdx < 0 || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];

		otherIdx = i - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::WEST)] = (otherIdx < 0) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::NORTH_WEST)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0) ? nullptr : Board[otherIdx];

		//knight moves clockwise
		otherIdx = i + 2 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT1)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0 || otherIdx % Size.Y == 1) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 + (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT2)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 + (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT3)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];

		otherIdx = i - 2 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT4)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0 || otherIdx % Size.Y == Size.Y - 2 || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];


		otherIdx = i - 2 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT5)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0 || otherIdx % Size.Y == Size.Y - 2 || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];
		
		otherIdx = i - 1 - (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT6)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == Size.Y - 1) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 - (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT7)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0) ? nullptr : Board[otherIdx];

		otherIdx = i + 2 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT8)] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.Y == 0 || otherIdx % Size.Y == 1) ? nullptr : Board[otherIdx];

	}
}

void ACGChessBoard::Destroyed()
{
	for (ACGBoardTile* t : Board)
	{
		if (t)
		{
			t->Destroy();
		}
	}
	Board.Empty();
	for (ACGPiece* p : Pieces)
	{
		if (p)
		{
			p->Destroy();
		}
	}
	Pieces.Empty();
	Undos.Empty();
	Super::Destroyed();
}

void ACGChessBoard::StartGame(ACGChessPlayerController* p1, ACGChessPlayerController* p2)
{
	//TODO: remove
		
	if (HasAuthority())
	{
		FenStringToChessPieces(DefaultBoardFen);
		Undos.Empty();
	}
}


/*
	default fen:
	rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/
bool ACGChessBoard::FenStringToChessPieces(FString fen)
{
	//clear the pieces from the board
	for (ACGPiece* p : Pieces)
	{
		if (p)
		{
			p->Destroy();
		}
	}
	Pieces.Empty();

	int8 x = 0;
	int8 y = Size.Y-1;
	bool boardFinished = false;
	UWorld* world = GetWorld();
	if (!world)
	{
		return false;
	}
	for (const TCHAR& chr : fen)
	{
		bool charHandled = false;
		if (!boardFinished)
		{
			//is new line
			if (chr == '\\' || chr == '/')
			{
				//x = 0;
				//y += 1;
				charHandled = true;
			}
			//is numeric
			if (chr >= 48 && chr <= 57)
			{
				int8 skip = chr - 48;
				x += skip;
				charHandled = true;
			}
			else
			{
				if (ACGGameMode* gameMode = world->GetAuthGameMode<ACGGameMode>())
				{
					FCGUndo dummyUndo;//we don't want to undo the initial piece spawns!
					for (TSubclassOf<class ACGPiece> temp : gameMode->PieceTemplates)
					{
						if (temp) 
						{
							ACGPiece* def = Cast<ACGPiece>(temp.Get()->GetDefaultObject());
							if (def)
							{
								for (const TCHAR pieceChr : def->GetFenChars())
								{
									if (pieceChr == chr)
									{
										//spawn piece
										FActorSpawnParameters params;
										bool isWhite = TChar<TCHAR>::IsUpper(pieceChr);
										params.Owner = this;
										ACGPiece* newPiece = world->SpawnActor<ACGPiece>(temp, params);
										newPiece->SetMaterial(isWhite ? gameMode->WhiteMaterial : gameMode->BlackMaterial);
										newPiece->SetColor(isWhite);
										newPiece->Board = this;
										newPiece->MoveToTileInternal(GetTile({ x,y }), dummyUndo, false);
										Pieces.Add(newPiece);

										//ptr to the kings
										if (newPiece->IsA(ACGKing::StaticClass()))
										{
											if (newPiece->IsBlack())
											{
												BlackKing = Cast<ACGKing>(newPiece);
											}
											else
											{
												WhiteKing = Cast<ACGKing>(newPiece);
											}
										}

										charHandled = true;
										x += 1;

										goto continuePieceDone;
									}
								}
							}
						}
					}
				}
			}

			continuePieceDone:;

			if (x >= Size.X)
			{
				x = 0;
				y -= 1;
			}
		}
		if (!charHandled)
		{
			break;
			return false;
		}
		if (x == Size.X && y <= 0)
		{
			boardFinished = true;
			//TODO: remove this!
			break;
			//return true;
		}
	}
	for (ACGPiece* p : Pieces)
	{
		if (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
		{
			p->SnapToPlace();
		}
		else
		{
			p->ClientSnapToPlace();
		}
	}
	return true;
}

FTransform ACGChessBoard::CoordToTransform(const FCGSquareCoord& coord) const
{
	float boardHalfSizeX = TileSize.X * .5f * (Size.X - 1);
	float boardHalfSizeY = TileSize.Y * .5f * (Size.Y - 1);

	FTransform tileTransform = GetTransform();
	FVector tileLocation = tileTransform.GetLocation();

	tileLocation.X += coord.X * TileSize.X - boardHalfSizeX;
	tileLocation.Y += coord.Y * TileSize.Y - boardHalfSizeY;
	tileTransform.SetLocation(tileLocation);
	return tileTransform;
}

FCGSquareCoord ACGChessBoard::TransformToCoord(const FTransform& transform)
{
	return LocationToCoord(transform.GetLocation());
}

FCGSquareCoord ACGChessBoard::LocationToCoord(const FVector& location)
{
	float boardHalfSizeX = TileSize.X * .5f * Size.X;// -1);
	float boardHalfSizeY = TileSize.Y * .5f * Size.Y;// -1);

	FVector boardLoc = GetTransform().GetLocation();
	boardLoc.X += boardHalfSizeX;
	boardLoc.Y += boardHalfSizeY;
	boardLoc -= location;
	return FCGSquareCoord(Size.X - boardLoc.X / TileSize.X, Size.Y - boardLoc.Y / TileSize.Y);
}

ACGBoardTile* ACGChessBoard::GetTile(const FCGSquareCoord& coord)
{
	ACGBoardTile* ret = Board[coord.X * Size.X + coord.Y];
	ensure(coord == ret->Position);
	return ret;
}

void ACGChessBoard::CoordToLabel(const FCGSquareCoord coord, TCHAR& X, TCHAR& Y)
{
	X = 'A'+coord.X;
	Y = coord.Y + 1;
}

FCGUndo& ACGChessBoard::CreateUndo()
{
	Undos.Emplace(Undos.Num());
	return Undos.Last();
}

void ACGChessBoard::UndoInternal(FCGUndo& pUndo)
{
	FCGUndo dummyUndo;
	if (pUndo.Piece)
	{
		pUndo.Piece->MoveToTileInternal(pUndo.From, dummyUndo, false);
		pUndo.Piece->Flags = pUndo.Flags;
		pUndo.Piece->ClientSnapToPlace();
	}
	if (pUndo.Capture)
	{
		pUndo.Capture->UnCapture();
		pUndo.Capture->MoveToTileInternal(pUndo.To, dummyUndo, false);
		pUndo.Capture->ClientSnapToPlace();
	}
	if (pUndo.Promotion)
	{
		pUndo.Promotion->Destroy();
	}
}

void ACGChessBoard::UndoTo(int pMoveNum)
{
	for (int i = Undos.Num() - 1; i >= pMoveNum;--i)
	{
		UndoInternal(Undos[i]);
		Undos.RemoveAt(i);
	}
	//listen server has to update the ui
	if (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
	{
		UndoNotify();
	}
}

void ACGChessBoard::RequestUndoTo(int pMoveNum)
{

}

void ACGChessBoard::UndoNotify()
{
	//GetLoc
	if (UWorld* w = GetWorld())
	{
		if (ULocalPlayer* lp = w->GetFirstLocalPlayerFromController())
		{
			if (APlayerController* pc = lp->GetPlayerController(w))
			{
				if (ACGHUD* hud = pc->GetHUD<ACGHUD>())
				{
					hud->UpdateHud();
				}
			}
		}
	}
}

void ACGChessBoard::RebuildAttackMap(bool pIsBlack)
{
	for (ACGBoardTile* t : Board)
	{
		if (t)
		{
			t->AttackedBy.Empty();
		}
	}
	for (ACGPiece* p : Pieces)
	{
		if (p && p->IsBlack() == pIsBlack)
		{
			p->FillAttackMap();
		}
	}
}

void ACGChessBoard::ApplySkin(ACGChessPlayerController* playerController, int skin)
{

}

void ACGChessBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGChessBoard, Pieces)
	DOREPLIFETIME(ACGChessBoard, CapturedWhite)
	DOREPLIFETIME(ACGChessBoard, CapturedBlack)
	DOREPLIFETIME(ACGChessBoard, Board)
	DOREPLIFETIME(ACGChessBoard, Undos)
	DOREPLIFETIME(ACGChessBoard, WhiteKing)
	DOREPLIFETIME(ACGChessBoard, BlackKing)
}
