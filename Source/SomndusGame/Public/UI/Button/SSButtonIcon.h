// ©2025 Schartier Isaac. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SSButtonText.h"
#include "SSButtonIcon.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSButtonIcon : public USSButtonText
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetIcon(UTexture2D* InIcon);

	UFUNCTION(BlueprintCallable)
	void SetShowIcon(bool InShowIcon);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowIcon = true;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UImage> IconImage;
};
