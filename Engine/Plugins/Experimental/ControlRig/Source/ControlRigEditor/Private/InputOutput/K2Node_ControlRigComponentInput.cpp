// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "K2Node_ControlRigComponentInput.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_VariableSet.h"
#include "K2Node_CallFunction.h"
#include "K2Node_DynamicCast.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "ControlRig.h"
#include "Textures/SlateIcon.h"
#include "ControlRigComponent.h"
#include "ControlRigField.h"

#define LOCTEXT_NAMESPACE "K2Node_ControlRigComponentInput"

UK2Node_ControlRigComponentInput::UK2Node_ControlRigComponentInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Gets ControlRig outputs");
}

void UK2Node_ControlRigComponentInput::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	// Create default exec pins
	CreatePin(EGPD_Input, Schema->PC_Exec, FString(), nullptr, Schema->PN_Execute);
	CreatePin(EGPD_Output, Schema->PC_Exec, FString(), nullptr, Schema->PN_Then);

	Super::AllocateDefaultPins();

	CreateInputPins();
}

void UK2Node_ControlRigComponentInput::CreateInputPins()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	TArray<TSharedRef<IControlRigField>> InputInfos = GetInputVariableInfo(DisabledInputs);

	for (const TSharedRef<IControlRigField>& InputInfo : InputInfos)
	{
		UEdGraphPin* InputPin = CreatePin(EGPD_Input, InputInfo->GetPinType(), InputInfo->GetPinString());
		Schema->SetPinAutogeneratedDefaultValueBasedOnType(InputPin);
	}
}

FText UK2Node_ControlRigComponentInput::GetTooltipText() const
{
	return NodeTooltip;
}

FText UK2Node_ControlRigComponentInput::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("ControlRigInput_Title", "Set ControlRig Input");
}

void UK2Node_ControlRigComponentInput::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	if (CompilerContext.bIsFullCompile)
	{
		if (UClass* Class = GetControlRigClass())
		{
			const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

			// Hook off exec path
			UEdGraphPin* ExecPath = FindPinChecked(Schema->PN_Execute);

			// Add getter for ControlRig
			UK2Node_CallFunction* CallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
			CallFunction->SetFromFunction(UControlRigComponent::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UControlRigComponent, BP_GetControlRig)));
			CallFunction->AllocateDefaultPins();
			UEdGraphPin* ControlRigValuePin = CallFunction->GetReturnValuePin();

			// hook up self if we are in an actor
			if (IsInActor())
			{
				UEdGraphPin* SelfPin = CallFunction->FindPinChecked(UEdGraphSchema_K2::PN_Self, EGPD_Input);
				CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(ControlRigComponentPinName, EGPD_Input), *SelfPin);
			}

			// Cast to correct type
			UK2Node_DynamicCast* DynamicCast = CompilerContext.SpawnIntermediateNode<UK2Node_DynamicCast>(this, SourceGraph);
			DynamicCast->TargetType = Class;
			DynamicCast->SetPurity(true);
			DynamicCast->AllocateDefaultPins();

			ControlRigValuePin->MakeLinkTo(DynamicCast->GetCastSourcePin());
			ControlRigValuePin = DynamicCast->GetCastResultPin();
			DynamicCast->NotifyPinConnectionListChanged(DynamicCast->GetCastSourcePin());

			// Hook up input parameter pins to setters
			TArray<UEdGraphPin*> InputPins;
			TArray<TSharedRef<IControlRigField>> FieldInfo;
			GetInputParameterPins(DisabledInputs, InputPins, FieldInfo);

			bool bFirstInput = true;
			for (int32 PinIndex = 0; PinIndex < InputPins.Num(); PinIndex++)
			{
				FieldInfo[PinIndex]->ExpandPin(GetControlRigClass(), CompilerContext, SourceGraph, this, InputPins[PinIndex], ControlRigValuePin, bFirstInput, ExecPath);
				bFirstInput = false;
			}

			if (InputPins.Num() > 0)
			{
				// hook any trailing exec path if we intervened
				UEdGraphPin* ThenPin = FindPinChecked(Schema->PN_Then);
				if (ThenPin->LinkedTo.Num() > 0)
				{
					CompilerContext.MovePinLinksToIntermediate(*ThenPin, *ExecPath);
				}
			}

			BreakAllNodeLinks();
		}
	}
}

#undef LOCTEXT_NAMESPACE
