#ifndef _INCLUDE_INSPECTCONTROL_H_
	#define _INCLUDE_INSPECTCONTROL_H_

#include "smsdk_ext.h"

class CInspectControl : public SDKExtension
{
	public:
		virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
		virtual void SDK_OnUnload();
};

#endif
