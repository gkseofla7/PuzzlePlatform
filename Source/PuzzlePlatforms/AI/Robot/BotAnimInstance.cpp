// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAnimInstance.h"
#include "GameFramework/Character.h"

void UBotAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	auto MyCharacter = Cast<ACharacter>(Pawn);
	if (::IsValid(MyCharacter))
	{

		CurrenPawnSpeed = MyCharacter->GetVelocity().Size();
		Direction = CalculateDirection(MyCharacter->GetVelocity(), MyCharacter->GetActorRotation());

	}
}