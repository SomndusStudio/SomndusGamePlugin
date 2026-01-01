/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Input/Context/SSInputContextComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SSLog.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Helper/SSHelperStatics.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Pawn/SSPawnData.h"

FName USSInputContextComponent::NAME_InputContextReady = FName("InputContextReady");

USSInputContextComponent::USSInputContextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USSInputContextComponent::BeginPlay()
{
	Super::BeginPlay();

	InputAxisMgr = NewObject<USSInputAxisMgr>(this);
	
	OwnerController = Cast<APlayerController>(GetOwner());

	if (!OwnerController)
	{
		UE_LOG(LogSomndusInput, Warning, TEXT("[USSInputContextComponent] Owner is not a PlayerController."));
		return;
	}

	ControlledPawn = OwnerController->GetPawn();

	if (ControlledPawn.IsValid())
	{
		OnPawnChanged(ControlledPawn.Get());
	}
}

UObject* USSInputContextComponent::GetOrCreateSharedObject(TSubclassOf<UObject> InClass)
{
	for (UObject* Obj : SharedObjects)
	{
		if (Obj->IsA(InClass))
		{
			return Obj;
		}
	}
	
	UObject* NewObj = NewObject<UObject>(this, InClass);
	SharedObjects.Add(NewObj);
	return NewObj;
}

void USSInputContextComponent::RegisterPawnInputContext(const FGameplayTag& ContextTag, USSPawnInputContext* InputContext)
{
	if (!InputContext) return;

	PawnInputContexts.Add(ContextTag, InputContext);
	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Registered PawnInputContext for tag %s"), *ContextTag.ToString());
}

USSPawnInputContext* USSInputContextComponent::GetOrCreatePawnInputContext(const FGameplayTag& ContextTag)
{
	if (TObjectPtr<USSPawnInputContext>* FoundContext = PawnInputContexts.Find(ContextTag))
	{
		return *FoundContext;
	}
	// Create
	USSPawnInputContext* NewPawnContext = NewObject<USSPawnInputContext>(this);
	NewPawnContext->NativeInitialize(this, ContextTag);
	PawnInputContexts.Add(ContextTag, NewPawnContext);
	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Created new PawnInputContext for tag %s"), *ContextTag.ToString());

	return NewPawnContext;
}

bool USSInputContextComponent::RegisteringPawnData(const TObjectPtr<USSPawnData> PawnData)
{
	LoadedPawnsData.AddUnique(PawnData);

	return true;
}

void USSInputContextComponent::OnPawnChanged(APawn* NewPawn)
{
	if (!NewPawn)
	{
		UE_LOG(LogSomndusInput, Warning, TEXT("[USSInputContextComponent] NewPawn is null"));
		return;
	}

	ControlledPawn = NewPawn;
	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Controlled Pawn changed to %s"), *NewPawn->GetName());

	// Deactivate previous context if any
	if (ActivePawnInputContext)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
		{
			if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				ActivePawnInputContext->Deactivate(InputComp);
			}
		}

		ActivePawnInputContext = nullptr;
	}

	// Step 1: Try to find the corresponding PawnData in LoadedPawnsData
	FGameplayTag MatchingTag;
	for (const TObjectPtr<USSPawnData>& PawnData : LoadedPawnsData)
	{
		if (!PawnData || !PawnData->PawnClass) continue;

		if (NewPawn->IsA(PawnData->PawnClass))
		{
			MatchingTag = PawnData->PawnTypeTag;
			UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Matched PawnClass %s to Tag %s"),
			       *PawnData->PawnClass->GetName(), *MatchingTag.ToString());
			break;
		}
	}

	// Step 2: Activate input context by tag
	if (MatchingTag.IsValid())
	{
		ActivateContextByTag(MatchingTag);
	}
	else
	{
		UE_LOG(LogSomndusInput, Warning, TEXT("[USSInputContextComponent] No PawnData match found for controlled pawn %s"), *NewPawn->GetName());
	}
}

