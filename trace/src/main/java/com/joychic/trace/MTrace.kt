package com.joychic.trace

import android.os.Debug
import com.bytedance.shadowhook.ShadowHook

class MTrace {
    private fun hookStart(config: Config) {
        methodUnHook()
        methodHook(config.methodName, config.tid, config.depth, config.debug, config.traceNative)
    }

    private external fun methodHook(
        methodName: String,
        tid: Int,
        depth: Int,
        debug: Boolean,
        traceNative: Boolean
    )

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

    class Config(
        val methodName: String,
        val tid: Int,
        val depth: Int,
        val debug: Boolean,
        val traceNative: Boolean
    )

    class ConfigBuilder {
        private var methodName: String = ""
        private var tid: Int = -1
        private var depth: Int = 30
        private var debug: Boolean = false
        private var traceThread: Boolean = true


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

        fun traceNative(debug: Boolean): ConfigBuilder {
            this.traceThread = debug
            return this
        }

        fun build(): Config {
            return Config(
                methodName = methodName,
                tid = tid,
                depth = depth,
                debug = debug,
                traceNative = traceThread
            )
        }
    }
}