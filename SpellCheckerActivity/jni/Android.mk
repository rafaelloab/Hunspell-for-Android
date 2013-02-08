LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := hunspell
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/hunspell/include 
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -ldl -llog

LOCAL_SRC_FILES := \
     hunspell/src/affentry.cpp \
	 hunspell/src/affixmgr.cpp \
     hunspell/src/csutil.cpp \
     hunspell/src/dictmgr.cpp \
	 hunspell/src/filemgr.cpp \
	 hunspell/src/hashmgr.cpp \
	 hunspell/src/hunspell.cpp \
	 hunspell/src/hunzip.cpp \
	 hunspell/src/phonet.cpp \
	 hunspell/src/replist.cpp \
	 hunspell/src/suggestmgr.cpp \
     hunsjni.cpp
include $(BUILD_SHARED_LIBRARY)

