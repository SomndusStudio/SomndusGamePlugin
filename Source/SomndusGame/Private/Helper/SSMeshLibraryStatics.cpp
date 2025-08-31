/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "Helper/SSMeshLibraryStatics.h"

TArray<UMaterialInstanceDynamic*> USSMeshLibraryStatics::GetDynamicMaterials(USkinnedMeshComponent* Mesh)
{
	TArray<UMaterialInstanceDynamic*> MaterialInstances;
	
	const TArray<UMaterialInterface*> MaterialInterfaces = Mesh->GetMaterials();		
	for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); ++MaterialIndex)
	{
		if (UMaterialInterface* MaterialInterface = MaterialInterfaces[MaterialIndex])
		{
			MaterialInstances.Add(GetDynamicMaterial(Mesh, MaterialInterface, MaterialIndex));
		}
	}
	return MaterialInstances;
}

UMaterialInstanceDynamic* USSMeshLibraryStatics::GetDynamicMaterial(USkinnedMeshComponent* Mesh, UMaterialInterface* MaterialInterface, const int32 MaterialIndex)
{
	if (UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface))
	{
		return DynamicMaterial;
	}
	if (UMaterialInstanceDynamic* NewDIM = Mesh->CreateDynamicMaterialInstance(MaterialIndex))
	{
		return NewDIM;
	}
	return nullptr;
}

void USSMeshLibraryStatics::SetTextureParameterValueOnMaterials(
	USkinnedMeshComponent* Mesh,
	const FName ParameterName,
	UTexture* ParameterValue
)
{
	const TArray<UMaterialInterface*> MaterialInterfaces = Mesh->GetMaterials();		
	for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); ++MaterialIndex)
	{
		// 1 :: Try found DIM
		UMaterialInterface* MaterialInterface = nullptr;
		if (auto FoundDIM = MaterialInterfaces[MaterialIndex])
		{
			MaterialInterface = FoundDIM;
		}
		if (UMaterialInstanceDynamic* NewDIM = Mesh->CreateDynamicMaterialInstance(MaterialIndex))
		{
			MaterialInterface = NewDIM;
		}
		// 2 :: Apply texture
		UMaterialInstanceDynamic* DynamicMaterial = GetDynamicMaterial(Mesh, MaterialInterface, MaterialIndex);

		// from classic
		DynamicMaterial->SetTextureParameterValue(ParameterName, ParameterValue);
		
		// from layer
		FMaterialParameterInfo ParameterInfo;
		ParameterInfo.Name = ParameterName;
		ParameterInfo.Index = 0;
		ParameterInfo.Association = EMaterialParameterAssociation::LayerParameter;
		DynamicMaterial->SetTextureParameterValueByInfo(ParameterInfo, ParameterValue);
		
	}
}

void USSMeshLibraryStatics::SetColorParameterValueOnMaterials(USkinnedMeshComponent* Mesh, const FName ParameterName,
	FLinearColor InLinearColor, int32 LayerIndex /* = 0 */)
{
	const FVector Value = FVector(InLinearColor);
	Mesh->SetVectorParameterValueOnMaterials(ParameterName, Value);
	// Layer param
	const TArray<UMaterialInterface*> MaterialInterfaces = Mesh->GetMaterials();		
	for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); ++MaterialIndex)
	{
		if (UMaterialInterface* MaterialInterface = MaterialInterfaces[MaterialIndex])
		{
			UMaterialInstanceDynamic* DynamicMaterial = GetDynamicMaterial(Mesh, MaterialInterface, MaterialIndex);

			// classic
			DynamicMaterial->SetVectorParameterValue(ParameterName, Value);

			// layer
			FMaterialParameterInfo ParameterInfo;
			ParameterInfo.Name = ParameterName;
			ParameterInfo.Index = LayerIndex;
			ParameterInfo.Association = EMaterialParameterAssociation::LayerParameter;
			DynamicMaterial->SetVectorParameterValueByInfo(ParameterInfo, Value);
		}
	}
}