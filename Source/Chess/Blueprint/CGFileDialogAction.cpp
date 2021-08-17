
#include "Blueprint/CGFileDialogAction.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "GameLogic/CGGameInstance.h"
#include "GameLogic/CGSettingsSave.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

void UCGFileDialogAction::Activate()
{
	if (IDesktopPlatform* dp = FDesktopPlatformModule::Get())
	{
		TArray< FString > filenames;
		void* handle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
		FString title = TEXT("Select UCI engine executable.");
		FString defaultPath;
		FString defaultName;
		FString filter = TEXT("Executable file|*.exe");
		FString ext;
		FPaths::Split(Path, defaultPath, defaultName, ext);
		if (!ext.IsEmpty())
		{
			defaultName.AppendChar('.');
			defaultName.Append(ext);
		}
		int flags = 0;
		//can't get to run the file dialog on my own thread, sho this whole thing is pointless >_<
		if (dp->OpenFileDialog(nullptr, title, defaultPath, defaultName, filter, flags, filenames))
		{
			if (filenames.Num() > 0)
			{
				FString file = filenames[0];
				//AsyncTask(ENamedThreads::GameThread, [this, file]() {
					Completed.Broadcast(file, true);
				//});
				return;//success
			}
		}
	}
	//something went wrong
	//AsyncTask(ENamedThreads::GameThread, [this]() {
		Completed.Broadcast("", false);
	//});
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