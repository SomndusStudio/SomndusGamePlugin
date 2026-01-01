/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "SSButtonText.h"
#include "SSButtonIcon.generated.h"

class UImage;

/**
 * USSButtonIcon
 *
 * A button widget class that extends USSButtonText to include optional icon display functionality.
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
