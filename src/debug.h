#pragma once

#include <vulkan/vulkan.h>
#include <ILog.h>
#include <assert.h>

#include <string>


#if defined(VK_CHECK)
    #undef VK_CHECK
#endif

#define VK_CHECK(fun)                                                                                                                                                                        \
{                                                                                                                                                                                            \
    VkResult result = fun;                                                                                                                                                                   \
    /* #fun turns the function name into a string */                                                                                                                                         \
    I_DEBUG_ASSERT_ERROR(result != VK_SUCCESS, "VK_CHECK Error:\nFunction: %s Failed, Vulkan Error Code: %d", #fun, (int)result);                                                                    \
}


#define VK_CHECK_RESULT(result)                                                                                                                                                                        \
{                                                                                                                                                                                            \
    I_DEBUG_ASSERT_ERROR(result != VK_SUCCESS, "VK_CHECK\nVulkan Error Code: %d", (int)result);                                                                    \
}