// Author:					Todd Hamilton
// Last Change:				02/05/16
// Description of File:		BaseApplication class. 

#pragma once

class Engine;

// All applications should derive from this.
class BaseApplication 
{
public:

	BaseApplication(Engine* a_engine) : engine(a_engine) {}
	virtual ~BaseApplication() {}

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	virtual bool Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	Engine* engine;
};