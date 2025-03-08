package com.joychic.trace

import android.annotation.SuppressLint
import android.util.Log

/**
 * @Date: 2024/12/5 17:50
 * @author: chi.jiang
 * @Description:
 **/
object SystemPropertiesUtils {
    private const val TAG = "SystemPropertiesUtils"

    @SuppressLint("PrivateApi")
    fun getBoolean(key: String, def: Boolean): Boolean {
        try {
            val systemPropertiesClass = Class.forName("android.os.SystemProperties")
            val getBooleanMethod = systemPropertiesClass.getDeclaredMethod(
                "getBoolean", String::class.java, Boolean::class.java
            )
            getBooleanMethod.isAccessible = true
            return getBooleanMethod.invoke(systemPropertiesClass, key, def) as Boolean
        } catch (e: Throwable) {
            Log.e(TAG, "getBoolean", e)
        }
        return def
    }

    @SuppressLint("PrivateApi", "DiscouragedPrivateApi")
    fun set(key: String, value: String) {
        try {
            val systemPropertiesClass = Class.forName("android.os.SystemProperties")
            val getBooleanMethod = systemPropertiesClass.getDeclaredMethod(
                "set", String::class.java, String::class.java
            )
            getBooleanMethod.isAccessible = true
            getBooleanMethod.invoke(systemPropertiesClass, key, value)
        } catch (e: Throwable) {
            Log.e(TAG, "set", e)
        }
    }
}