void USSInputContextComponent::ActivateContextByTag(const FGameplayTag& Tag)
{
	TObjectPtr<USSPawnInputContext> FoundContext = GetOrCreatePawnInputContext(Tag);
	if (!FoundContext)
	{
		UE_LOG(LogSomndusInput, Warning, TEXT("[USSInputContextComponent] No input context found for tag %s"), *Tag.ToString());
		return;
	}

	ActivePawnInputContext = FoundContext;

	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Activated context %s"), *Tag.ToString());
}

void USSInputContextComponent::Init(APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;

	UEnhancedInputComponent* _EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	SetEIC(_EIC);
	
	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Initialized with PlayerController %s"), *PlayerController->GetName());
}

APlayerController* USSInputContextComponent::GetPlayerController()
{
	return Cast<APlayerController>(GetOwner());
}

void USSInputContextComponent::SetEIC(UEnhancedInputComponent* InEIC)
{
	// if should unbind old
	if (IsValid(EIC))
	{
		for (const auto& InInputActionEvent : EIC->GetActionEventBindings())
		{
			EIC->RemoveBinding(*InInputActionEvent);
		}
	}
	EIC = InEIC;
}

bool USSInputContextComponent::AddInputTypeModeMapping(const FSSInputTypeModeMapping& InInputTypeModeMapping)
{
	UE_LOG(LogSomndusInput, Log, TEXT("%s: Attempt to AddInputTypeModeMapping with %d InputTypes and %d ContextTags"),
	       *GetNameSafe(this), InInputTypeModeMapping.InputTypes.Num(), InInputTypeModeMapping.InputMappingContextTags.Num());

	// if should merge/override
	bool bUpdate = false;
	for (auto& InputTypeModeMapping : InputTypeModeMappings)
	{
		// in Same Input type Mode (Gamepad, Keyboard, etc)
		if (InputTypeModeMapping.HasSameInputTypes(InInputTypeModeMapping))
		{
			UE_LOG(LogSomndusInput, Log, TEXT("%s: Found existing mapping with same InputTypes, merging InputMappingContextTags"), *GetNameSafe(this));

			// Merge distinct input mapping tags (Default,AbilitySet,etc)
			for (auto& OtherInputMappingContextTag : InInputTypeModeMapping.InputMappingContextTags)
			{
				// 0 : if existing config already have this type
				bool bInputMappingContextTagsUpdated = false;
				for (auto& InputMappingContextTag : InputTypeModeMapping.InputMappingContextTags)
				{
					if (InputMappingContextTag.Tag == OtherInputMappingContextTag.Tag)
					{
						UE_LOG(LogSomndusInput, Verbose, TEXT("%s: Appending contexts to existing tag %s"),
						       *GetNameSafe(this), *OtherInputMappingContextTag.Tag.ToString());

						// TODO: Force append unique ?
						InputMappingContextTag.InputMappingContexts.Append(OtherInputMappingContextTag.InputMappingContexts);
						bInputMappingContextTagsUpdated = true;
					}
				}
				// 1 : if should add new InputMappingContextTag because first time
				if (!bInputMappingContextTagsUpdated)
				{
					UE_LOG(LogSomndusInput, Log, TEXT("%s: Adding new InputMappingContextTag %s"),
					       *GetNameSafe(this), *OtherInputMappingContextTag.Tag.ToString());

					InputTypeModeMapping.InputMappingContextTags.Add(OtherInputMappingContextTag);
				}
			}

			UE_LOG(LogSomndusInput, Verbose, TEXT("%s: Appending unique DefaultActiveTags (%d new)"),
			       *GetNameSafe(this), InInputTypeModeMapping.DefaultActiveTags.Num());

			USSHelperStatics::AppendUniqueValue(InputTypeModeMapping.DefaultActiveTags, InInputTypeModeMapping.DefaultActiveTags);

			// Attempt to update missing
			if (CurrentInputTypeModeMapping->HasSameInputTypes(InputTypeModeMapping))
			{
				UE_LOG(LogSomndusInput, Log, TEXT("%s: CurrentInputTypeModeMapping updated"), *GetNameSafe(this));

				CurrentInputTypeModeMapping = &InputTypeModeMapping;
			}

			bUpdate = true;

			break;
		}
	}

	if (!bUpdate)
	{
		UE_LOG(LogSomndusInput, Log, TEXT("%s: No existing mapping found, adding as new"), *GetNameSafe(this));

		InputTypeModeMappings.Add(InInputTypeModeMapping);
	}

	// Check if should retrieve InputTypeMode if first time add this by example
	CheckInputTypeModeMapping();

	// Active default tags if not yet
	for (const auto& ActiveTag : InInputTypeModeMapping.DefaultActiveTags)
	{
		ActiveMappingContext(ActiveTag, true);
	}
	return true;
}

