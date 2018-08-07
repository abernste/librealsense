/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (C) 2010-2012 Ken Tossell
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the author nor other contributors may be
*     used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/
/**
* @defgroup ctrl Video capture and processing controls
* @brief Functions for manipulating device settings and stream parameters
*
* The `uvc_get_*` and `uvc_set_*` functions are used to read and write the settings associated
* with the device's input, processing and output units.
*/

#include "windows.h"
#include "winusb_uvc.h"

/***** GENERIC CONTROLS *****/
/**
* @brief Get the length of a control on a terminal or unit.
*
* @param devh UVC device handle
* @param unit Unit or Terminal ID; obtain this from the uvc_extension_unit_t describing the extension unit
* @param ctrl Vendor-specific control number to query
* @return On success, the length of the control as reported by the device. Otherwise,
*   a uvc_error_t error describing the error encountered.
* @ingroup ctrl
*/
int uvc_get_ctrl_len(winusb_uvc_device *devh, uint8_t unit, uint8_t ctrl) {
    char buf[2];

    if (!devh)
        return UVC_ERROR_NO_DEVICE;

    int ret = winusb_SendControl(
        devh->winusbHandle,
        UVC_REQ_TYPE_INTERFACE_GET, 
        UVC_GET_LEN,
        ctrl << 8,
        unit << 8 | devh->deviceData.ctrl_if.bInterfaceNumber,
        buf,
        2);

    if (ret < 0)
        return ret;
    else
        return (unsigned short)SW_TO_SHORT(buf);
}

/**
* @brief Perform a GET_* request from an extension unit.
*
* @param devh UVC device handle
* @param unit Unit ID; obtain this from the uvc_extension_unit_t describing the extension unit
* @param ctrl Control number to query
* @param data Data buffer to be filled by the device
* @param len Size of data buffer
* @param req_code GET_* request to execute
* @return On success, the number of bytes actually transferred. Otherwise,
*   a uvc_error_t error describing the error encountered.
* @ingroup ctrl
*/
int uvc_get_ctrl(winusb_uvc_device *devh, uint8_t unit, uint8_t ctrl, void *data, int len, enum uvc_req_code req_code) {
    if (!devh)
        return UVC_ERROR_NO_DEVICE;

    return winusb_SendControl(
        devh->winusbHandle,
        UVC_REQ_TYPE_INTERFACE_GET, req_code,
        ctrl << 8,
        unit << 8 | devh->deviceData.ctrl_if.bInterfaceNumber,		// XXX saki
        static_cast<char*>(data),
        len);
}

/**
* @brief Perform a SET_CUR request to a terminal or unit.
*
* @param devh UVC device handle
* @param unit Unit or Terminal ID
* @param ctrl Control number to set
* @param data Data buffer to be sent to the device
* @param len Size of data buffer
* @return On success, the number of bytes actually transferred. Otherwise,
*   a uvc_error_t error describing the error encountered.
* @ingroup ctrl
*/
int uvc_set_ctrl(winusb_uvc_device *devh, uint8_t unit, uint8_t ctrl, void *data, int len) {
    if (!devh)
        return UVC_ERROR_NO_DEVICE;

    return winusb_SendControl(
        devh->winusbHandle,
        UVC_REQ_TYPE_INTERFACE_SET, UVC_SET_CUR,
        ctrl << 8,
        unit << 8 | devh->deviceData.ctrl_if.bInterfaceNumber,		// XXX saki
        static_cast<char*>(data),
        len);
}

/***** INTERFACE CONTROLS *****/
uvc_error_t uvc_get_power_mode(winusb_uvc_device *devh, enum uvc_device_power_mode *mode, enum uvc_req_code req_code) {
    uint8_t mode_char;
    int ret;
    if (!devh)
        return UVC_ERROR_NO_DEVICE;

    ret = winusb_SendControl(
        devh->winusbHandle,
        UVC_REQ_TYPE_INTERFACE_GET, req_code,
        UVC_VC_VIDEO_POWER_MODE_CONTROL << 8,
        devh->deviceData.ctrl_if.bInterfaceNumber,
        (char *)&mode_char,
        sizeof(mode_char));

    if (ret == 1) {
        *mode = static_cast<uvc_device_power_mode>(mode_char);
        return UVC_SUCCESS;
    }
    else {
        return static_cast<uvc_error_t>(ret);
    }
}

uvc_error_t uvc_set_power_mode(winusb_uvc_device *devh, enum uvc_device_power_mode mode) {
    uint8_t mode_char = mode;
    int ret;

    if (!devh)
        return UVC_ERROR_NO_DEVICE;

    ret = winusb_SendControl(
        devh->winusbHandle,
        UVC_REQ_TYPE_INTERFACE_SET, UVC_SET_CUR,
        UVC_VC_VIDEO_POWER_MODE_CONTROL << 8,
        devh->deviceData.ctrl_if.bInterfaceNumber,
        (char *)&mode_char,
        sizeof(mode_char));

    if (ret == 1)
        return UVC_SUCCESS;
    else
        return static_cast<uvc_error_t>(ret);
}

/** @todo Request Error Code Control (UVC 1.5, 4.2.1.2) */