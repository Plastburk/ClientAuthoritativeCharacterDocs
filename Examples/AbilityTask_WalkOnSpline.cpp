// Copyright 2023 Anton Hesselbom. All Rights Reserved.

#include "AbilityTask_WalkOnSpline.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

FRootMotionSource_WalkOnSpline::FRootMotionSource_WalkOnSpline() :
	bIsOnSpline(false)
{
}

FVector FRootMotionSource_WalkOnSpline::Update(float DeltaSeconds, const ACharacter* Character, const UCharacterMovementComponent* MovementComponent)
{
	FVector CurrentLocation = Character->GetActorLocation();
	FVector CharacterOffset = FVector(0, 0, bAttachAtFeet ? Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() : 0);

	float SplineKey = Spline->FindInputKeyClosestToWorldLocation(CurrentLocation - CharacterOffset);
	float DistanceAlongSpline = Spline->GetDistanceAlongSplineAtSplineInputKey(SplineKey);

	if (!bIsOnSpline)
	{
		bIsOnSpline = true;
		return Spline->GetLocationAtSplineInputKey(SplineKey, ESplineCoordinateSpace::World) + CharacterOffset;
	}

	FVector SplineForward = Spline->GetDirectionAtSplineInputKey(SplineKey, ESplineCoordinateSpace::World);
	FVector SplineLocation = Spline->GetLocationAtSplineInputKey(SplineKey, ESplineCoordinateSpace::World);
	UKismetSystemLibrary::DrawDebugArrow(Character, SplineLocation, SplineLocation + SplineForward * 100, 10, FLinearColor::Red, 0, 4);

	SplineForward.Z = 0;
	FVector DesiredMovementDirection = Character->GetLastMovementInputVector();
	float Dot = FVector::DotProduct(DesiredMovementDirection, SplineForward);
	float Direction = Dot >= 0.f ? 1.f : -1.f;
	float Input = Direction * DesiredMovementDirection.Size();

	float Distance = Input * Speed * DeltaSeconds;

	if (FMath::IsNearlyZero(Distance))
		return CurrentLocation;
	else
		return Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline + Distance, ESplineCoordinateSpace::World) + CharacterOffset;
}

void FRootMotionSource_WalkOnSpline::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Spline);
	FRootMotionSource::AddReferencedObjects(Collector);
}

UAbilityTask_WalkOnSpline* UAbilityTask_WalkOnSpline::WalkOnSpline(UGameplayAbility* OwningAbility, FName TaskInstanceName, ERootMotionFinishVelocityMode VelocityOnFinishMode, FVector SetVelocityOnFinish, float ClampVelocityOnFinish, USplineComponent* Spline, float Speed, bool bAttachAtFeet)
{
	CA_ABILITYTASK_BOILERPLATE(UAbilityTask_WalkOnSpline);

	MyTask->Spline = Spline;
	MyTask->Speed = Speed;
	MyTask->bAttachAtFeet = bAttachAtFeet;

	return MyTask;
}

FClientAuthoritativeRootMotionSource* UAbilityTask_WalkOnSpline::CreateRootMotionSource()
{
	FRootMotionSource_WalkOnSpline* Source = new FRootMotionSource_WalkOnSpline();

	Source->Spline = Spline;
	Source->Speed = Speed;
	Source->bAttachAtFeet = bAttachAtFeet;

	return Source;
}

void UAbilityTask_WalkOnSpline::OnStart()
{
	Cast<ACharacter>(AbilitySystemComponent->AbilityActorInfo->AvatarActor)->GetCapsuleComponent()->SetCollisionProfileName(TEXT("PawnOnSpline"));
}

void UAbilityTask_WalkOnSpline::OnStop()
{
	Cast<ACharacter>(AbilitySystemComponent->AbilityActorInfo->AvatarActor)->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}
