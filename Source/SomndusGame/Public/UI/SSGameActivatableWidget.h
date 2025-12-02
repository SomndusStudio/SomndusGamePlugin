/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SSCommonUITypes.h"
#include "Engine/DataTable.h"
#include "SSGameActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class ESSWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};
/**
 * 
 */
UCLASS()
class SOMNDUSGAME_API USSGameActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	
	USSGameActivatableWidget();
	
	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

	FString GetInputToken();
	virtual void NativeOnActivated() override;
	
	UFUNCTION()
	void OnDelayDeactivatedFinished();
	
	virtual void NativeOnDeactivated() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditDefaultsOnly)
	bool bUseAutoShowAnimation = true;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWidget> LastFocusedWidget = nullptr;
	
protected:

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> ShowAnimation;
	
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ESSWidgetInputMode InputConfig = ESSWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	bool bShouldStoreLastFocusedWidget = false;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterBinding(FDataTableRowHandle InputAction, const FSSInputActionExecutedDelegate& Callback, FUIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable)
	void UnregisterBinding(FUIActionBindingHandle BindingHandle);

	UFUNCTION(BlueprintCallable)
	void UnregisterAllBindings();

	UFUNCTION(BlueprintCallable)
	void StoreLastFocusedWidget();

	UFUNCTION(BlueprintCallable)
	void StoreAsLastFocusedWidget(UWidget* InWidget);
	
private:

	UPROPERTY()
	TArray<FUIActionBindingHandle> BindingHandles;
};