// © 2024 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SSButtonGamepadInteraction.generated.h"



/**
* Button class used for multiple purposes:

- Determine whether to trigger the selection event when the controller takes focus.
- Automatically switch the triggering input action in real-time for the UX footer.

 */
UCLASS()
class SOMNDUSGAME_API USSButtonGamepadInteraction : public UCommonButtonBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bHoverPerformWithGamepad = false;
	
	UPROPERTY(BlueprintReadOnly, Category = Input)
	int32 TriggeringInputActionIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	TArray<FDataTableRowHandle> ConditionalTriggeringInputActions;
	
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

	UFUNCTION(BlueprintCallable)
	void SetTriggeringActionIndex(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void ClearTriggeringAction();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateTriggeringAction();
};
