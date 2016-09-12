LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_ROOT:=D:/OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED

  include ${OPENCV_ROOT}/sdk/native/jni/OpenCV.mk


LOCAL_SRC_FILES  := asus_jerry_chen_gr2_GR_Result.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := gr

include $(BUILD_SHARED_LIBRARY)
