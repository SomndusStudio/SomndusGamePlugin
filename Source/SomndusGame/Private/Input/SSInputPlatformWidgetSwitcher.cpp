// ©2025 Schartier Isaac. All rights reserved.


#include "Input/SSInputPlatformWidgetSwitcher.h"

#include "CommonInputSubsystem.h"
#include "Components/WidgetSwitcher.h"
#include "Core/SSCoreGlobals.h"
#include "Engine/AssetManager.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Input/SSInputLocalPlayerSubsystem.h"
#include "Input/SSInputStaticsLibrary.h"
#include "Kismet/GameplayStatics.h"

void USSInputPlatformWidgetSwitcher::NativeConstruct()
{
	Super::NativeConstruct();

	NativeRefreshLayoutType();
}

void USSInputPlatformWidgetSwitcher::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ResolveDefaultGamepad();
	
	BindEvents();
}

void USSInputPlatformWidgetSwitcher::BindEvents()
{
	if (UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
	{
		InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddUniqueDynamic(this, &ThisClass::OnInputHardwareDeviceChanged);
	}
	
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(USSInputStaticsLibrary::GetLocalPlayer(this)))
	{
		CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
	}
}

void USSInputPlatformWidgetSwitcher::ResolveDefaultGamepad()
{
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(USSInputStaticsLibrary::GetLocalPlayer(this));

	// Default windows fallback
	FName DefaultGamepadName = "XSX";
	
	ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
	// If found valid gamepad
	if (CurrentInputType == ECommonInputType::Gamepad)
	{
		DefaultGamepadName = CommonInputSubsystem->GetCurrentGamepadName();
	}

	NativeRefreshLayoutTypeFromName(DefaultGamepadName);
}

void USSInputPlatformWidgetSwitcher::OnInputHardwareDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	NativeRefreshLayoutType();
}

void USSInputPlatformWidgetSwitcher::HandleInputMethodChanged(ECommonInputType CommonInput)
{
	if (CommonInput == ECommonInputType::Gamepad)
	{
		NativeRefreshLayoutType();
	}
}

void USSInputPlatformWidgetSwitcher::NativeRefreshLayoutType()
{
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(UGameplayStatics::GetPlayerController(this, 0)->GetLocalPlayer());

	// if not gamepad mode don't d oanything
	if (CommonInputSubsystem->GetCurrentInputType() != ECommonInputType::Gamepad)
	{
		return;
	}
	
	FName CurrentGamepadName = CommonInputSubsystem->GetCurrentGamepadName();
	
	NativeRefreshLayoutTypeFromName(CurrentGamepadName);
}

void USSInputPlatformWidgetSwitcher::NativeRefreshLayoutTypeFromName(const FName& GamepadName)
{
	FName CurrentGamepadName = GamepadName;
	
	auto* GamepadWidget = GetGamepadWidgetFromName(CurrentGamepadName);

	if (GamepadWidget)
	{
		GamepadModeSwitcher->SetActiveWidget(GamepadWidget);
	}
	else
	{
		if (ShouldLoadGamepadWidget(CurrentGamepadName))
		{
			StartLoadingGamepadWidget(CurrentGamepadName);
		}
	}
}

void USSInputPlatformWidgetSwitcher::RequestShowLoading_Implementation(bool bShow)
{
	UE_LOG(LogSomndusGame, Log, TEXT("[%s] RequestShowLoading_Implementation"), *GetNameSafe(this));
}

UUserWidget* USSInputPlatformWidgetSwitcher::GetGamepadWidgetFromName(const FName& InGamepadName)
{
	return GamepadWidgets.FindRef(InGamepadName);
}

bool USSInputPlatformWidgetSwitcher::ShouldLoadGamepadWidget(const FName& InGamepadName)
{
	if (LoadingGamepadNames.Contains(InGamepadName))
	{
		return false;
	}

	return true;
}

void USSInputPlatformWidgetSwitcher::OnGamepadWidgetLoaded(FName GamepadName)
{
	// Clean up handle
	ActiveLoadHandles.Remove(GamepadName);
	LoadingGamepadNames.Remove(GamepadName);
	
	// If should unblock input
	if (bBlockInputWhenLoading)
	{
		USSInputStaticsLibrary::GetInputLocalPlayerSubsystem(this)->ResumeInputForUIAnimation();
	}
	RequestShowLoading(false);
	
	FSSInputPlatformWidget InputPlatformWidgetSetting;
	if (!PlatformInputSet->GetValidSettingFromName(GamepadName, InputPlatformWidgetSetting))
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] Loaded widget, but no setting found for gamepad %s"), *GetNameSafe(this), *GamepadName.ToString());
		return;
	}
	
	UClass* LoadedClass = InputPlatformWidgetSetting.WidgetClass.Get();
	if (!LoadedClass)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] Failed to get widget class for gamepad %s after async load"), *GetNameSafe(this), *GamepadName.ToString());
		return;
	}

	// Now you can instantiate the widget, switch UI, etc.
	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this, LoadedClass);
	if (WidgetInstance)
	{
		GamepadWidgets.Add(GamepadName, WidgetInstance);

		// Add to widget switcher and active it
		GamepadModeSwitcher->AddChild(WidgetInstance);
		GamepadModeSwitcher->SetActiveWidget(WidgetInstance);
		
		UE_LOG(LogSomndusGame, Log, TEXT("[%s] Successfully loaded and created widget for %s"), *GetNameSafe(this), *GamepadName.ToString());
	}
}

void USSInputPlatformWidgetSwitcher::OnGamepadWidgetLoaded_Fail(FName Name)
{
	// If should unblock input
	if (bBlockInputWhenLoading)
	{
		USSInputStaticsLibrary::GetInputLocalPlayerSubsystem(this)->ResumeInputForUIAnimation();
	}

	RequestShowLoading(false);
}

bool USSInputPlatformWidgetSwitcher::StartLoadingGamepadWidget(const FName& InGamepadName)
{
	FSSInputPlatformWidget InputPlatformWidgetSetting;
	if (!PlatformInputSet->GetValidSettingFromName(InGamepadName, InputPlatformWidgetSetting))
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] No valid setting found for gamepad %s"), *GetNameSafe(this), *InGamepadName.ToString());
		return false;
	}

	// If should block input
	if (bBlockInputWhenLoading)
	{
		USSInputStaticsLibrary::GetInputLocalPlayerSubsystem(this)->SuspendInputForUIAnimation();
	}
	
	RequestShowLoading(true);
	
	LoadingGamepadNames.AddUnique(InputPlatformWidgetSetting.GamepadName);

	const TSoftClassPtr<UUserWidget> SoftWidgetClass = InputPlatformWidgetSetting.WidgetClass;
	const FSoftObjectPath& AssetRef = SoftWidgetClass.ToSoftObjectPath();

	if (!AssetRef.IsValid())
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] Invalid widget asset reference for gamepad %s"), *GetNameSafe(this), *InGamepadName.ToString());
		OnGamepadWidgetLoaded_Fail(InGamepadName);
		return false;
	}

	// Create (or reuse) streamable manager
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	// Store handle if you need to cancel later or check progress
	TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(
		AssetRef,
		FStreamableDelegate::CreateUObject(this, &USSInputPlatformWidgetSwitcher::OnGamepadWidgetLoaded, InGamepadName),
		FStreamableManager::AsyncLoadHighPriority
	);

	// Store the handle if you need to cancel or track
	ActiveLoadHandles.Add(InGamepadName, Handle);
	
	return true;
}

