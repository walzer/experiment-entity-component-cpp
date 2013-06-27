LOCAL_PATH := $(call my-dir)

############################ Compiler Flags Setting ############################
PROJ_COMPILE_FLAGS := #-g -fpermissive 

########################## Library Dependencies Setting ########################
PROJ_LDLIBS := -llog

########################## Public Header Files Setting #########################

PROJ_INCLUDE_DIR := $(LOCAL_PATH)/../../../foundation/include

############################# Source Files Setting #############################

#   Define project top source file dirs.
PROJ_SRC_DIRS := $(addprefix $(LOCAL_PATH)/, \
    ../../../foundation/src \
    ./ \
)

#   Define exclude dirs(start with prefix '%', split with blackspace).
PROJ_EXCLUDE_SRC_DIRS := 

#   Define exclude files(start with prefix '%', split with blackspace).
PROJ_EXCLUDE_SRC_FILES := 

#   Find all source sub dirs in defined source dirs.
PROJ_SRC_DIRS :=  $(shell find $(PROJ_SRC_DIRS) -maxdepth 99 -type d)
PROJ_SRC_DIRS :=  $(filter-out $(PROJ_EXCLUDE_SRC_DIRS),$(PROJ_SRC_DIRS))

#   Collect all source files in project source dirs.
PROJ_SRC_FILES := $(foreach sdir,$(PROJ_SRC_DIRS),$(wildcard $(sdir)/*.cpp))
PROJ_SRC_FILES += $(foreach sdir,$(PROJ_SRC_DIRS),$(wildcard $(sdir)/*.c))
PROJ_SRC_FILES := $(subst $(LOCAL_PATH)/,,$(PROJ_SRC_FILES))

#   Filter out exclude source files.
PROJ_SRC_FILES := $(filter-out $(PROJ_EXCLUDE_SRC_FILES), $(PROJ_SRC_FILES))

############################## Compile Project #################################

include $(CLEAR_VARS)

LOCAL_MODULE := foundation

LOCAL_SRC_FILES := ${PROJ_SRC_FILES}

LOCAL_C_INCLUDES:= \
    $(PROJ_INCLUDE_DIR) \
    $(PROJ_SRC_DIRS)

LOCAL_LDLIBS := $(PROJ_LDLIBS)

include $(BUILD_SHARED_LIBRARY)
