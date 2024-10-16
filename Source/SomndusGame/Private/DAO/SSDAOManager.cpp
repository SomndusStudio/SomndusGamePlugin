/**
* Copyright (C) 2020-2024 Schartier Isaac
*
* Official Documentation: https://www.somndus-studio.com
*/


#include "DAO/SSDAOManager.h"

USSDAOObjectBase* USSDAOManager::GetDAO(TSubclassOf<USSDAOObjectBase> DAOClass)
{
	for (auto* DAOObject : DAOObjects)
	{
		if (DAOObject && DAOObject->IsA(DAOClass))
		{
			return DAOObject;
		}
	}

	auto* DAOObject = NewObject<USSDAOObjectBase>(this, DAOClass);
	DAOObject->NativeInitialize();
	DAOObjects.Add(DAOObject);

	return DAOObject;
}
