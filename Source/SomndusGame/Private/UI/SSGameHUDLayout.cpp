// ©2025 Schartier Isaac. All rights reserved.


#include "UI/SSGameHUDLayout.h"


#include "CommonLocalPlayer.h"
#include "CommonUIExtensions.h"
#include "NativeGameplayTags.h"
#include "PrimaryGameLayout.h"
#include "Helper/SSHelperStatics.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/SSGameMessagingSubsystem.h"

#define LOCTEXT_NAMESPACE "FSomndusGameModule"

UE_DEFINE_GAMEPLAY_TAG(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

namespace DUGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_GamePlayer, "UI.Layer.GamePlayer");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_GameOverlay, "UI.Layer.GameOverlay");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_ModeMenu, "UI.Layer.ModeMenu");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_Dialogue, "UI.Layer.Dialogue");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_Cinematic, "UI.Layer.Cinematic");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_CommonScreen, "UI.Layer.CommonScreen");
	UE_DEFINE_GAMEPLAY_TAG(Tag_UI_Layer_Modal, "UI.Layer.Modal");
}

bool USSGameHUDLayout::TryDeactivateLoadingWidget(FGameplayTag LoadingType)
{
	if (auto WidgetRef = LoadingWidgets.Find(LoadingType))
	{
		UCommonActivatableWidget* LoadingWidget = *WidgetRef;
		LoadingWidget->DeactivateWidget();

		return true;
	}

	return false;
}

void USSGameHUDLayout::OnLoadingTokenChange(USSGameMessagingSubsystem* GameMessagingSubsystem, FGameplayTag LoadingType, bool bRemoved)
{
	// if should remove
	if (bRemoved)
	{
		TryDeactivateLoadingWidget(LoadingType);
		LoadingWidgets.Remove(LoadingType);
	}
	else
	{   FSSLoadingInformationTokenInfo TokenInfo;
		bool bHaveOne = GameMessagingSubsystem->FindBetterLoadingInfo(LoadingType, TokenInfo);
		// if not have one try remove widget
		if (!bHaveOne)
		{
			TryDeactivateLoadingWidget(LoadingType);
			LoadingWidgets.Remove(LoadingType);
		}
		// Type push/update
		else
		{
			if (auto WidgetClass = LoadingWidgetClasses.Find(LoadingType))
			{
				if (UCommonLocalPlayer* LocalPlayer = GetOwningLocalPlayer<UCommonLocalPlayer>())
				{
					if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
					{
						auto LoadingWidgetClassPtr = USSHelperStatics::TryGetClass(*WidgetClass);

						auto* Descriptor = CreateLoadingDescriptor(TokenInfo.Message);
						
						GlobalLoadingResultCallback = FCommonMessagingResultDelegate::CreateUObject(this, &ThisClass::HandleGlobalLoadingResult);
						
						auto LoadingWidget = RootLayout->PushWidgetToLayerStack<UCommonGameDialog>(SSGameplayTags::TAG_SS_LAYER_MODAL, LoadingWidgetClassPtr, [&, Descriptor](UCommonGameDialog& Dialog) {
							Dialog.SetupDialog(Descriptor, GlobalLoadingResultCallback);
						});

						LoadingWidgets.Add(LoadingType, LoadingWidget);
					}
				}
			}
		}
	}
}

void USSGameHUDLayout::HandleGlobalLoadingResult(ECommonMessagingResult CommonMessagingResult)
{
	UE_LOG(LogTemp, Log, TEXT("Token Loading dialog closed"));
}

UCommonGameDialogDescriptor* USSGameHUDLayout::CreateLoadingDescriptor(const FText& Body)
{
	UCommonGameDialogDescriptor* Descriptor = NewObject<UCommonGameDialogDescriptor>();
	Descriptor->Header = LOCTEXT("Loading", "Loading");
	Descriptor->Body = Body;

	return Descriptor;
}

void USSGameHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));

	// Listen loading token
	USSGameMessagingSubsystem* Subsystem = GetOwningLocalPlayer()->GetSubsystem<USSGameMessagingSubsystem>();
	Subsystem->OnLoadingTokenChange.AddUniqueDynamic(this, &ThisClass::OnLoadingTokenChange);
}

bool USSGameHUDLayout::CanPerformEscapeAction_Implementation()
{
	return true;
}

void USSGameHUDLayout::HandleEscapeAction()
{
	if (ensure(!EscapeMenuClass.IsNull()) && CanPerformEscapeAction())
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), DUGameplayTags::Tag_UI_Layer_ModeMenu, EscapeMenuClass);
	}
}

#undef LOCTEXT_NAMESPACE