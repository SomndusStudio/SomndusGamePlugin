// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSNarrativeTypes.h"
#include "SSContextObject.h"
#include "SSNarrativeNode.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SOMNDUSGAME_API USSNarrativeNode : public USSContextObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	class USSNarrativeManager* Manager;
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanActivate(class USSNarrativeContext* Context) const;

	/**
	 * Called to execute the node.
	 * Once the node is done (immediately or later), it must manually notify the manager.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Execute(USSNarrativeContext* Context, class USSNarrativeManager* InManager);

	void NativeExecute(USSNarrativeContext* Context, class USSNarrativeManager* InManager);
	
	UFUNCTION(BlueprintCallable)
	void ExecuteFinished();
	
	UPROPERTY()
	TArray<FSSNarrativeNodeLink> OutgoingLinks;
	
	virtual class UWorld* GetWorld() const override;

public:
	UPROPERTY()
	FVector2D EditorPosition;

	UPROPERTY(EditAnywhere)
	FLinearColor DefaultEditorNodeColor = FLinearColor(0.4f, 0.4f, 0.4f);
	
	/** Return the editor color of the node (used in graph visual) */
	virtual FLinearColor GetEditorNodeColor() const
	{
		// Default: light grey
		return DefaultEditorNodeColor;
	}
	
	UFUNCTION(BlueprintNativeEvent)
	FText GetDisplayName() const;
};