bool USSInputContextComponent::RemoveInputTypeModeMapping(const FSSInputTypeModeMapping& InInputTypeModeMapping)
{
	int32 RemovedCount = InputTypeModeMappings.RemoveAll([InInputTypeModeMapping](const FSSInputTypeModeMapping& InputTypeModeMapping)
	{
		return InputTypeModeMapping.HasSameInputTypes(InInputTypeModeMapping);
	});

	UE_LOG(LogSomndusGame, Log, TEXT("[USSInputContextComponent] Removed %d input type mode mapping(s) matching input types from component: %s"),
	       RemovedCount,
	       *GetNameSafe(this));

	return true;
}

bool USSInputContextComponent::CheckInputTypeModeMapping()
{
	for (auto& InputTypeModeMapping : InputTypeModeMappings)
	{
		// if already same ignore it
		if (CurrentInputTypeModeMapping)
		{
			if (CurrentInputTypeModeMapping->InputMappingContextTags.Num() > 0 && CurrentInputTypeModeMapping->HasSameInputTypes(InputTypeModeMapping))
			{
				return false;
			}
		}
		// Store previous before active new one for deactivate previous context later
		if (CurrentInputTypeModeMapping)
		{
			PreviousInputTypeModeMapping = *CurrentInputTypeModeMapping;
		}
		CurrentInputTypeModeMapping = &InputTypeModeMapping;

		return true;
	}
	return false;
}

bool USSInputContextComponent::ActiveMappingContextInfo(const FSSInputMappingContextTag& InputMappingContextTag, bool bForceBindInputAction)
{
	const ULocalPlayer* LocalPlayer = USSInputStaticsLibrary::GetLocalPlayer(this);
	UEnhancedInputLocalPlayerSubsystem* EISubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	
	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;

	for (const auto& InputMappingContext : InputMappingContextTag.InputMappingContexts)
	{
		auto* InputMapping = InputMappingContext.InputMapping.Get();

		if (!EISubsystem->HasMappingContext(InputMapping))
		{
			EISubsystem->AddMappingContext(InputMapping, 0, ModifyContextOptions);

			UE_LOG(LogSomndusInput, Log, TEXT("%s: AddMappingContext : %s"), *GetNameSafe(this), *GetNameSafe(InputMapping));
		}
	}

	// add active tags
	ActiveMappingContextTags.AddUnique(InputMappingContextTag.Tag);

	// store in cache
	bool bNew = true;
	for (auto& ActiveMappingContextInfoTag : ActiveMappingContextInfoTags)
	{
		if (ActiveMappingContextInfoTag.Tag == InputMappingContextTag.Tag)
		{
			ActiveMappingContextInfoTag.InputMappingContexts = InputMappingContextTag.InputMappingContexts;
			bNew = false;
		}
	}
	if (bNew)
	{
		ActiveMappingContextInfoTags.Add(InputMappingContextTag);
		UE_LOG(LogSomndusInput, Log, TEXT("%s: ActiveMappingContextInfoTags : %s"), *GetNameSafe(this), *InputMappingContextTag.Tag.GetTagName().ToString());
	}

	//OnMappingContextTagChange.Broadcast(InputMappingContextTag.Tag, true);

	return true;
}

