// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"
#include "../MyPlayerController.h"
#include "../UI/PlayerInfoWidget.h"
#include "../UI/MinimapWidget.h"
#include "../UI/MapWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"
#include "../Character_Master.h"

#include "Components/Image.h"
#include "Components/Throbber.h"
#include "Kismet/KismetMathLibrary.h"

UPointOfInterestWidget::UPointOfInterestWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UActorComponent> PointOfInterestComponentBPClass(TEXT("/Game/MiniMap/BP_PointOfInterestComponent"));
	if (PointOfInterestComponentBPClass.Succeeded())
	{
		PointOfInterestComponentClass = PointOfInterestComponentBPClass.Class;

		//EquippedItem->Get
	}
}

void UPointOfInterestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Owner != nullptr)
	{
		auto Player = Cast<ACharacter_Master>(Owner);
		if (Player != nullptr)
		{
			if (Player->bDead == true)
			{
				RemoveFromParent();
			}
		}


	}
	if (IsMinimap == true)
	{
		MinimapCoord();
	}
	else
	{
		MapCoord();
	}
}


void UPointOfInterestWidget::CustomInitialize(AActor* NewOwner, bool NewIsStatic, bool NewIsMinimap)
{
	IsMinimap = NewIsMinimap;
	Owner = NewOwner;
	IsStatic = NewIsStatic;
	UActorComponent*  Comp = Owner->GetComponentByClass(UPointOfInterestComponent::StaticClass());//걍bp말고 일반찾아도,,ㅋㅋ
	ABCHECK(Comp != nullptr);
	auto PointOfInterest = Cast<UPointOfInterestComponent>(Comp);
	ABCHECK(PointOfInterest!=nullptr);

	if (PointOfInterest->IconImage != nullptr)
	{
		CustomImage->SetBrushFromTexture(PointOfInterest->IconImage);
		CustomImage->SetVisibility(ESlateVisibility::Visible);
		//DefaultImage->SetVisibility(ESlateVisibility::Hidden);

	}
	else
	{
		CustomImage->SetVisibility(ESlateVisibility::Hidden);
		//DefaultImage->SetVisibility(ESlateVisibility::Visible);
	}

	SetColor();

}

float UPointOfInterestWidget::FindAngle(FVector2D A, FVector2D B)
{
	float DeltaX = A.X - B.X;
	float DeltaY = A.Y - B.Y;
	float angle = UKismetMathLibrary::DegAtan2(DeltaY, DeltaX);
	return angle;
}
FVector2D UPointOfInterestWidget::FindCoord(float Radius, float Angle)
{
	float NewRadius = Radius;
	if(IsMinimap)
		NewRadius = UKismetMathLibrary::Clamp(Radius, 0., 130.);
	float CoordX = (-1)* NewRadius* UKismetMathLibrary::DegSin(Angle);
	float CoordY = (-1) * NewRadius * UKismetMathLibrary::DegCos(Angle);
	FVector2D Out = FVector2D(CoordX, CoordY);
	return Out;
}

void UPointOfInterestWidget::MapCoord()
{
	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABCHECK(Controller != nullptr);
	auto MyController = Cast< AMyPlayerController>(Controller);
	ABCHECK(MyController != nullptr);
	if (MyController->PlayerInfoHUDWidget != nullptr)
	{
		auto MinimapWidget = MyController->PlayerInfoHUDWidget->Minimap_Widget;
		auto MapWidget = MyController->PlayerInfoHUDWidget->MapWidget;
		if (MapWidget != nullptr)
		{
			float MapDimensions = 35000 / 1000;

			auto Target = MyController->GetPawn();
			ABCHECK(Target != nullptr);



			//auto TargetLoc = Target->GetActorLocation();
			auto TargetLoc = FVector(0,0,0);
			auto OwnerLoc = Owner->GetActorLocation();
			float deltaX = (- OwnerLoc.X) / MapDimensions;
			float deltaY = -1 * (- OwnerLoc.Y) / MapDimensions;
			//UE_LOG(LogTemp, Warning, TEXT("delta x : %f, delta y : %f"), deltaX, deltaY);
			FVector2D Point = FVector2D(deltaX, deltaY);
			FVector2D PlayerPoint = FVector2D(0, 1);

			float Angle = FindAngle(PlayerPoint, Point);
			float Radius = Point.Size();
			//UE_LOG(LogTemp, Warning, TEXT("%f"),  Angle);
			FVector2D CoordVec = FindCoord(Radius, Angle);
			SetRenderTranslation(CoordVec);
			//DefaultImage->SetVisibility(ESlateVisibility::Visible);

		}
	}

}

void UPointOfInterestWidget::MinimapCoord()
{

	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABCHECK(Controller != nullptr);
	auto MyController = Cast< AMyPlayerController>(Controller);
	ABCHECK(MyController != nullptr);
	if (MyController->PlayerInfoHUDWidget != nullptr)
	{
		auto MinimapWidget = MyController->PlayerInfoHUDWidget->Minimap_Widget;
		if (MinimapWidget != nullptr)
		{
			float MapDimensions = MinimapWidget->Zoom * (MinimapWidget->Dimensions / 300);

			auto Target = MyController->GetPawn();
			ABCHECK(Target != nullptr);



			auto TargetLoc = Target->GetActorLocation();
			auto OwnerLoc = Owner->GetActorLocation();
			float deltaX = (TargetLoc.X - OwnerLoc.X) / MapDimensions;
			float deltaY = -1 * (TargetLoc.Y - OwnerLoc.Y) / MapDimensions;
			FVector2D Point = FVector2D(deltaX, deltaY);
			FVector2D PlayerPoint = FVector2D(0, 0);

			float Angle = FindAngle(PlayerPoint, Point);
			float Radius = Point.Size();
			//UE_LOG(LogTemp, Warning, TEXT("%f"),  Angle);
			FVector2D CoordVec = FindCoord(Radius, Angle);
			SetRenderTranslation(CoordVec);
			if (IsStatic == false)
			{
				if (CoordVec.Size() >= 129.)
				{
					//DefaultImage->SetVisibility(ESlateVisibility::Hidden);//이미지 있으면 이미지로 해야됨
					CustomImage->SetVisibility(ESlateVisibility::Hidden);//이미지 있으면 이미지로 해야됨

				}
				else
				{
					//DefaultImage->SetVisibility(ESlateVisibility::Visible);
					CustomImage->SetVisibility(ESlateVisibility::Visible);//이미지 있으면 이미지로 해야됨
				}
			}

		}
	}
}