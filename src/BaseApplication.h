// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		CaseApplication class. All applications should derive from this.

#pragma once

class BaseApplication {
public:

	BaseApplication() {}
	virtual ~BaseApplication() {}

	void run();
	
	virtual bool startup() = 0;
	virtual void shutdown() = 0;

	virtual bool update(float deltaTime) = 0;
	virtual void draw() = 0;
};