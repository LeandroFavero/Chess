// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess.h"
//#include "Templates/SharedPointer.h"
//#include "OnlineSessionSettings.h"
#include "CGGameInstance.generated.h"

class UCGSettingsSave;
class FOnlineSessionSearchResult;

USTRUCT(BlueprintType)
struct FJoinableGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	FOnlineSessionSearchResult SessionInfo;

	FJoinableGame() {};
	FJoinableGame(const FString name, const FOnlineSessionSearchResult sessionInfo) :Name(name), SessionInfo(sessionInfo){};
};
/**
 * 
 */
UCLASS()
class CHESS_API UCGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCGGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, Category = "Chess settings")
	UCGSettingsSave* Settings;

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void SaveCfg();

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	bool LoadCfg();

	//https://unreal.gg-labs.com/wiki-archives/networking/how-to-use-sessions-in-c++

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	bool Host();

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;


	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/**
*	Function fired when a session create request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void Search();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess settings")
	void SearchComplete(const TArray<FJoinableGame>& Games, bool bWasSuccessful);
	
	/**
*	Find an online session
*
*	@param UserId user that initiated the request
*	@param bIsLAN are we searching LAN matches
*	@param bIsPresence are we searching presence sessions
*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);


	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void Join();

	/**
*	Joins a session via a search result
*
*	@param SessionName name of session
*	@param SearchResult Session to join
*
*	@return bool true if successful, false otherwise
*/
	bool JoinSessionInternal(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/**
*	Delegate fired when a session join request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void DestroySession();

	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
