// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGrippingState : uint8 {
    None           UMETA(DisplayName="None"),
    GripNotHold        UMETA(DisplayName="Gripping"),
    GripAndHold        UMETA(DisplayName="Gripping and Holding Object"),
};