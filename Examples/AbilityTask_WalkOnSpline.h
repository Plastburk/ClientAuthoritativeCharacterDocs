// Copyright 2023 Anton Hesselbom. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClientAuthoritativeAbilityTask.h"
#include "Components/SplineComponent.h"
#include "AbilityTask_WalkOnSpline.generated.h"

class USplineComponent;

USTRUCT()
struct SNEAKYGAME_API FRootMotionSource_WalkOnSpline : public FClientAuthoritativeRootMotionSource
{
	GENERATED_BODY()
	CA_ROOT_MOTION_SOURCE_BOILERPLATE(FRootMotionSource_WalkOnSpline)

public:
	FRootMotionSource_WalkOnSpline();

	virtual FVector Update(float DeltaSeconds, const ACharacter* Character, const UCharacterMovementComponent* MovementComponent) override;
	virtual void AddReferencedObjects(class FReferenceCollector& Collector) override;

	UPROPERTY()
	TObjectPtr<USplineComponent> Spline;

	float Speed;
	bool bAttachAtFeet;

private:
	bool bIsOnSpline;
};

UCLASS()
class SNEAKYGAME_API UAbilityTask_WalkOnSpline : public UClientAuthoritativeAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WalkOnSpline* WalkOnSpline(UGameplayAbility* OwningAbility, FName TaskInstanceName, ERootMotionFinishVelocityMode VelocityOnFinishMode, FVector SetVelocityOnFinish, float ClampVelocityOnFinish,
													 USplineComponent* Spline, float Speed, bool bAttachAtFeet);

protected:
	virtual FClientAuthoritativeRootMotionSource* CreateRootMotionSource() override;
	virtual void OnStart() override;
	virtual void OnStop() override;

private:
	UPROPERTY()
	USplineComponent* Spline;

	float Speed;
	bool bAttachAtFeet;
};
