#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "YT_ButtonBase.generated.h"

class UTextBlock;

/**
 * Base CommonUI button for YT_WidgetUtility plugin
 * Subclass in Blueprint to build the visual tree (background, optional ButtonLabel text block)
 * and assign a UCommonButtonStyle to Style; reuse the same class/subclass across buttons rather
 * than authoring styling per instance.
 */
UCLASS()
class YT_WIDGETUTILITY_API UYT_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYT_ButtonBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;

////////////////
// Components //
////////////////
protected:
	/** Optional text label; SetButtonText no-ops if the Blueprint doesn't bind this. */
	UPROPERTY(BlueprintReadOnly, Category = "Button", meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ButtonLabel;

////////////////
// Attributes //
////////////////
public:
	/** Text shown on the button label. Applied automatically on construct, including in the UMG designer preview. Change at runtime via SetButtonText, not by setting this directly, so ButtonLabel stays in sync. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	FText ButtonText;

/////////////
// Methods //
/////////////
public:
	/** Sets the button's label text, if this Blueprint binds a ButtonLabel text block. */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& InText);
};
