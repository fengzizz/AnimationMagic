// Copyright 2023 Fengzz. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneIndices.h"
#include "BonePose.h"
#include "BoneContainer.h"
#include "AnimationMagicTypes.generated.h"


USTRUCT(BlueprintType)
struct ANIMATIONMAGIC_API  FMagicBoneSocketTarget
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "MagicBoneSocketTarget")
	bool bUseSocket = false;

	UPROPERTY(EditAnywhere, Category = "MagicBoneSocketTarget", meta = (EditCondition = "!bUseSocket"))
	FBoneReference BoneReference;

	UPROPERTY(EditAnywhere, Category = "MagicBoneSocketTarget", meta = (EditCondition = "bUseSocket"))
	FSocketReference SocketReference;


	FMagicBoneSocketTarget(FName InName = NAME_None, bool bInUseSocket = false)
	{
		bUseSocket = bInUseSocket;

		if (bUseSocket)
		{
			SocketReference.SocketName = InName;
		}
		else
		{
			BoneReference.BoneName = InName;
		}
	}

	void InitializeBoneReferences(const FBoneContainer& RequiredBones)
	{
		if (bUseSocket)
		{
			SocketReference.InitialzeCompactBoneIndex(RequiredBones);
			BoneReference.InvalidateCachedBoneIndex();
		}
		else
		{
			BoneReference.Initialize(RequiredBones);
			SocketReference.InvalidateCachedBoneIndex();
		}
	}

	/** Initialize Bone Reference, return TRUE if success, otherwise, return false **/
	void Initialize(const FAnimInstanceProxy* InAnimInstanceProxy)
	{
		if (bUseSocket)
		{
			SocketReference.InitializeSocketInfo(InAnimInstanceProxy);
		}
	}


	/** return true if valid. Otherwise return false **/
	bool HasValidSetup() const
	{
		if (bUseSocket)
		{
			return SocketReference.HasValidSetup();
		}

		return BoneReference.BoneIndex != INDEX_NONE;
	}

	bool HasTargetSetup() const
	{
		if (bUseSocket)
		{
			return (SocketReference.SocketName != NAME_None);
		}

		return (BoneReference.BoneName != NAME_None);
	}

	FName GetTargetSetup() const
	{
		if (bUseSocket)
		{
			return (SocketReference.SocketName);
		}

		return (BoneReference.BoneName);
	}

	/** return true if valid. Otherwise return false **/
	bool IsValidToEvaluate(const FBoneContainer& RequiredBones) const
	{
		if (bUseSocket)
		{
			return SocketReference.IsValidToEvaluate();
		}

		return BoneReference.IsValidToEvaluate(RequiredBones);
	}

	// this will return the compact pose bone index that matters
	// if you're using socket, it will return socket's related joint's compact pose index
	FCompactPoseBoneIndex GetCompactPoseBoneIndex() const
	{
		if (bUseSocket)
		{
			return SocketReference.GetCachedSocketCompactBoneIndex();
		}

		return BoneReference.CachedCompactPoseIndex;
	}

	/** Get Target Transform from current incoming pose */
	template<typename poseType>
	FTransform GetTargetTransform(const FVector& TargetOffset, FCSPose<poseType>& InPose, const FTransform& InComponentToWorld) const
	{
		FTransform OutTargetTransform;

		auto SetComponentSpaceOffset = [](const FVector& InTargetOffset, const FTransform& LocalInComponentToWorld, FTransform& LocalOutTargetTransform)
		{
			LocalOutTargetTransform.SetIdentity();
			FVector CSTargetOffset = LocalInComponentToWorld.InverseTransformPosition(InTargetOffset);
			LocalOutTargetTransform.SetLocation(CSTargetOffset);
		};

		if (bUseSocket)
		{
			// this has to be done outside
			if (SocketReference.IsValidToEvaluate())
			{
				FTransform SocketTransformInCS = SocketReference.GetAnimatedSocketTransform(InPose);

				FVector CSTargetOffset = SocketTransformInCS.TransformPosition(TargetOffset);
				OutTargetTransform = SocketTransformInCS;
				OutTargetTransform.SetLocation(CSTargetOffset);
			}
			else
			{
				// if none is found, we consider this offset is world offset
				SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
			}
		}
		// if valid data is available
		else if (BoneReference.HasValidSetup())
		{
			if (BoneReference.IsValidToEvaluate() &&
				ensureMsgf(InPose.GetPose().IsValidIndex(BoneReference.CachedCompactPoseIndex), TEXT("Invalid Cached Pose : Name %s(Bone Index (%d), Cached (%d))"), *BoneReference.BoneName.ToString(), BoneReference.BoneIndex, BoneReference.CachedCompactPoseIndex.GetInt()))
			{
				OutTargetTransform = InPose.GetComponentSpaceTransform(BoneReference.CachedCompactPoseIndex);
				FVector CSTargetOffset = OutTargetTransform.TransformPosition(TargetOffset);
				OutTargetTransform.SetLocation(CSTargetOffset);
			}
			else
			{
				// if none is found, we consider this offset is world offset
				SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
			}
		}
		else
		{
			// if none is found, we consider this offset is world offset
			SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
		}

		return OutTargetTransform;
	}

	template<typename poseType>
	FTransform GetTargetTransform(const FTransform& TargetOffset, FCSPose<poseType>& InPose, const FTransform& InComponentToWorld) const
	{
		FTransform OutTargetTransform;

		auto SetComponentSpaceOffset = [](const FTransform& InTargetOffset, const FTransform& LocalInComponentToWorld, FTransform& LocalOutTargetTransform)
		{
			LocalOutTargetTransform = InTargetOffset.GetRelativeTransform(LocalInComponentToWorld);
		};

		if (bUseSocket)
		{
			// this has to be done outside
			if (SocketReference.IsValidToEvaluate())
			{
				OutTargetTransform = TargetOffset * SocketReference.GetAnimatedSocketTransform(InPose);
			}
			else
			{
				SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
			}
		}
		// if valid data is available
		else if (BoneReference.HasValidSetup())
		{
			if (BoneReference.IsValidToEvaluate() &&
				ensureMsgf(InPose.GetPose().IsValidIndex(BoneReference.CachedCompactPoseIndex), TEXT("Invalid Cached Pose : Name %s(Bone Index (%d), Cached (%d))"), *BoneReference.BoneName.ToString(), BoneReference.BoneIndex, BoneReference.CachedCompactPoseIndex.GetInt()))
			{
				OutTargetTransform = TargetOffset * InPose.GetComponentSpaceTransform(BoneReference.CachedCompactPoseIndex);
			}
			else
			{
				SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
			}
		}
		else
		{
			SetComponentSpaceOffset(TargetOffset, InComponentToWorld, OutTargetTransform);
		}

		return OutTargetTransform;
	}
};