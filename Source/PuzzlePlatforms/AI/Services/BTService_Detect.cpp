// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "../NPCAIController.h"
#include "../../PuzzlePlatformsCharacter.h"
#include "../NPC_Archer.h"

#include"BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{

	NodeName = TEXT("Detect");
	Interval = 1.0f;
}
//�� �ʸ��� ��� ���
//������ �������� ������ ���� ���� �ӵ� ����
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ANPCAIController::IsHitKey) == true)
		return;
	APawn* ControllingPawn =OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();//����ġ
	float DetectRadius = 600.f;

	if (World == nullptr) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(OverlapResults,
		Center,FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
		);//channel objecttype profile



	if (bResult)
	{

		//if (OverlapResults.Num() == 0)//���� �ƹ��� ������
		//{
		//	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, nullptr);
		//	UE_LOG(LogTemp, Warning, TEXT("Nullptr"));
		//	return;
		//}
		APuzzlePlatformsCharacter* Target = nullptr;
		for (auto const& OverlapResult : OverlapResults)//�̹� �����ϸ� �·� ��Ӱ�
		{
			auto Current = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
			//bool same = false;
			APuzzlePlatformsCharacter* Player = Cast< APuzzlePlatformsCharacter>(OverlapResult.GetActor());

			if (Player && Player->GetController()->IsPlayerController())
			{
				if (Current == Player)//�ϴ� nullptr�� �ƴѰ� ������ Ȯ��
				{
					//���� player�� ���� �ȿ� ����
					//UE_LOG(LogTemp, Warning, TEXT("Player is still in the Range"));
					return;
				}
				if (Target == nullptr)//ó�� �ɸ� �ָ�
					Target = Player;
				auto Archer = Cast<ANPC_Archer>(ControllingPawn);
				if (Archer != nullptr)
				{
					Archer->NetMulticast_SetTarget(Target);
				}
				
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Player->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(),
					Player->GetActorLocation(), FColor::Blue, false, 0.2f);
				
			}
		}

		if (Target != nullptr)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, Target);
			//UE_LOG(LogTemp, Warning, TEXT("Go to the Before Player"));
			return;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(ANPCAIController::TargetKey);
		}

	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);


	//UE_LOG(LogTemp, Warning, TEXT("Setting Nullptr"));
	


}

