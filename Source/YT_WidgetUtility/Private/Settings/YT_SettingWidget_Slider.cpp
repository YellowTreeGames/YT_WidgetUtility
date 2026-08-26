#include "Settings/YT_SettingWidget_Slider.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameUserSettings/Definitions/YT_SettingDef_Numeric.h"

void UYT_SettingWidget_Slider::NativeConstruct()
{
	Super::NativeConstruct();

	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		if (SettingSlider)
		{
			SettingSlider->SetMinValue(NumericDef->MinValue);
			SettingSlider->SetMaxValue(NumericDef->MaxValue);
			SettingSlider->SetStepSize(NumericDef->StepSize);
			SettingSlider->SetValue(NumericDef->GetPendingValue());
			SettingSlider->OnValueChanged.RemoveDynamic(this, &UYT_SettingWidget_Slider::OnSliderValueChanged);
			SettingSlider->OnValueChanged.AddDynamic(this, &UYT_SettingWidget_Slider::OnSliderValueChanged);
		}
	}
}

void UYT_SettingWidget_Slider::NativeDestruct()
{
	if (SettingSlider)
	{
		SettingSlider->OnValueChanged.RemoveDynamic(this, &UYT_SettingWidget_Slider::OnSliderValueChanged);
	}
	Super::NativeDestruct();
}

void UYT_SettingWidget_Slider::UpdateWidgetVisuals_Implementation()
{
	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		float PendingValue = NumericDef->GetPendingValue();
		if (SettingSlider)
		{
			SettingSlider->SetValue(PendingValue);
		}

		if (ValueText)
		{
			if (NumericDef->bIsInteger)
			{
				ValueText->SetText(FText::AsNumber(static_cast<int32>(PendingValue)));
			}
			else
			{
				ValueText->SetText(FText::AsNumber(PendingValue));
			}
		}
	}
}

void UYT_SettingWidget_Slider::OnSliderValueChanged(float NewValue)
{
	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		float SteppedValue = NewValue;
		if (NumericDef->StepSize > 0.0f)
		{
			SteppedValue = FMath::RoundToFloat(NewValue / NumericDef->StepSize) * NumericDef->StepSize;
		}

		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("%s slider value changed to %f."), *GetName(), SteppedValue);
		NumericDef->SetPendingValue(SteppedValue);

		// If we snapped to a step, update the slider position and text
		if (SteppedValue != NewValue)
		{
			UpdateWidgetVisuals();
		}
	}
}
