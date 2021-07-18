// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessBoard.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGGameMode.h"
#include "CGRook.h"
#include "CGKing.h"
#include "ChessLogic/CGTile.h"
#include "GameLogic/CGChessPlayerController.h"
#include "GameLogic/CGCapturedPieces.h"
#include "UI/CGHUD.h"
#include "Misc/Char.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/CGBPUtils.h"

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

void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();

	if (UCGBPUtils::IsHotSeatMode(this))
	{
		StartGame(UCGBPUtils::FenFromMapParameter(this), GetWorld()->GetFirstPlayerController<ACGChessPlayerController>());
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
	if (UWorld* w = GetWorld())
	{
		for (int32 i = Board.Num(); i < tileCount; ++i)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			ACGTile* newTile = w->SpawnActor<ACGTile>(TileTemplate, params);
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
		ACGTile* tile = Board[i];
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
		ACGTile* tile = Board[i];

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
	for (ACGTile* t : Board)
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

void ACGChessBoard::StartGame(const FString& fen, ACGChessPlayerController* p1, ACGChessPlayerController* p2)
{
	if (HasAuthority())
	{
		FenStringToChessPieces(DefaultBoardFen);
		if (p2)
		{
			if (p1)
			{
				//randomize
				if (p1->PreferredSide == p2->PreferredSide && (p1->PreferredSide == 0 || p1->PreferredSide == 1))
				{
					p1->bIsBlack = static_cast<bool>(FMath::RandRange(0, 1));
					p2->bIsBlack = !p1->bIsBlack;
				}
				else
				{
					//give them what they want
					if (p1->PreferredSide == 2)
					{
						p1->bIsBlack = p2->PreferredSide == 0;
					}
					else
					{
						p1->bIsBlack = p1->PreferredSide == 1;
					}
					if (p2->PreferredSide == 2)
					{
						p2->bIsBlack = p1->PreferredSide == 0;
					}
					else
					{
						p2->bIsBlack = p2->PreferredSide == 1;
					}
				}
			}
		}
		else
		{
			//solo mode

		}
		
	}
}

/*
	default fen:
	rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/
bool ACGChessBoard::FenStringToChessPieces(const FString& fen)
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
	Undos.Empty();

	int8 x = 0;
	int8 y = Size.Y - 1;
	bool malformed{ false };
	UWorld* world = GetWorld();
	if (!world)
	{
		return false;
	}

	int field = 0;
	auto it = fen.CreateConstIterator();
	for ( ;field == FenField::PIECE_PLACEMENT && it && *it != ' '; ++it)
	{
		const TCHAR& chr = *it;

		if (x >= Size.X)
		{
			x = 0;
			y -= 1;
		}

		//is new line
		if (chr == '\\' || chr == '/')
		{
			continue;
		}
		//is numeric
		if (chr >= 48 && chr <= 57)
		{
			int8 skip = chr - 48;
			x += skip;
			continue;
		}
		else
		{
			if (ACGGameState* gameState = world->GetGameState<ACGGameState>())
			{
				FCGUndo dummyUndo;//we don't want to undo the initial piece spawns!
				for (TSubclassOf<class ACGPiece> temp : gameState->PieceTemplates)
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
									//newPiece->SetMaterial(isWhite ? gameState->WhiteMaterial : gameState->BlackMaterial);
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

									//charHandled = true;
									x += 1;

									goto continueNextPiece;
								}
							}
						}
					}
				}
			}
		}
		//the only way to get here is to find an unexpected character
		malformed = true;
	continueNextPiece:;
	}

	field += 1;
	++it;
	for (; field == FenField::NEXT_MOVE && it && *it != ' '; ++it)
	{
		const TCHAR& chr = *it;
		if (chr == 'w' || chr == 'W')
		{
			//there's nothing to do, the first move belongs to white already
		}
		else if (chr == 'b' || chr == 'B')
		{
			//add dummy undo token with a white move
			FCGUndo& u = CreateUndo();
			u.Imported = true;
			u.LastMoveIsBlack = false;
		}
		else
		{
			malformed = true;
		}
	}
	field += 1;
	++it;
	FString tmp("KQkq",0);
	for (; field == FenField::CASTLING_AVAILABILITY && it; ++it)
	{
		const TCHAR& chr = *it;
		if (chr == ' ')
		{
			//we're done!
			field += 1;
			//flag the remaining pieces moved, so they can't castle
			for (const TCHAR& chr2 : tmp)
			{
				if (chr2 == 'K' || chr2 == 'Q' || chr2 == 'k' || chr2 == 'q')
				{
					for (ACGTile* t = (TChar<TCHAR>::IsLower(chr2) ? BlackKing : WhiteKing)->Tile; t; t = t->Neighbours[(chr2 == 'K' || chr2 == 'k') ? EDir::WEST : EDir::EAST])
					{
						if (t->OccupiedBy && t->OccupiedBy->IsA(ACGRook::StaticClass()))
						{
							t->OccupiedBy->Flags |= EPieceFlags::Moved;
							goto continueNextChar;
						}
					}
				}
				else
				{
					malformed = true;
				}
			continueNextChar:;
			}
		}
		else
		{
			//since rooks can castle by default, we have to find what's missing from the string
			int idx{ 0 };
			if (tmp.FindChar(chr, idx))
			{
				tmp.RemoveAt(idx);
			}
			else if(chr != '-')
			{
				malformed = true;
			}
		}
	}

	tmp.Empty(4);
	for (; field == FenField::EN_PASSANT_TARGET_SQUARE && it && *it != ' '; ++it)
	{
		//the square that was skipped with a pawn double opener
		if (*it != '-')
		{
			for (; it && *it != ' '; ++it)
			{
				const TCHAR& chr = *it;
				tmp.AppendChar(chr);
			}
			//find the pawn that did the double open and add dummy undo token
			if (tmp.Len() == 2)
			{
				tmp = tmp.ToLower();
				int tileX = tmp[0] - 'a';
				int tileY = tmp[1] - '1';
				bool isBlack = tileY > (Size.Y / 2);
				tileY = isBlack ? tileY - 1 : tileY + 1;
				ACGTile* t = GetTile({ tileX, tileY });
				if (t && t->OccupiedBy)
				{
					FCGUndo& u = Undos.Num() > 0 ? Undos.Last() : CreateUndo();
					u.Imported = true;
					u.Piece = t->OccupiedBy;
					u.To = t;
					for (int distance = 0; distance < 2; ++distance)
					{
						if (t)
						{
							t = t->Neighbours[isBlack ? EDir::NORTH : EDir::SOUTH];
						}
					}
					u.From = t;
				}
			}
			else
			{
				malformed = true;
			}
			field += 1;
		}
	}

	for (; field == FenField::HALFMOVE_CLOCK && it && *it != ' '; ++it)
	{
		//fifty-move rule is not supported yet :( 
	}
	field += 1;
	++it;
	for (; field == FenField::FULLMOVE_NUMBER && it && *it != ' '; ++it)
	{
		//at the moment we're using the undo count to determine the current move number
	}

	for (ACGPiece* p : Pieces)
	{
		if (UCGBPUtils::IsListenServer(this))
		{
			p->SnapToPlace();
		}
		else
		{
			p->ClientSnapToPlace();
		}
	}

	//update moves on listen server
	if (Undos.Num() > 0 && UCGBPUtils::IsListenServer(this))
	{
		UndoNotify();
	}
	GameStarted();
	return !malformed;
}

FTransform ACGChessBoard::CoordToTransform(const FCGSquareCoord& coord) const
{
	float boardHalfSizeX = TileSize.X * .5f * (Size.X - 1);
	float boardHalfSizeY = TileSize.Y * .5f * (Size.Y - 1);

	FTransform tileTransform = GetTransform();
	FVector tileLocation = tileTransform.GetLocation();

	tileLocation.X += coord.X * TileSize.X - boardHalfSizeX;
	tileLocation.Y += (Size.Y-1-coord.Y) * TileSize.Y - boardHalfSizeY;
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
	return FCGSquareCoord(Size.X - boardLoc.X / TileSize.X, boardLoc.Y / TileSize.Y);
}

bool ACGChessBoard::HasValidMove(bool pIsBlack)
{
	ACGPiece** found = Pieces.FindByPredicate([pIsBlack](ACGPiece* p) { return p->IsBlack() == pIsBlack && p->HasAvailableMoves(); });
	return found != nullptr;
}

bool ACGChessBoard::GameOverCheck(bool pIsBlack)
{
	if (!HasValidMove(pIsBlack))
	{
		if (UWorld* w = GetWorld())
		{
			if(ACGGameState* gameState = Cast<ACGGameState>(w->GetGameState()))
			{
				//checkmate?
				if ((pIsBlack ? BlackKing : WhiteKing)->IsInCheck())
				{
					gameState->ClientGameFinished(pIsBlack ? EGameResult::WHITE_WINS : EGameResult::BLACK_WINS);
				}
				else
				{
					gameState->ClientGameFinished(EGameResult::DRAW);
				}
				return true;
			}
		}
	}
	return false;
}

void ACGChessBoard::RefreshPieceColors()
{
	for (ACGPiece * p : Pieces)
	{
		if (p)
		{
			p->RefreshMaterial();
		}
	}
}

ACGTile* ACGChessBoard::GetTile(const FCGSquareCoord& coord)
{
	ACGTile* ret = Board[coord.X * Size.X + coord.Y];
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
	if (pUndo.CastleRook)
	{
		pUndo.CastleRook->MoveToTileInternal(pUndo.CastleRookTile, dummyUndo, false);
		pUndo.CastleRook->ClientSnapToPlace();
	}
	if (pUndo.Promotion)
	{
		pUndo.Promotion->Destroy();
	}
}

void ACGChessBoard::GameStarted_Implementation()
{
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		state->GameState = EGameResult::NOT_FINISHED;
	}
	if (ACGHUD* hud = GetWorld()->GetFirstPlayerController()->GetHUD<ACGHUD>())
	{
		hud->ShowGame();
	}
}

void ACGChessBoard::UndoTo(int pMoveNum)
{
	for (int i = Undos.Num() - 1; i >= pMoveNum;--i)
	{
		if (!Undos[i].Imported) 
		{
			UndoInternal(Undos[i]);
			Undos.RemoveAt(i);
		}
	}
	//listen server has to update the ui
	if (UCGBPUtils::IsListenServer(this))
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
	for (ACGTile* t : Board)
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
