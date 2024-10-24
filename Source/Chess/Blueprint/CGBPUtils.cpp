
#include "Blueprint/CGBPUtils.h"
#include "Engine/DataTable.h"
#include "Components/ComboBoxString.h"
#include "GameLogic/CGSkinData.h"
#include "GameLogic/CGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ChessLogic/CGPiece.h"
#include "ChessLogic/CGTile.h"
#include "ChessLogic/CGChessBoard.h"
#include "EngineUtils.h"
#include "HAL/FileManagerGeneric.h"
#include "HAL/PlatformApplicationMisc.h"
#include <SocketSubsystem.h>
#include <IPAddress.h>

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

void UCGBPUtils::FillCBFromDT(UDataTable* DataTable, UComboBoxString* ComboBox)
{
	if (!DataTable || !ComboBox)
	{
		return;
	}

	ComboBox->ClearOptions();

	for (const auto& it : DataTable->GetRowMap())
	{
		FCGDataBase* row = (FCGDataBase*)(it.Value);
		if (row)
		{
			ComboBox->AddOption(row->Name.ToString());
		}
	}
}

bool UCGBPUtils::IsHotSeatMode(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>())
		{
			FString m = UGameplayStatics::ParseOption(mode->OptionsString, "Mode");
			if (m.Equals("HS", ESearchCase::IgnoreCase))
			{
				return true;
			}
		}
	}
	return false;
}

bool UCGBPUtils::IsChessEngineMode(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>())
		{
			FString m = UGameplayStatics::ParseOption(mode->OptionsString, "Mode");
			if (m.Equals("UCI", ESearchCase::IgnoreCase))
			{
				return true;
			}
		}
	}
	return false;
}

const FString UCGBPUtils::FenFromMapParameter(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>())
		{
			return UGameplayStatics::ParseOption(mode->OptionsString, "Fen");
		}
	}
	return FString();
}

bool UCGBPUtils::IsWaitingForPlayers(const UObject* WorldContextObject)
{
	if (IsHotSeatMode(WorldContextObject) || IsChessEngineMode(WorldContextObject))
	{
		return false;
	}
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return w->GetNumPlayerControllers() != 2;
	}
	return false;
}

bool UCGBPUtils::IsClient(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_Client;
	}
	return false;
}

bool UCGBPUtils::IsListenServer(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_ListenServer;
	}
	return false;
}

bool UCGBPUtils::IsStandalone(const UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		return GEngine->GetNetMode(w) == NM_Standalone;
	}
	return false;
}

bool UCGBPUtils::IsLocalUpdateRequired(const UObject* WorldContextObject)
{
	return IsListenServer(WorldContextObject) || IsStandalone(WorldContextObject);
}

int UCGBPUtils::GetCurrentViewMode(const APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		UGameViewportClient* GameViewportClient = PlayerController->GetWorld()->GetGameViewport();
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		bool ignore = GameViewportClient->IgnoreInput();
		EMouseCaptureMode capt = GameViewportClient->GetMouseCaptureMode();
		if (ignore == false && capt == EMouseCaptureMode::CaptureDuringMouseDown)
		{
			return 0;  // Game And UI
		}
		else if (ignore == true && capt == EMouseCaptureMode::NoCapture)
		{
			return 1;  // UI Only
		}
		else
		{
			return 2;  // Game Only
		}
	}
	return -1;
}

FString UCGBPUtils::UndoToNotationString(const FCGUndo& undo)
{
	if (undo.Imported)
	{
		return FString(TEXT("Imported board state."));
	}
	if (!undo.Piece || !undo.From || !undo.To || !undo.Piece->Board)
	{
		return FString(TEXT("ERROR"));
	}

	//special cases
	//castling
	if (undo.CastleRookTile)
	{
		return FString::Printf(TEXT("%d. %s"), undo.FenMoveNumber, undo.CastleRookTile->Position.X > undo.Piece->Position.X ? TEXT("0-0") : TEXT("0-0-0"));
	}
	//{number}. {piece}{fromX if not simple}{fromY if not simple}{'x' if capture '-' if simple otherwise nothing}{toX}{toY}{piece chosen from promotion (if promotion)}{+ if check # if checkmate}
	FString ret;
	ret.AppendInt(undo.FenMoveNumber);
	ret.Append(". ");
	ret.Append(*undo.Piece->GetUnicode());
	if (!undo.SimpleNotation && undo.From)
	{
		undo.From->AppendCoordToString(ret);
	}
	if (undo.Capture || !undo.SimpleNotation)
	{
		ret.AppendChar(undo.Capture ? 'x' : '-');
	}
	if (undo.To)
	{
		undo.To->AppendCoordToString(ret);
	}
	if (undo.Promotion)
	{
		ret.Append(undo.Promotion->GetFenChar());
	}
	if (undo.Check)
	{
		ret.AppendChar('+');
		//TODO: # if checkmate
	}
	return ret;
}

ACGChessBoard* UCGBPUtils::FindBoard(const UObject* WorldContextObject)
{
	if (TActorIterator<ACGChessBoard> it = TActorIterator<ACGChessBoard>(GEngine->GetWorldFromContextObjectChecked(WorldContextObject)))
	{
		return *it;
	}
	return nullptr;
}

FString UCGBPUtils::GetLocalIP()
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	return (localIp->IsValid() ? localIp->ToString(false) : "");
}

bool UCGBPUtils::IsEditorPresent()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

void UCGBPUtils::BoardFenToClipboard(const UObject* WorldContextObject)
{
	if (ACGChessBoard* board = FindBoard(WorldContextObject))
	{
		FPlatformApplicationMisc::ClipboardCopy(*(board->PiecesToFen()));
	}
}

FString UCGBPUtils::TryFindStockfishPath()
{
	TArray<FString> fileNames;
	FFileManagerGeneric::Get().FindFilesRecursive(fileNames, *FPaths::LaunchDir(), TEXT("*stockfish*"), true, false, false);

	for (FString& s : fileNames)
	{
		if (FPaths::GetExtension(s).ToLower().Equals("exe"))
		{
			return s;
		}
	}
	return FString();
}

