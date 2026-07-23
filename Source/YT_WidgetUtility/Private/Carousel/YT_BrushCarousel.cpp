// Copyright Epic Games, Inc. All Rights Reserved.

#include "Carousel/YT_BrushCarousel.h"
#include "YT_WidgetUtilityLogging.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// CORE

UYT_BrushCarousel::UYT_BrushCarousel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentIndex = 0;
}

void UYT_BrushCarousel::NativeConstruct()
{
	Super::NativeConstruct();

	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UYT_BrushCarousel::HandleNextClicked);
	}

	if (PrevButton)
	{
		PrevButton->OnClicked.AddDynamic(this, &UYT_BrushCarousel::HandlePrevClicked);
	}

	RefreshDisplay();
}

// METHODS

void UYT_BrushCarousel::Next()
{
	const int32 Count = Slides.Num();

	if (Count > 0)
	{
		SetIndex((CurrentIndex + 1) % Count);
	}
}

void UYT_BrushCarousel::Previous()
{
	const int32 Count = Slides.Num();

	if (Count > 0)
	{
		SetIndex((CurrentIndex - 1 + Count) % Count);
	}
}

void UYT_BrushCarousel::SetIndex(int32 NewIndex)
{
	const int32 Count = Slides.Num();

	if (Count > 0)
	{
		CurrentIndex = ((NewIndex % Count) + Count) % Count;
		UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("%s set carousel index to %d of %d."), *GetName(), CurrentIndex, Count);
		RefreshDisplay();
	}
}

void UYT_BrushCarousel::SetSlides(const TArray<FSlateBrush>& InSlides)
{
	Slides = InSlides;
	CurrentIndex = 0;
	UE_LOG(LogYT_WidgetUtility, Verbose, TEXT("%s received %d carousel slides."), *GetName(), Slides.Num());
	RefreshDisplay();
}

void UYT_BrushCarousel::HandleNextClicked()
{
	Next();
}

void UYT_BrushCarousel::HandlePrevClicked()
{
	Previous();
}

void UYT_BrushCarousel::RefreshDisplay()
{
	const int32 Count = Slides.Num();

	if (DisplayImage)
	{
		if (Count > 0)
		{
			DisplayImage->SetBrush(Slides[CurrentIndex]);
			DisplayImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			DisplayImage->SetBrush(FSlateBrush());
			DisplayImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IndexText)
	{
		if (Count > 0)
		{
			IndexText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentIndex + 1, Count)));
		}
		else
		{
			IndexText->SetText(FText::GetEmpty());
		}
	}

	const bool bCanNavigate = Count > 1;

	if (NextButton)
	{
		NextButton->SetIsEnabled(bCanNavigate);
	}

	if (PrevButton)
	{
		PrevButton->SetIsEnabled(bCanNavigate);
	}

	OnIndexChanged.Broadcast(CurrentIndex);
}
