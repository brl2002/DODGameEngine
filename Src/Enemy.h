#pragma once

#include "Entity.h"
#include "Behavior.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// Sub-class of Entity, AI controlled entity.
/////////////////////////////////////////////////////////////////////////////////////////////
class Enemy : public Entity
{
	Entity* m_Target;

	bool m_ShouldFindPath;

	float m_Speed;

	Segment* m_LastTargetSegment;

	float m_CheckPathRate;

	float m_CurrentCheckPathTime;

public:
	std::vector<Segment*> path;

	Behavior behavior;

	int currentPathIndex;

	int currentTaskIndex;

public:
	Enemy( char renderCharacter, float speed );

	virtual ~Enemy();

	inline bool ShouldFindPath() { return m_ShouldFindPath; }

	inline void SetShouldFindPath(bool shouldFindPath) { m_ShouldFindPath = shouldFindPath; }

	inline void SetTarget(Entity* entity) { m_Target = entity; }

	inline Entity* GetTarget() { return m_Target; }

	inline void SetLastTargetSegment(Segment* segment) { m_LastTargetSegment = segment; }

	inline Segment* GetLastTargetSegment() { return m_LastTargetSegment; }

	inline float GetSpeed() { return m_Speed; }

	void Update( float deltaTime );
};