bool USSInputContextComponent::ActiveMappingContext(const FGameplayTag& Tag, bool bForceBindInputAction)
{
	if (!CurrentInputTypeModeMapping)
	{
		UE_LOG(LogSomndusInput, Warning, TEXT("%s: ActiveMappingContext for %s already active"), *GetNameSafe(this), *Tag.GetTagName().ToString());

		return false;
	}
	for (const auto& InputMappingContextTag : CurrentInputTypeModeMapping->InputMappingContextTags)
	{
		if (InputMappingContextTag.Tag == Tag)
		{
			return ActiveMappingContextInfo(InputMappingContextTag, bForceBindInputAction);
		}
	}
	return false;
}

void USSInputContextComponent::BindInputActions(UInputMappingContext* InputMappingContext)
{
	UE_LOG(LogSomndusInput, Log, TEXT("[USSInputContextComponent] Binding %d input actions from context: %s"),
	       InputMappingContext ? InputMappingContext->GetMappings().Num() : 0,
	       *GetNameSafe(InputMappingContext));

	for (const auto& Mapping : InputMappingContext->GetMappings())
	{
		const UInputAction* InputAction = Mapping.Action;
		// Bind only active contexts tag
		BindInputAction(InputAction, Mapping.Key);
	}
}

void USSInputContextComponent::BindInputAction(const UInputAction* InputAction, FKey Key)
{
	// Bind only active contexts tag
	// If input id value type
	/*
	if (InputAction->ValueType == EInputActionValueType::Axis1D)
	{
		// Add cached input id to key
		for (const auto& Modifier : InputAction->Modifiers)
		{
			if (const UDUInputModifierIdValue* ModifierIdValue = Cast<UDUInputModifierIdValue>(Modifier))
			{
				// Add in map for performance
				InputActionIdMap.Add(InputAction, FDUInputActionIdMap(ModifierIdValue->InputId));
				IdInputActions.Add(ModifierIdValue->InputId, InputAction);

				if (auto* InputKeyListRef = IdInputKeys.Find(ModifierIdValue->InputId))
				{
					InputKeyListRef->Keys.AddUnique(Key);
				}
				else
				{
					FDUInputKeyList InputKeyList;
					InputKeyList.Keys.AddUnique(Key);
					IdInputKeys.Add(ModifierIdValue->InputId, InputKeyList);
				}
			}
		}
	}
	*/
	// Avoid multiple action bind
	if (BoundInputActions.Contains(InputAction))
	{
		return;
	}
	// Bind native axis for native logic
	//NativeAxisManager->BindAction(EIC, InputAction);

	// Bind
	BoundTriggeredInputActions.Add(&EIC->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputActionTrigger));
	BoundStartedInputActions.Add(&EIC->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnInputActionStarted));
	BoundCancelInputActions.Add(&EIC->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::OnInputActionCancel));
	BoundCompletedInputActions.Add(&EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputActionCompleted));

	BoundInputActions.AddUnique(InputAction);
}

void USSInputContextComponent::UnbindInputActions()
{
	for (const auto& TmpInputAction : BoundTriggeredInputActions)
	{
		if (TmpInputAction)
		{
			EIC->RemoveBinding(*TmpInputAction);
		}
	}
	for (const auto& TmpInputAction : BoundStartedInputActions)
	{
		if (TmpInputAction)
		{
			EIC->RemoveBinding(*TmpInputAction);
		}
	}
	for (const auto& TmpInputAction : BoundCancelInputActions)
	{
		if (TmpInputAction)
		{
			EIC->RemoveBinding(*TmpInputAction);
		}
	}
	for (const auto& TmpInputAction : BoundCompletedInputActions)
	{
		if (TmpInputAction)
		{
			EIC->RemoveBinding(*TmpInputAction);
		}
	}
	//InputActionIdMap.Empty();
	BoundTriggeredInputActions.Empty();
	BoundStartedInputActions.Empty();
	BoundCancelInputActions.Empty();
	BoundCompletedInputActions.Empty();
}

