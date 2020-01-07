# VULKAN_SDK_PATH = /home/user/VulkanSDK/x.x.x.x/x86_64

#VULKAN_HEADERS = /usr/include/vulkan
#VULKAN_LIBS = /usr/lib/x86_64-linux-gnu
#VULKAN_VALIDATION = /usr/share/vulkan/explicit_layer.d

CFLAGS = -std=c++17 #-I$(VULKAN_HEADERS)
LDFLAGS =  `pkg-config --static --libs glfw3` -lvulkan

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
