#include <jni.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <ReactCommon/CallInvokerHolder.h>
#include "react-native-quick-sqlite.h"
#include "logs.h"
#include <typeinfo>

struct QuickSQLiteBridge : jni::JavaClass<QuickSQLiteBridge>
{
  static constexpr auto kJavaDescriptor = "Lcom/reactnativequicksqlite/QuickSQLiteBridge;";

  static void registerNatives()
  {
    javaClassStatic()->registerNatives({// initialization for JSI
                                        makeNativeMethod("installNativeJsi", QuickSQLiteBridge::installNativeJsi)});
  }

private:
  static void installNativeJsi(jni::alias_ref<jni::JObject> thiz,
                               jlong jsiRuntimePtr,
                               jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder,
                               jni::alias_ref<jni::JString> docPath)
  {
    LOGW("ROPO INSTALLING BINDINGS 2")
    auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);
    auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
    std::string docPathString = docPath->toStdString();

    installSequel(*jsiRuntime, jsCallInvoker, docPathString.c_str());
  }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *)
{
  return jni::initialize(vm, []
                         { QuickSQLiteBridge::registerNatives(); });
}