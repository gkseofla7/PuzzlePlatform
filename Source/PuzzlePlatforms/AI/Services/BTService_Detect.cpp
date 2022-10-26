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
//매 초마다 계속 계싼
//맞으면 그쪽으로 움직임 범위 있음 속도 빠름
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ANPCAIController::IsHitKey) == true)//맞으면 무조건 따라감~, 일정 시간 벗어나면 안따라가게설정함
		return;
	APawn* ControllingPawn =OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();//내위치
	float DetectRadius = 600.f;

	if (World == nullptr) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(OverlapResults,
		Center,FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
		);//DetectRadius 여기서 설정



	if (bResult)//범위 내에 몬스터 없으면~ 초기화! 근데 범위 멀면 어찌하냐,,
	{

		//if (OverlapResults.Num() == 0)//지금 아무도 없으면
		//{
		//	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ANPCAIController::TargetKey, nullptr);
		//	UE_LOG(LogTemp, Warning, TEXT("Nullptr"));
		//	return;
		//}
		ACharacter_Master* Target = nullptr;
		for (auto const& OverlapResult : OverlapResults)//이미 존재하면 걔로 계속감
		{
			auto Current = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANPCAIController::TargetKey);
			//bool same = false;
			ACharacter_Master* Player = Cast< ACharacter_Master>(OverlapResult.GetActor());
			AController* Controller =nullptr;
			if (Player!= nullptr&&Player->bDead == true)//이미 죽은 애면
			{
				continue;
			}
			if(Player)
				Controller=Player->GetController();
			if (Player && Controller&&Controller->IsPlayerController())
			{
				if (Current == Player)//일단 nullptr이 아닌건 위에서 확보
				{
					//아직 player가 범위 안에 있음
					return;
				}
				if (Target == nullptr)//처음 걸린 애면
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

