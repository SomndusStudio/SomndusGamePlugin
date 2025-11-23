/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Messaging/CommonGameDialog.h"
#include "Messaging/CommonMessagingSubsystem.h"
#include "Notification/SSGameNotificationManager.h"
#include "SSGameMessagingSubsystem.generated.h"

namespace SSGameplayTags
{
	SOMNDUSGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Loading_Modal);
	SOMNDUSGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag_Loading_Indicator);
};

USTRUCT(BlueprintType)
struct SOMNDUSGAME_API FSSLoadingInformationTokenInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NameToken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Token;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag LoadingType;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSSLoadingTokenChangeEventDelegate, USSGameMessagingSubsystem*, GameMessagingSubsystem, FGameplayTag, LoadingType, bool, bRemoved);

/**
 * A subsystem class responsible for handling game messaging functionalities,
 * enabling communication and management of various messaging systems within the game.
 */
UCLASS()
class SOMNDUSGAME_API USSGameMessagingSubsystem : public UCommonMessagingSubsystem
{
	GENERATED_BODY()

public:
	USSGameMessagingSubsystem() { }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void ShowConfirmation(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate()) override;
	virtual void ShowError(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate()) override;

	void ShowAlert(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate());

	void ShowLoading(UCommonGameDialogDescriptor* DialogDescriptor, FCommonMessagingResultDelegate ResultCallback = FCommonMessagingResultDelegate());

	void HandleGlobalLoadingResult(ECommonMessagingResult CommonMessagingResult);
	
	UFUNCTION(BlueprintCallable)
	void InternalShowGlobalLoading(FText Message);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (WorldContext = "InWorldContextObject"))
	static void ShowGlobalLoading(UObject* InWorldContextObject, FText Message);
	
	UFUNCTION(BlueprintCallable)
	void InteralCloseCurrentLoadingDialog();
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta = (WorldContext = "InWorldContextObject"))
	static void CloseCurrentLoadingDialog(UObject* InWorldContextObject);
	
	static UCommonGameDialogDescriptor* CreateAlertDescriptor(const FText& Header, const FText& Body);
	static UCommonGameDialogDescriptor* CreateLoadingDescriptor(const FText& Body);

	/**
	 * 
	 * @param Message 
	 * @param LoadingToken 
	 * @param LoadingType 
	 * @return Generated token
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	FName RequestLoadingInformation(const FText& Message, FGameplayTag LoadingToken, FGameplayTag LoadingType);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void StopLoadingInformation(FGameplayTag LoadingToken, FGameplayTag LoadingType);
	
	void AddUniqueLoadingInfo(const FSSLoadingInformationTokenInfo& NewInfo);
	void RemoveLoadingInfoByToken(const FGameplayTag& TokenToRemove);
	bool FindBetterLoadingInfo(FGameplayTag LoadingType, FSSLoadingInformationTokenInfo& OutResult);
	bool ContainAnyLoadingInfoFromType(FGameplayTag LoadingType);

	UPROPERTY(BlueprintAssignable, Category = "Notification")
	FSSLoadingTokenChangeEventDelegate OnLoadingTokenChange;
	
	FCommonMessagingResultDelegate GlobalLoadingResultCallback;

protected:
	
	UPROPERTY()
	TArray<FSSLoadingInformationTokenInfo> LoadingInformations;
	
private:
	UPROPERTY()
	TSubclassOf<UCommonGameDialog> ConfirmationDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UCommonGameDialog> ErrorDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UCommonGameDialog> AlertDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UCommonGameDialog> LoadingDialogClassPtr;

	UPROPERTY()
	TObjectPtr<UCommonGameDialog> CurrentLoadingDialogWidget;
	
	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> ConfirmationDialogClass;

	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> ErrorDialogClass;

	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> AlertDialogClass;

	UPROPERTY(config)
	TSoftClassPtr<UCommonGameDialog> LoadingDialogClass;
	
	UPROPERTY()
	TObjectPtr<USSGameNotificationManager> NotificationManager;

public:

	UFUNCTION(BlueprintCallable)
	USSGameNotificationManager* GetNotificationManager() const;
};
