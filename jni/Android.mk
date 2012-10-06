LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE :=STATIC
include D:/others/final-blind/OpenCV-2.4.2-android-sdk/OpenCV-2.4.2-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := main_object
LOCAL_LDLIBS +=  -llog -ldl
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_SRC_FILES := testcvjni.cpp cvjni.cpp imageMorphological.cpp findEdges.cpp cornerPointsfilter.cpp

#LOCAL_STATIC_LIBRARIES := cxcore cv

include $(BUILD_SHARED_LIBRARY)
