package com.reactnativequicksqlite;

import androidx.annotation.NonNull;
import android.util.Log;

import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;
import com.facebook.react.module.annotations.ReactModule;

@ReactModule(name = SequelModule.NAME)
class SequelModule extends ReactContextBaseJavaModule {
  public static final String NAME = "QuickSQLiteBridge";
  
  public SequelModule(ReactApplicationContext context) {
    super(context);
  }

  @NonNull
  @Override
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void install() {
    try {
      System.loadLibrary("react-native-quick-sqlite");
      QuickSQLiteBridge.instance.install(getReactApplicationContext());
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
    }
  }
}