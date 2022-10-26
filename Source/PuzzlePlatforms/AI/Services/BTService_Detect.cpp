// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "../NPCAIController.h"
#include "../../Character_Master.h"
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
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ANPCAIController::IsHitKey) == true)//������ ������ ����~, ���� �ð� ����� �ȵ��󰡰Լ�����
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
		);//DetectRadius ���⼭ ����



	if (bResult)//���� ���� ���� ������~ �ʱ�ȭ! �ٵ� ���� �ָ� �����ϳ�,,
	{

		//if (OverlapResults.Num() == 0)//���� �ƹ��� ������
		//{
		//	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, nullptr);
		//	UE_LOG(LogTemp, Warning, TEXT("Nullptr"));
		//	return;
		//}
		ACharacter_Master* Target = nullptr;
		for (auto const& OverlapResult : OverlapResults)//�̹� �����ϸ� �·� ��Ӱ�
		{
			auto Current = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
			//bool same = false;
			ACharacter_Master* Player = Cast< ACharacter_Master>(OverlapResult.GetActor());
			AController* Controller =nullptr;
			if (Player!= nullptr&&Player->bDead == true)//�̹� ���� �ָ�
			{
				continue;
			}
			if(Player)
				Controller=Player->GetController();
			if (Player && Controller&&Controller->IsPlayerController())
			{
				if (Current == Player)//�ϴ� nullptr�� �ƴѰ� ������ Ȯ��
				{
					//���� player�� ���� �ȿ� ����
					return;
				}
				if (Target == nullptr)//ó�� �ɸ� �ָ�
				{
					Target = Player;

				}

				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, Player->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(),
				//	Player->GetActorLocation(), FColor::Blue, false, 0.2f);
				
			}
		}

		if (Target != nullptr)
		{
			auto Monster = Cast<ANPC_Master>(ControllingPawn);
			Monster->NetMulticast_SetTarget(Target);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, Target);
			return;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(ANPCAIController::TargetKey);
		}

	}

}

