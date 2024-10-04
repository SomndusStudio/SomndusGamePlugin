/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/SSInputLocalPlayerSubsystem.h"

#include "CommonUITypes.h"
#include "EnhancedInputSubsystems.h"

void USSInputLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load global mapping context
	GenericUIMappingContext = GenericUIMappingContextSoft.LoadSynchronous();
}

bool USSInputLocalPlayerSubsystem::FirstTimeMappingContext(const UInputMappingContext* MappingContext)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext) return false;
	}
	return true;
}

bool USSInputLocalPlayerSubsystem::ContainsMappingContextPull(const UInputMappingContext* MappingContext)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext) return true;
	}

	return false;
}

bool USSInputLocalPlayerSubsystem::PullContainsMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	for (const auto& PullToken: InputMappingContextPullTokens)
	{
		if (PullToken.MappingContext == MappingContext && PullToken.Token == Token) return true;
	}
	return false;
}

void USSInputLocalPlayerSubsystem::AddPullMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	FSSInputMappingContextPullToken PullToken;
	PullToken.MappingContext = MappingContext;
	PullToken.Token = Token;
	InputMappingContextPullTokens.Add(PullToken);	
}

void USSInputLocalPlayerSubsystem::PullMappingContext(const UInputMappingContext* MappingContext, int32 InputMappingPriority, FString Token)
{
	// If first time same input mapping context
	if (FirstTimeMappingContext(MappingContext))
	{
		if (CommonUI::IsEnhancedInputSupportEnabled())
		{
			if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSystem->AddMappingContext(MappingContext, InputMappingPriority);
				}
			}
		}

		AddPullMappingContext(MappingContext, Token);
		return;
	}

	// Avoid duplicate entry
	if (!PullContainsMappingContext(MappingContext, Token))
	{
		AddPullMappingContext(MappingContext, Token);
	}

}

void USSInputLocalPlayerSubsystem::PullGlobalMappingContext(FString Token)
{
	PullMappingContext(GenericUIMappingContext, 0, Token);
}

void USSInputLocalPlayerSubsystem::PopGlobalMappingContext(FString Token)
{
	PopMappingContext(GenericUIMappingContext, Token);
}

void USSInputLocalPlayerSubsystem::PopMappingContext(const UInputMappingContext* MappingContext, FString Token)
{
	// Remove from pull array
	InputMappingContextPullTokens.RemoveAll([MappingContext, Token](const FSSInputMappingContextPullToken& Value)
	{
		return Value.MappingContext == MappingContext && Value.Token == Token;
	});
	
	// If pop all same mapping context
	if (!ContainsMappingContextPull(MappingContext))
	{
		if (CommonUI::IsEnhancedInputSupportEnabled())
		{
			if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					InputSystem->RemoveMappingContext(MappingContext);
				}
			}
		}
	}

}
