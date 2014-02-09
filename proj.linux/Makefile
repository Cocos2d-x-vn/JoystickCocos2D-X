EXECUTABLE = JoystickCocos2D

INCLUDES = -I.. -I../Classes

SOURCES = main.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/ContactListener.cpp \
        ../Classes/Stages/StageScene.cpp \
        ../Classes/SneakyInput/SneakyButton.cpp \
        ../Classes/SneakyInput/SneakyButtonSkinnedBase.cpp \
        ../Classes/SneakyInput/SneakyJoystick.cpp \
        ../Classes/SneakyInput/SneakyJoystickSkinnedBase.cpp \
        ../Classes/Extras/ParallaxNodeExtras.cpp \
        ../Classes/Extras/ParallaxScrollNode.cpp \
        ../Classes/Extras/ParallaxScrollOffset.cpp \
        ../Classes/Entities/Entity.cpp \
        ../Classes/Entities/Player.cpp 

COCOS_ROOT = ../../..
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lcocosdenshion
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so $(LIB_DIR)/libcocosdenshion.so
#INCLUDES += -I$(COCOS_ROOT)/CocosDenshion/include
#INCLUDES += -I$(COCOS_ROOT)/external/
INCLUDES +=	 -I../ \
	-I../../ \
	-I../Classes \
	-I$(COCOS_ROOT)/CocosDenshion/include \
	-I$(COCOS_ROOT)/extensions/ \
	-I$(COCOS_ROOT)/external/ \
	-I$(COCOS_ROOT)/external/chipmunk/include/chipmunk

STATICLIBS += \
	$(LIB_DIR)/libextension.a \
	$(LIB_DIR)/libbox2d.a \
	$(LIB_DIR)/libchipmunk.a

	
$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
