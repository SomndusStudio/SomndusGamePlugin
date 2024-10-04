/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Messaging/CommonGameDialog.h"
#include "Messaging/CommonMessagingSubsystem.h"
#include "SSGameMessagingSubsystem.generated.h"

/**
 * 
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

	FCommonMessagingResultDelegate GlobalLoadingResultCallback;
	
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
};
