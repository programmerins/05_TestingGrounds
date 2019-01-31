// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"


/// Added
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRouteComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the patrol route
	APawn*					ControlledPawn	= OwnerComp.GetAIOwner()->GetPawn();
	UPatrolRouteComponent*	PatrolRouteComponent		= ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!ensure(PatrolRouteComponent))	{ return EBTNodeResult::Failed; }

	// Warn about empty patrol routes
	const TArray<AActor*>	PatrolPoints	= PatrolRouteComponent->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)		{ return EBTNodeResult::Failed; }

	// Set next waypoint
	UBlackboardComponent*	BlackboardComp	= OwnerComp.GetBlackboardComponent();
	int32					Index			= BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle the index
	const int32				NextIndex		= (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	
	return EBTNodeResult::Succeeded;
}