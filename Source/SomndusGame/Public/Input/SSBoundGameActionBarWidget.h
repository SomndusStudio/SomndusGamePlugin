// Copyright (C) 2020-2023 Schartier Isaac

#pragma once

#include "CoreMinimal.h"
#include "SSBoundGameActionBar.h"
#include "Blueprint/UserWidget.h"
#include "SSBoundGameActionBarWidget.generated.h"

UCLASS()
class SOMNDUSGAME_API USSBoundGameActionBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;

	void SetupGameActions();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SomndusGame")
	bool bPreConstructActions = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SomndusGame")
	TArray<FSSBoundGameActionInfo> GameActions;
	
private:
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USSBoundGameActionBar> BoundGameActionBar;
};
