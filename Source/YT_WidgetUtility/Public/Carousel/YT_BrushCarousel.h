// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"
#include "YT_BrushCarousel.generated.h"

class UButton;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FYT_OnCarouselIndexChanged, int32, NewIndex);

/**
 * Generic image carousel: holds a list of Slate brushes and displays one at a time,
 * with next/previous navigation that wraps around at the ends. Knows nothing about what
 * the brushes depict — callers supply brushes through SetSlides and observe the active
 * slide through OnIndexChanged. Meant to be subclassed by a WBP that binds DisplayImage
 * and, optionally, the navigation buttons and index label.
 */
UCLASS(Abstract, Blueprintable)
class YT_WIDGETUTILITY_API UYT_BrushCarousel : public UUserWidget
{
	GENERATED_BODY()

//////////
// Core //
//////////
public:
	UYT_BrushCarousel(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

//////////////////////////
// Events and Delegates //
//////////////////////////
public:
	/** Fires whenever the displayed slide changes, carrying the new active index. */
	UPROPERTY(BlueprintAssignable, Category = "Carousel")
	FYT_OnCarouselIndexChanged OnIndexChanged;

///////////////
// Accessors //
///////////////
public:
	/** Returns the index of the slide currently displayed, or 0 when there are no slides. */
	UFUNCTION(BlueprintPure, Category = "Carousel")
	int32 GetIndex() const { return CurrentIndex; }

	/** Returns the number of slides currently held. */
	UFUNCTION(BlueprintPure, Category = "Carousel")
	int32 GetSlideCount() const { return Slides.Num(); }

/////////////
// Methods //
/////////////
public:
	/** Advances to the next slide, wrapping to the first after the last. No-op when empty. */
	UFUNCTION(BlueprintCallable, Category = "Carousel")
	void Next();

	/** Steps to the previous slide, wrapping to the last before the first. No-op when empty. */
	UFUNCTION(BlueprintCallable, Category = "Carousel")
	void Previous();

	/** Jumps to NewIndex, wrapping it into range, then refreshes the display. No-op when empty. */
	UFUNCTION(BlueprintCallable, Category = "Carousel")
	void SetIndex(int32 NewIndex);

	/** Replaces all slides, resets the active index to 0, and refreshes the display. */
	UFUNCTION(BlueprintCallable, Category = "Carousel")
	void SetSlides(const TArray<FSlateBrush>& InSlides);

protected:
	/** Bound to NextButton's OnClicked; forwards to Next. */
	UFUNCTION()
	void HandleNextClicked();

	/** Bound to PrevButton's OnClicked; forwards to Previous. */
	UFUNCTION()
	void HandlePrevClicked();

	/** Pushes the active slide to DisplayImage, updates IndexText and nav state, and broadcasts OnIndexChanged. */
	void RefreshDisplay();

////////////////
// Components //
////////////////
protected:
	/** Shows the active slide's brush. Blank and hidden when there are no slides. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DisplayImage;

	/** Optional label showing the active position, e.g. "2 / 5". Empty when there are no slides. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> IndexText;

	/** Optional button that advances to the next slide. Disabled when fewer than two slides. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> NextButton;

	/** Optional button that steps to the previous slide. Disabled when fewer than two slides. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> PrevButton;

/////////////
// Runtime //
/////////////
private:
	/** Index of the slide currently displayed. */
	int32 CurrentIndex;

	/** Brushes to cycle through, one per slide. */
	TArray<FSlateBrush> Slides;
};