void USSInputContextComponent::SetBlockInput(bool bInBlockInput)
{
	bBlockInput = bInBlockInput;
}

void USSInputContextComponent::OnInputActionStarted(const FInputActionInstance& InputActionInstance)
{
	OnInputActionStartedEvent.Broadcast(this, InputActionInstance);
	// Store in pressed input id
	/*
	int32 InputId = -1;
	if (auto* InputActionMap = InputActionIdMap.Find(InputActionInstance.GetSourceAction()))
	{
		InputId = InputActionMap->InputId;
		PressedInputIds.AddUnique(InputId);
	}

	if (InputId > 0)
	{
		if (DU_Input_Log.GetValueOnGameThread())
		{
			UE_LOG(LogSomndusInput, Log, TEXT("%s : Pressed game input id :  %d"), *GetName(), InputId);
		}
	}

	OnGameInputActionPressed.Broadcast(InputActionInstance.GetSourceAction(), InputId);

	// Priority 1 : Command action combo
	if (InputCommandMgr->OnGameInputActionPressed(InputActionInstance.GetSourceAction(), InputId))
	{
		return;
	}
	*/

	// pass if blocked
	if (bBlockInput) return;
	
	// Priority 2 : Active Pawn Input Context
	if (ActivePawnInputContext)
	{
		for (const auto& InputActionHandler : ActivePawnInputContext->GetActiveInputHandlerContexts())
		{
			if (InputActionHandler->InternalInputActionStarted(InputActionInstance))
			{
				return;
			}
		}
	}
}

void USSInputContextComponent::OnInputActionTrigger(const FInputActionInstance& InputActionInstance)
{
	// pass if blocked
	if (bBlockInput) return;
	
	if (ActivePawnInputContext)
	{
		for (const auto& InputActionHandler : ActivePawnInputContext->GetActiveInputHandlerContexts())
		{
			if (InputActionHandler->InternalInputActionTrigger(InputActionInstance))
			{
				break;
			}
		}
	}
}

void USSInputContextComponent::OnInputActionCancel(const FInputActionInstance& InputActionInstance)
{
	// pass if blocked
	if (bBlockInput) return;
	
	if (ActivePawnInputContext)
	{
		for (const auto& InputActionHandler : ActivePawnInputContext->GetActiveInputHandlerContexts())
		{
			if (InputActionHandler->InternalInputActionCancel(InputActionInstance))
			{
				break;
			}
		}
	}
}

void USSInputContextComponent::OnInputActionCompleted(const FInputActionInstance& InputActionInstance)
{
	OnInputActionCompletedEvent.Broadcast(this, InputActionInstance);
	// Store in pressed input id
	/*
	int32 InputId = -1;
	if (auto* InputActionMap = InputActionIdMap.Find(InputActionInstance.GetSourceAction()))
	{
		InputId = InputActionMap->InputId;
		PressedInputIds.Remove(InputId);
	}
	*/

	/*
	if (InputId > 0)
	{
		if (DU_Input_Log.GetValueOnGameThread())
		{
			UE_LOG(LogSomndusInput, Log, TEXT("%s : Released game input id :  %d"), *GetName(), InputId);
		}
	}
	*/

	//OnGameInputActionReleased.Broadcast(InputActionInstance.GetSourceAction(), InputId);

	// pass if blocked
	if (bBlockInput) return;
	
	if (ActivePawnInputContext)
	{
		for (const auto& InputActionHandler : ActivePawnInputContext->GetActiveInputHandlerContexts())
		{
			if (InputActionHandler->InternalInputActionCompleted(InputActionInstance))
			{
				return;
			}
		}
	}
}
