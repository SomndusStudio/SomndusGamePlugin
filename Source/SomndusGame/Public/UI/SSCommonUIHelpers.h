// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "CommonActivatableWidget.h"
#include "PrimaryGameLayout.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include  "SSCommonUIHelpers.generated.h"

UCLASS()
class SOMNDUSGAME_API USSCommonUIHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Delegate type flexible pour gérer tous les états async de PushWidgetToLayerStackAsync.
	 */
	using FWidgetLayerAsyncCallback = TFunction<void(EAsyncWidgetLayerState State, UCommonActivatableWidget* Widget)>;
	 
	/**
	 * Pousse un widget sur la couche UI et exécute la callback pour tous les états (Initialize, AfterPush, Canceled)
	 * dans une seule invocation.
	 * @param PlayerController Propriétaire du widget.
	 * @param WidgetClass Widget à instancier (TSoftClassPtr)
	 * @param LayerName Tag de la couche UI.
	 * @param bSuspendInputUntilComplete Suspend input pendant push si true.
	 * @param Callback Fonction lambda ou delegate appelée à chaque état.
	 * @return Handle du streamable pour gestion ultérieure (annulation, etc).
	 */
	static TSharedPtr<FStreamableHandle> PushWidgetWithAllStatesCallback(
		APlayerController* PlayerController,
		TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
		FGameplayTag LayerName,
		bool bSuspendInputUntilComplete,
		FWidgetLayerAsyncCallback Callback
	);
};