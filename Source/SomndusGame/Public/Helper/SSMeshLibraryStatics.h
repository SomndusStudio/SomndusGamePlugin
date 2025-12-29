/**
* Copyright (C) Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSMeshLibraryStatics.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
/**
 * Utility class that provides functions to manipulate and retrieve materials on a skinned mesh component.
 */
UCLASS()
class SOMNDUSGAME_API USSMeshLibraryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SS|Rendering|Material")
	static TArray<UMaterialInstanceDynamic*> GetDynamicMaterials(USkinnedMeshComponent* Mesh);
	
	UFUNCTION(BlueprintCallable, Category = "SS|Rendering|Material")
	static UMaterialInstanceDynamic* GetDynamicMaterial(UPrimitiveComponent* Mesh, UMaterialInterface* MaterialInterface, const int32 MaterialIndex);

	/** Set all occurrences of Vector Material Parameters with ParameterName in the set of materials of the SkeletalMesh to ParameterValue */
	UFUNCTION(BlueprintCallable, Category = "SS|Rendering|Material")
	static void SetTextureParameterValueOnMaterials(USkinnedMeshComponent* Mesh, const FName ParameterName, UTexture* ParameterValue);

	UFUNCTION(BlueprintCallable, Category = "SS|Rendering|Material")
	static void SetColorParameterValueOnMaterials(USkinnedMeshComponent* Mesh, const FName ParameterName, FLinearColor InLinearColor, int32 LayerIndex = 0);
};
