
#include "Blueprint/CGFileDialogAction.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGSettingsSave.h"

#if PLATFORM_WINDOWS
	#include "Windows/MinWindows.h"
	#include <commdlg.h>
#endif

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

void UCGFileDialogAction::Activate()
{
#if PLATFORM_WINDOWS
	HWND hWnd = (HWND)(FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr));
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, hWnd]()
	{
		OPENFILENAME ofn;       // common dialog box structure
		TCHAR szFile[512] = { 0 };       // if using TCHAR macros
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("Executable\0*.EXE\0All\0*.*\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			FString path(ofn.lpstrFile);
			AsyncTask(ENamedThreads::GameThread, [this, path]()
			{
				Completed.Broadcast(path, true);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				Completed.Broadcast("", false);
			});
		}
	});
#endif
}

UCGFileDialogAction* UCGFileDialogAction::OpenFileChooser(UObject* WorldContextObject)
{
	if (UWorld* w = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		if (UCGGameInstance* inst = w->GetGameInstance<UCGGameInstance>())
		{
			if (inst->Settings)
			{
				UCGFileDialogAction* ret = NewObject<UCGFileDialogAction>();
				ret->RegisterWithGameInstance(WorldContextObject);
				ret->Path = inst->Settings->EnginePath;
				return ret;
			}
		}
	}
	return nullptr;
}