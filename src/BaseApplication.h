// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		BaseApplication class. 

#pragma once

// All applications should derive from this.
class BaseApplication {
public:

	BaseApplication() {}
	virtual ~BaseApplication() {}

	void Run();
	
	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	virtual bool Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};