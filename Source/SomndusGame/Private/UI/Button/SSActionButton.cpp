// ©2025 Schartier Isaac. All rights reserved.


#include "UI/Button/SSActionButton.h"

#include "CommonActionWidget.h"
#include "Components/TextBlock.h"

void USSActionButton::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();
	
	if (InputActionWidget) //optional bound widget
	{
		FText ActionDisplayName = InputActionWidget->GetDisplayText();

		if (Text_ActionName)
		{
			Text_ActionName->SetText(ActionDisplayName);
		}
	}
}