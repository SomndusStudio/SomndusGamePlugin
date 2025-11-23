// ©2025 Schartier Isaac. All rights reserved.


#include "UI/Button/SSCommonButton.h"

#include "CommonUISubsystemBase.h"
#include "TimerManager.h"
#include "Animation/UMGSequencePlayer.h"
#include "Input/SSInputStaticsLibrary.h"

namespace SSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Tag_Input_Token_UI_Button, "Input.Token.UIButton");
}

void USSCommonButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClickAnimation)
	{
		ClickAnimEndDelegate.BindDynamic(this, &ThisClass::OnClickFXFinished);
		BindToAnimationFinished(ClickAnimation, ClickAnimEndDelegate);
	}
}

void USSCommonButton::NativeOnClicked()
{
	if (!GetLocked())
	{
		BP_OnClicked();
		if (bUseClickFXDelay)
		{
			if (bSuspendInputClickFX)
			{
				USSInputStaticsLibrary::SetGameSuspendInput(this, SSGameplayTags::Tag_Input_Token_UI_Button, true);
			}
			// Only if we want handle the fx finished custom (for animation or ignore set timer cause game paused)
			if (bUseClickAnimation)
			{
				if (ClickAnimation)
				{
					PlayAnimationForward(ClickAnimation);
				}
			}
			else
			{
				// Simulate wait ClickFXDelay before trigger on click event for UX Polish Feedback
				GetWorld()->GetTimerManager().SetTimer(
					ClickFXHandle, this, &ThisClass::OnClickFXFinished, ClickFXDelay, false);
			}
		}
		else
		{
			OnClickFXFinished();
		}
	}
	else
	{
		BP_OnLockClicked();
		OnLockClicked().Broadcast();
	}
}

void USSCommonButton::OnClickFXFinished()
{
	if (bUseClickFXDelay)
	{
		if (bSuspendInputClickFX)
		{
			USSInputStaticsLibrary::SetGameSuspendInput(this, SSGameplayTags::Tag_Input_Token_UI_Button, false);
		}
	}

	OnClicked().Broadcast();
	BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::ClickEvent);
	if (OnButtonBaseClicked.IsBound())
	{
		OnButtonBaseClicked.Broadcast(this);
	}
		
	FString ButtonName, ABTestName, ExtraData;
	if (GetButtonAnalyticInfo(ButtonName, ABTestName, ExtraData))
	{
		UCommonUISubsystemBase* CommonUISubsystem = GetUISubsystem();
		if (GetGameInstance())
		{
			check(CommonUISubsystem);

			CommonUISubsystem->FireEvent_ButtonClicked(ButtonName, ABTestName, ExtraData);
		}
	}
}