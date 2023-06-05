// Copyright 2023 Fengzz. All Rights Reserved.


// The node logic of "Magic Two Bone IK" is the same as "Two Bone IK", 
// But this node can be used in AnimBlueprint Template ( "Two Bone IK" is not ).
// It just is replicated from FAnimNode_TwoBoneIK, and fix some bugs.
// FAnimNode_TwoBoneIK cannot be used in the AnimBlueprint template at engine version 5.2 and before.
// Because, there is no TargetEditableSkeleton in the AnimBlueprint template.
// See: AnimGraphNodeDetails.cpp => void FBoneSocketTargetCustomization::Build()

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "CommonAnimTypes.h"
#include "AnimationMagic/MagicCore/AnimationMagicTypes.h"
#include "AnimNode_MagicTwoBoneIK.generated.h"

class USkeletalMeshComponent;

/**
 * Simple 2 Bone IK Controller.
 */

USTRUCT(BlueprintInternalUseOnly)
struct ANIMATIONMAGIC_API FAnimNode_MagicTwoBoneIK : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()
	
	/** Name of bone to control. This is the main bone chain to modify from. **/
	UPROPERTY(EditAnywhere, Category=IK)
	FBoneReference IKBone;

	/** Limits to use if stretching is allowed. This value determines when to start stretch. For example, 0.9 means once it reaches 90% of the whole length of the limb, it will start apply. */
	UPROPERTY(EditAnywhere, Category=IK, meta = (editcondition = "bAllowStretching", ClampMin = "0.0", UIMin = "0.0"))
	double StartStretchRatio;

	/** Limits to use if stretching is allowed. This value determins what is the max stretch scale. For example, 1.5 means it will stretch until 150 % of the whole length of the limb.*/
	UPROPERTY(EditAnywhere, Category= IK, meta = (editcondition = "bAllowStretching", ClampMin = "0.0", UIMin = "0.0"))
	double MaxStretchScale;

#if WITH_EDITORONLY_DATA
	/** Limits to use if stretching is allowed - old property DEPRECATED */
	UPROPERTY()
	FVector2D StretchLimits_DEPRECATED;

	/** Whether or not to apply twist on the chain of joints. This clears the twist value along the TwistAxis */
	UPROPERTY()
	bool bNoTwist_DEPRECATED;

	/** If JointTargetSpaceBoneName is a bone, this is the bone to use. **/
	UPROPERTY()
	FName JointTargetSpaceBoneName_DEPRECATED;

	/** If EffectorLocationSpace is a bone, this is the bone to use. **/
	UPROPERTY()
	FName EffectorSpaceBoneName_DEPRECATED;
#endif

	/** Effector Location. Target Location to reach. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effector, meta = (PinShownByDefault))
	FVector EffectorLocation;

	// cached limb index for upper
	FCompactPoseBoneIndex CachedUpperLimbIndex;

	UPROPERTY(EditAnywhere, Category=Effector)
	FMagicBoneSocketTarget EffectorTarget; //FBoneSocketTarget EffectorTarget;

	/** Joint Target Location. Location used to orient Joint bone. **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=JointTarget, meta=(PinShownByDefault))
	FVector JointTargetLocation;

	// cached limb index for lower
	FCompactPoseBoneIndex CachedLowerLimbIndex;

	UPROPERTY(EditAnywhere, Category = JointTarget)
	FMagicBoneSocketTarget JointTarget; //FBoneSocketTarget JointTarget;

	/** Specify which axis it's aligned. Used when removing twist */
	UPROPERTY(EditAnywhere, Category = IK, meta = (editcondition = "!bAllowTwist"))
	FAxis TwistAxis;

	/** Reference frame of Effector Location. */
	UPROPERTY(EditAnywhere, Category=Effector)
	TEnumAsByte<enum EBoneControlSpace> EffectorLocationSpace;

	/** Reference frame of Joint Target Location. */
	UPROPERTY(EditAnywhere, Category=JointTarget)
	TEnumAsByte<enum EBoneControlSpace> JointTargetLocationSpace;

	/** Should stretching be allowed, to be prevent over extension */
	UPROPERTY(EditAnywhere, Category=IK)
	uint8 bAllowStretching:1;

	/** Set end bone to use End Effector rotation */
	UPROPERTY(EditAnywhere, Category=IK)
	uint8 bTakeRotationFromEffectorSpace : 1;

	/** Keep local rotation of end bone */
	UPROPERTY(EditAnywhere, Category = IK)
	uint8 bMaintainEffectorRelRot : 1;

	/** Whether or not to apply twist on the chain of joints. This clears the twist value along the TwistAxis */
	UPROPERTY(EditAnywhere, Category = IK)
	uint8 bAllowTwist : 1;

	FAnimNode_MagicTwoBoneIK();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
#if WITH_EDITOR
	void ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp) const;
#endif // WITH_EDITOR
	// End of FAnimNode_SkeletalControlBase interface
	static FTransform GetTargetTransform(const FTransform& InComponentTransform, FCSPose<FCompactPose>& MeshBases, FMagicBoneSocketTarget& InTarget, EBoneControlSpace Space, const FVector& InOffset);
private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

#if WITH_EDITOR
	FVector CachedJoints[3];
	FVector CachedJointTargetPos;
#endif // WITH_EDITOR
};
