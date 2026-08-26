#include "Settings/YT_SettingWidget_Audio.h"
#include "YT_WidgetUtilityLogging.h"

#include "AudioSlider.h"
#include "Components/TextBlock.h"
#include "GameUserSettings/Definitions/YT_SettingDef_Numeric.h"

void UYT_SettingWidget_Audio::NativeConstruct()
{
	Super::NativeConstruct();

	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		if (AudioSlider)
		{
			// UAudioSlider uses 0-1 range for the normalized slider position (Value)
			// But it maps this to an output value (usually dB for volume)
			// We want to set the slider position based on our 0-1 volume setting
			AudioSlider->Value = NumericDef->GetPendingValue();
			AudioSlider->OnValueChanged.RemoveDynamic(this, &UYT_SettingWidget_Audio::OnSliderValueChanged);
			AudioSlider->OnValueChanged.AddDynamic(this, &UYT_SettingWidget_Audio::OnSliderValueChanged);
		}
	}
}

void UYT_SettingWidget_Audio::NativeDestruct()
{
	if (AudioSlider)
	{
		AudioSlider->OnValueChanged.RemoveDynamic(this, &UYT_SettingWidget_Audio::OnSliderValueChanged);
	}
	Super::NativeDestruct();
}

void UYT_SettingWidget_Audio::UpdateWidgetVisuals_Implementation()
{
	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		float PendingValue = NumericDef->GetPendingValue();
		if (AudioSlider)
		{
			AudioSlider->Value = PendingValue;
		}

		if (ValueText)
		{
			// Display as percentage for volume (0.0 to 1.0 -> 0% to 100%)
			int32 PercentValue = FMath::RoundToInt(PendingValue * 100.0f);
			ValueText->SetText(FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(PercentValue)));
		}
	}
}

void UYT_SettingWidget_Audio::OnSliderValueChanged(float NewValue)
{
	if (UYT_SettingDef_Numeric* NumericDef = Cast<UYT_SettingDef_Numeric>(SettingDefinition))
	{
		// NewValue from UAudioSlider is already the normalized 0-1 value (the 'Value' property)
		UE_LOG(LogYT_WidgetUtility_Settings, Verbose, TEXT("%s audio slider value changed to %f."), *GetName(), NewValue);
		NumericDef->SetPendingValue(NewValue);
	}
}

UWidget* UYT_SettingWidget_Audio::GetFocusTarget() const
{
	return AudioSlider;
}
