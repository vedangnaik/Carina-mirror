#include "DAQHandler.h"

DAQHandler::DAQHandler(DaqDeviceHandle handle) : handle{handle} {
    long long aiSupported;
    UlError err = ulDevGetInfo(handle, DEV_INFO_HAS_AI_DEV, 0, &aiSupported);
    if (err != ERR_NO_ERROR || aiSupported == 0) {
        // shit
    }
}
