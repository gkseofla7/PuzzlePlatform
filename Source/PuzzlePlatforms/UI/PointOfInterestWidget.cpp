// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestWidget.h"
#include "../PlayersComponent/PointOfInterestComponent.h"
#include "../MyPlayerController.h"
#include "../UI/PlayerInfoWidget.h"
#include "../UI/MinimapWidget.h"

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
	if (Owner == nullptr)
	{
		RemoveFromParent();
	}
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
			float deltaX = (TargetLoc.X - OwnerLoc.X)/MapDimensions;
			float deltaY = -1*(TargetLoc.Y - OwnerLoc.Y)/MapDimensions;
			FVector2D Point = FVector2D(deltaX, deltaY);
			FVector2D PlayerPoint = FVector2D(0, 0);

			float Angle = FindAngle(PlayerPoint, Point);
			float Radius = Point.Size();
			//UE_LOG(LogTemp, Warning, TEXT("%f"),  Angle);
			FVector2D CoordVec = FindCoord(Radius, Angle);
			SetRenderTranslation(CoordVec);
			if (IsStatic== false)
			{
				if (CoordVec.Size() >= 129.)
				{
					DefaultImage->SetVisibility(ESlateVisibility::Hidden);//이미지 있으면 이미지로 해야됨
				}
				else
				{

					DefaultImage->SetVisibility(ESlateVisibility::Visible);
				}
			}

		}
	}
}

void UPointOfInterestWidget::CustomInitialize(AActor* NewOwner, bool NewIsStatic)
{
	Owner = NewOwner;
	IsStatic = NewIsStatic;
	UActorComponent*  Comp = Owner->GetComponentByClass(PointOfInterestComponentClass);//걍bp말고 일반찾아도,,ㅋㅋ
	ABCHECK(Comp != nullptr);
	auto PointOfInterest = Cast<UPointOfInterestComponent>(Comp);
	ABCHECK(PointOfInterest!=nullptr);
	if (PointOfInterest->IconImage != nullptr)
	{
		CustomImage->SetBrushFromTexture(PointOfInterest->IconImage);
		DefaultImage->SetVisibility(ESlateVisibility::Hidden);

	}
	else
	{
		CustomImage->SetVisibility(ESlateVisibility::Hidden);
	}

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
	float NewRadius = UKismetMathLibrary::Clamp(Radius, 0., 130.);
	float CoordX = (-1)* NewRadius* UKismetMathLibrary::DegSin(Angle);
	float CoordY = (-1) * NewRadius * UKismetMathLibrary::DegCos(Angle);
	FVector2D Out = FVector2D(CoordX, CoordY);
	return Out;
}