package com.joychic.trace

import android.os.Debug
import com.bytedance.shadowhook.ShadowHook

class MTrace {
    private fun hookStart(config: Config) {
        methodUnHook()
        Debug.stopMethodTracing()
        Debug.stopNativeTracing()
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
        fun start(
            config: Config = Config.Builder()
                .isDebug(false)
                .traceThread(false)
                .setMethodDepth(15)
                .build()
        ) {
            mTrace.hookStart(config)
        }

        @JvmStatic
        fun stop() {
            mTrace.methodUnHook()
        }
    }

    class Config private constructor(
        val methodName: String,
        val tid: Int,
        val depth: Int,
        val debug: Boolean,
        val traceNative: Boolean
    ) {
        class Builder {
            private var methodName: String = ""
            private var tid: Int = -1
            private var depth: Int = 15
            private var debug: Boolean = false
            private var traceThread: Boolean = false

            /**
             * 对 package+class+method 的全路径名进行过滤
             * 匹配失败的堆栈不会进行插桩
             * @param str 路径匹配的字符串，匹配规则 startsWith
             * @return
             */
            fun setFilter(str: String): Builder {
                this.methodName = str
                return this
            }

            /**
             * 只对指定线程插桩
             *
             * @param tid
             * @return
             */
            fun onlyTraceTid(tid: Int): Builder {
                this.tid = tid
                return this
            }

            /**
             * 方法插桩的堆栈深度上限
             *
             * @param depth
             * @return
             */
            fun setMethodDepth(depth: Int): Builder {
                this.depth = depth
                return this
            }

            fun isDebug(debug: Boolean): Builder {
                this.debug = debug
                return this
            }

            /**
             * 是否对线程插桩
             *
             * @param enable
             * @return
             */
            fun traceThread(enable: Boolean): Builder {
                this.traceThread = enable
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
}