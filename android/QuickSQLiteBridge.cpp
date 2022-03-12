//
// Created by Sergei Golishnikov on 08/03/2022.
//

#include "QuickSQLiteBridge.h"

#include <utility>
#include "iostream"
#include "react-native-quick-sqlite.h"

using namespace facebook;
using namespace facebook::jni;

using TSelf = local_ref<HybridClass<QuickSQLiteBridge>::jhybriddata>;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
  return facebook::jni::initialize(vm, []
                                   { QuickSQLiteBridge::registerNatives(); });
}

// JNI binding
void QuickSQLiteBridge::registerNatives()
{
  __android_log_print(ANDROID_LOG_VERBOSE, "😇", "registerNatives");
  registerHybrid({
      makeNativeMethod("initHybrid",
                       QuickSQLiteBridge::initHybrid),
      makeNativeMethod("installJSIBindings",
                       QuickSQLiteBridge::installJSIBindings),
  });
}

QuickSQLiteBridge::QuickSQLiteBridge(
    jni::alias_ref<QuickSQLiteBridge::javaobject> jThis,
    jsi::Runtime *rt,
    std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker)
    : javaPart_(jni::make_global(jThis)),
      runtime_(rt),
      jsCallInvoker_(std::move(jsCallInvoker)) {}

// JNI init
TSelf QuickSQLiteBridge::initHybrid(
    alias_ref<jhybridobject> jThis,
    jlong jsContext,
    jni::alias_ref<facebook::react::CallInvokerHolder::javaobject>
        jsCallInvokerHolder)
{

  __android_log_write(ANDROID_LOG_INFO, "🥲", "initHybrid...");
  auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
  return makeCxxInstance(jThis, (jsi::Runtime *)jsContext, jsCallInvoker);
}

void QuickSQLiteBridge::installJSIBindings(
    jlong jsContext,
    jni::alias_ref<facebook::react::CallInvokerHolder::javaobject> jsCallInvokerHolder,
    jstring docPath)
{
  __android_log_print(ANDROID_LOG_VERBOSE, "😇", "installJSIBindings");
  // jThis.env
  auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
  jboolean isCopy;
  const char *docPathString = (env)->GetStringUTFChars(docPath, &isCopy);
  installSequel(*reinterpret_cast<facebook::jsi::Runtime *>(jsContext), jsCallInvoker, docPathString);
}