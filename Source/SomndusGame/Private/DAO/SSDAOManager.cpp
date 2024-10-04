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
		if (DAOObject)
		{
			return DAOObject;
		}
	}

	auto* DAOObject = NewObject<USSDAOObjectBase>(this, DAOClass);
	DAOObjects.Add(DAOObject);

	return DAOObject;
}
