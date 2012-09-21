//
//  FakeSMCPlugin.h
//  HWSensors
//
//  Created by mozo on 11/02/12.
//  Copyright (c) 2012 mozodojo. All rights reserved.
//

#ifndef HWSensors_FakeSMCFamily_h
#define HWSensors_FakeSMCFamily_h

#include <IOKit/IOService.h>
#include "FakeSMCDefinitions.h"

#define parent IOService

inline UInt16 swap_value(UInt16 value)
{
    return ((value & 0xff00) >> 8) | ((value & 0xff) << 8);
}
inline UInt16 encode_fpe2(UInt16 value)
{
	return swap_value(value << 2);
}

class FakeSMCPlugin : public IOService {
    OSDeclareAbstractStructors(FakeSMCPlugin)

protected:
    IOService *             fakeSMC;

    bool                    isActive;

public:
    virtual bool            init(OSDictionary *properties=0)
    {
        isActive = false;
        
        return parent::init(properties);
    }
    virtual IOService*      probe(IOService *provider, SInt32 *score)
    {
        if (parent::probe(provider, score) != this)
            return 0;
        
        return this;
    }
    virtual bool            start(IOService *provider)
    {
        if (!parent::start(provider))
            return false;
        
        if (!(fakeSMC = waitForService(serviceMatching(kFakeSMCDeviceService)))) {
            return false;
        }
        
        return true;
    }
    virtual void            stop(IOService *provider)
    {
        fakeSMC->callPlatformFunction(kFakeSMCRemoveHandler, true, this, NULL, NULL, NULL);
        
        parent::stop(provider);
    }
    virtual void            free(void)
    {
        parent:free();
    }

    virtual IOReturn        callPlatformFunction(const OSSymbol *functionName, bool waitForFunction, void *param1, void *param2, void *param3, void *param4 )
    {
        return parent::callPlatformFunction(functionName, waitForFunction, param1, param2, param3,
                                           param4);
    }

};

#endif