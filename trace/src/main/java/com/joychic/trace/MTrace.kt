package com.joychic.trace

import android.os.Debug
import com.bytedance.shadowhook.ShadowHook

/**
 * Description:
 *
 * @author zhou.junyou
 * Create by:Android Studio
 * Date:2022/9/22
 */
class MTrace {
    private fun hookStart(config: Config) {
        methodUnHook()
        methodHook(config.methodName, config.tid, config.depth, config.debug)
    }

    private external fun methodHook(methodName: String, tid: Int, depth: Int, debug: Boolean)
    private external fun methodUnHook()

    companion object {
        private val mTrace = MTrace()

        init {
            System.loadLibrary("Trace")
            ShadowHook.init(
                ShadowHook.ConfigBuilder()
                    .setMode(ShadowHook.Mode.SHARED)
                    .build()
            )
        }

        @JvmStatic
        fun start(config: Config = ConfigBuilder().build()) {
            mTrace.hookStart(config)
            Debug.stopMethodTracing()
        }
    }

    class Config constructor(
        val methodName: String,
        val tid: Int,
        val depth: Int,
        val debug: Boolean
    )

    class ConfigBuilder {
        private var methodName: String = ""
        private var tid: Int = -1
        private var depth: Int = 30
        private var debug: Boolean = false


        fun setMethodName(str: String): ConfigBuilder {
            this.methodName = str
            return this
        }

        fun setTid(tid: Int): ConfigBuilder {
            this.tid = tid
            return this
        }

        fun setMethodDepth(depth: Int): ConfigBuilder {
            this.depth = depth
            return this
        }

        fun isDebug(debug: Boolean): ConfigBuilder {
            this.debug = debug
            return this
        }

        fun build(): Config {
            return Config(
                methodName = methodName,
                tid = tid,
                depth = depth,
                debug = debug
            )
        }
    }
}