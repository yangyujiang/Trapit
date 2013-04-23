#ifndef __MY_CONTACT_LISTENER_H__
#define __MY_CONTACT_LISTENER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

class myContactListener:public b2ContactListener
{
protected:
	int count;
	
public:
	myContactListener();
    /// Called when two fixtures begin to touch.
    virtual void BeginContact(b2Contact* contact);
	 /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact);
	 virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

    /// This lets you inspect a contact after the solver is finished. This is useful
    /// for inspecting impulses.
    /// Note: the contact manifold does not include time of impact impulses, which can be
    /// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
    /// in a separate data structure.
    /// Note: this is only called for contacts that are touching, solid, and awake.
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif //__MY_CONTACT_LISTENER_H__