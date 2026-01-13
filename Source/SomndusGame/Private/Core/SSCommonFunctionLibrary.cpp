/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Core/SSCommonFunctionLibrary.h"

#include "Core/SSCommonTypes.h"
#include "Core/SSGameSubsystem.h"
#include "DAO/SSDAOManager.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

FText USSCommonFunctionLibrary::LocalizeFromString(FString Key)
{
	FText Result;
	if (FText::FindTextInLiveTable_Advanced(FString("FGameModule"), Key, Result))
	{
		return Result;
	}
	return FText::FromString(Key);
}

FText USSCommonFunctionLibrary::LocalizeFromName(const FName Key)
{
	return LocalizeFromString(Key.ToString());
}

USSGameModule* USSCommonFunctionLibrary::GetGameModule(UObject* ContextObject, TSubclassOf<USSGameModule> Class)
{
	if (USSGameSubsystem* GameSubsystem = UGameplayStatics::GetGameInstance(ContextObject)->GetSubsystem<
	USSGameSubsystem>())
	{
		return GameSubsystem->GetGameModule(Class);
	}
	UE_LOG(LogTemp, Warning, TEXT("Can't found GameModule named %s"), *Class->GetName());
	return nullptr;
}

USSDAOObjectBase* USSCommonFunctionLibrary::GetDAOByClass(UObject* ContextObject, TSubclassOf<USSDAOObjectBase> Class)
{
	if (auto* DAOManager = GetGameModuleByClass<USSDAOManager>(ContextObject))
	{
		return DAOManager->GetDAO(Class);
	}
	UE_LOG(LogTemp, Warning, TEXT("Can't found DAO named %s"), *Class->GetName());
	return nullptr;
}

bool USSCommonFunctionLibrary::OperationResult_IsSuccess(const FSSOperationResult& OperationResult)
{
	return OperationResult.IsSuccess();
}

bool USSCommonFunctionLibrary::OperationResult_IsError(const FSSOperationResult& OperationResult)
{
	return OperationResult.IsError();
}
