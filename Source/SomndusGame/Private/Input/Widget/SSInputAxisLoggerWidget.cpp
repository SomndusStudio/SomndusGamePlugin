// ©2025 Schartier Isaac. All rights reserved.


#include "Input/Widget/SSInputAxisLoggerWidget.h"

#include "SSLog.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Input/Context/SSInputAxisMgr.h"
#include "Input/Context/SSInputContextComponent.h"
#include "Kismet/GameplayStatics.h"

void USSInputAxisLoggerWidget::SetInputAxisMgr(USSInputAxisMgr* InMgr)
{
	InputAxisMgr = InMgr;
}

void USSInputAxisLoggerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Capture player
	auto* InputContextComponent = GetOwningLocalPlayer()->GetPlayerController(GetWorld())->FindComponentByClass<USSInputContextComponent>();
	if (!InputContextComponent)
	{
		UE_LOG(LogSomndusGame, Warning, TEXT("[%s] No InputContextComponent found"), *GetNameSafe(this));
		return;
	}
	
	SetInputAxisMgr(InputContextComponent->GetInputAxisMgr());
}

void USSInputAxisLoggerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!InputAxisMgr || !AnalogStickDot || !AnalogOverlay)
		return;

	// Normalisation du mouvement de -1 à 1 -> -OverlaySize/2 à +OverlaySize/2
	const FVector2D Input = InputAxisMgr->CachedMoveInput;
	
	const FVector2D DotPos = FVector2D(Input.X * MaxRange, -Input.Y * MaxRange); // Y inversé pour correspondre à l'écran

	// On positionne le point du stick au bon endroit
	UCanvasPanelSlot* DotSlot = Cast<UCanvasPanelSlot>(AnalogStickDot->Slot);
	if (DotSlot)
	{
		DotSlot->SetPosition((DotPos - DotCenterOffset) + FVector2D(MaxRange, MaxRange)); // Centré dans l'overlay
	}

	// Mettre à jour le texte
	if (MoveInputText)
	{
		MoveInputText->SetText(FText::FromString(FString::Printf(TEXT("Move: X=%.2f Y=%.2f"), Input.X, Input.Y)));
	}

	if (InputAngleText)
	{
		InputAngleText->SetText(FText::FromString(FString::Printf(TEXT("Angle: %.1f°"), InputAxisMgr->InputAngleDegrees)));
	}
}
